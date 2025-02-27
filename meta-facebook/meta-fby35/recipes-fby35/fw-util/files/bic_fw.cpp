#include <cstdio>
#include <iomanip>
#include <sstream>
#include <fcntl.h>
#include <syslog.h>
#include <unistd.h>
#include <facebook/fby35_common.h>
#include <facebook/bic.h>
#include <facebook/bic_ipmi.h>
#include "bic_fw.h"

using namespace std;

image_info BicFwComponent::check_image(const string& image, bool force) {
  int ret = 0;
  uint8_t board_rev = 0;
  image_info image_sts = {"", false, false};

  if (force == true) {
    image_sts.result = true;
  }

  switch (fw_comp) {
    case FW_SB_BIC:
      ret = get_board_rev(slot_id, BOARD_ID_SB, &board_rev);
      break;
    case FW_BB_BIC:
      ret = get_board_rev(slot_id, BOARD_ID_BB, &board_rev);
      break;
  }
  if (ret < 0) {
    cerr << "Failed to get board revision ID" << endl;
    return image_sts;
  }

  if (fby35_common_is_valid_img(image.c_str(), fw_comp, board_rev) == true) {
    image_sts.result = true;
    image_sts.sign = true;
  }

  return image_sts;
}

int BicFwComponent::update_internal(const string& image, bool force) {
  int ret = FW_STATUS_FAILURE;
  image_info image_sts = check_image(image, force);

  if (image_sts.result == false) {
    syslog(LOG_CRIT, "Update %s on %s Fail. File: %s is not a valid image",
           get_component_name(fw_comp), fru().c_str(), image.c_str());
    return FW_STATUS_FAILURE;
  }

  try {
    if (fw_comp != FW_BIC_RCVY) {
      server.ready();
      expansion.ready();
    }
  } catch (string& err) {
    cerr << err << endl;
    return FW_STATUS_NOT_SUPPORTED;
  }

  ret = bic_update_fw(slot_id, fw_comp, (char *)image.c_str(), force);
  if (ret != BIC_FW_UPDATE_SUCCESS) {
    switch (ret) {
      case BIC_FW_UPDATE_FAILED:
        cerr << this->alias_component() << ": update process failed" << endl;
        break;
      case BIC_FW_UPDATE_NOT_SUPP_IN_CURR_STATE:
        cerr << this->alias_component() << ": firmware update not supported in current state." << endl;
        break;
      default:
        cerr << this->alias_component() << ": unknow error (ret: " << ret << ")" << endl;
        break;
    }
    return FW_STATUS_FAILURE;
  }

  return ret;
}

int BicFwComponent::update(const string image) {
  return update_internal(image, false);
}

int BicFwComponent::fupdate(const string image) {
  return update_internal(image, true);
}

int BicFwComponent::get_ver_str(string& s) {
  int ret = 0;
  uint8_t rbuf[32] = {0};

  ret = bic_get_fw_ver(slot_id, fw_comp, rbuf);
  if (!ret) {
    stringstream ver;
    size_t len = strlen((char *)rbuf);
    if (len >= 4) {         // new version format
      ver << "oby35-" << string((char *)(rbuf + 4)) << "-v" << hex << setfill('0')
          << setw(2) << (int)rbuf[0] << setw(2) << (int)rbuf[1] << "."
          << setw(2) << (int)rbuf[2] << "." << setw(2) << (int)rbuf[3];
    } else if (len == 2) {  // old version format
      ver << "v" << hex << (int)rbuf[0] << "." << setfill('0') << setw(2) << (int)rbuf[1];
    } else {
      ver << "Format not supported";
    }
    s = ver.str();
  }

  return ret;
}

int BicFwComponent::print_version() {
  string ver("");
  string board_name = board;

  if (fw_comp == FW_BIC_RCVY) {
    return FW_STATUS_NOT_SUPPORTED;
  }

  transform(board_name.begin(), board_name.end(), board_name.begin(), ::toupper);
  try {
    server.ready();
    expansion.ready();
    if (get_ver_str(ver) < 0) {
      throw "Error in getting the version of " + board_name;
    }
    cout << board_name << " Bridge-IC Version: " << ver << endl;
  } catch (string& err) {
    cout << board_name + " Bridge-IC Version: NA (" + err + ")" << endl;
  }

  return FW_STATUS_SUCCESS;
}

void BicFwComponent::get_version(json& j) {
  string ver("");
  string board_name = board;

  try {
    server.ready();
    expansion.ready();
    if (get_ver_str(ver) < 0) {
      throw "Error in getting the version of " + board_name;
    }
    j["VERSION"] = ver;
  } catch (string& err) {
    if (err.find("empty") != string::npos) {
      j["VERSION"] = "not_present";
    } else {
      j["VERSION"] = "error_returned";
    }
  }
}
