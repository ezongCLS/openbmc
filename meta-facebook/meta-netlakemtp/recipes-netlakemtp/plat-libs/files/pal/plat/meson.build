pal_headers += [
    'pal_power.h',
    ]

pal_deps += [
    cc.find_library('netlakemtp_common'),
    cc.find_library('netlakemtp_fruid'),
    cc.find_library('obmc-i2c'),
    cc.find_library('obmc-sensors'),
    cc.find_library('gpio-ctrl'),
    cc.find_library('nvme-mi'),
    dependency('libphymem'),
    ]

pal_sources += [
    'pal_sensors.c',
    'pal_guid.c',
    'pal_power.c',
    ]
