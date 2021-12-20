#pragma once

#include "msg.hpp"

struct bad_resp_error : public std::runtime_error {
  bad_resp_error(const std::string& field, uint32_t exp, uint32_t val)
      : std::runtime_error(
            "Bad Response Received FIELD:" + field + " Expected: " +
            std::to_string(exp) + " Got: " + std::to_string(val)) {}
};

//---------- Read Holding Registers -------

struct ReadHoldingRegistersReq : public Msg {
  static constexpr uint8_t expected_function = 0x3;
  uint8_t dev_addr = 0;
  uint8_t function = expected_function;
  uint16_t starting_addr = 0;
  uint16_t reg_count = 0;

  ReadHoldingRegistersReq(uint8_t a, uint16_t reg_off, uint16_t cnt);
  ReadHoldingRegistersReq() {}
  void encode() override;
};

struct ReadHoldingRegistersResp : public Msg {
  static constexpr uint8_t expected_function = 0x3;
  uint8_t dev_addr = 0;
  uint8_t function = 0;
  uint8_t byte_count = 0;
  std::vector<uint16_t>& regs;
  explicit ReadHoldingRegistersResp(std::vector<uint16_t>& r);
  void decode() override;
};
