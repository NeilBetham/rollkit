#ifndef HAP_DEFS_HPP
#define HAP_DEFS_HPP

// Accessory Categories
#define HAP_CAT_OTHER 1
#define HAP_CAT_BRIDGE 2
#define HAP_CAT_FAN 3
#define HAP_CAT_GARAGE 4
#define HAP_CAT_LIGHTBULB 5
#define HAP_CAT_DOOR_LOCK 6
#define HAP_CAT_OUTLET 7
#define HAP_CAT_SWITCH 8
#define HAP_CAT_THERMOSTAT 9
#define HAP_CAT_SENSOR 10
#define HAP_CAT_SECURITY_SYSTEM 11
#define HAP_CAT_DOOR 12
#define HAP_CAT_WINDOW 13
#define HAP_CAT_WINDOW_COVERING 14
#define HAP_CAT_PROGRAMMABLE_SWITCH 15
#define HAP_CAT_RANGE_EXTENDER 16
#define HAP_CAT_IP_CAMERA 17
#define HAP_CAT_VIDEO_DOOR_BELL 18
#define HAP_CAT_AIR_PURIFIER 19

// TVL METHODS
#define HAP_TLV_METHOD_PAIR_SETUP 0
#define HAP_TLV_METHOD_PAIR_SETUP_WITH_AUTH 1
#define HAP_TLV_METHOD_PAIR_VERIFY 2
#define HAP_TLV_METHOD_ADD_PAIRING 3
#define HAD_TLV_METHOD_REMOVE_PAIRING 4
#define HAP_TLV_METHOD_LIST_PAIRINGS 5

// TLV Errors
#define HAP_TLV_ERROR_UNKOWN 0x01
#define HAP_TLV_ERROR_AUTH 0x02
#define HAP_TLV_ERROR_BACKOFF 0x03
#define HAP_TLV_ERROR_MAX_PEERS 0x04
#define HAP_TLV_ERROR_MAX_TRIES 0x05
#define HAP_TLV_ERROR_UNAVAILABLE 0x06
#define HAP_TLV_ERROR_BUSY 0x07

// TLV Types
#define HAP_TLV_TYPE_METHOD 0x00
#define HAP_TLV_TYPE_IDENTIFIER 0x01
#define HAP_TLV_TYPE_SALT 0x02
#define HAP_TLV_TYPE_PUB_KEY 0x03
#define HAP_TLV_TYPE_PROOF 0x04
#define HAP_TLV_TYPE_ENCRYPTED_DATA 0x05
#define HAP_TLV_TYPE_STATE 0x06
#define HAP_TLV_TYPE_ERROR 0x07
#define HAP_TLV_TYPE_RETRY_DELAY 0x08
#define HAP_TLV_TYPE_CERT 0x09
#define HAP_TLV_TYPE_SIGNATURE 0x0A
#define HAP_TLV_TYPE_PERMS 0x0B
#define HAP_TLV_TYPE_FRAGMENT_DATA 0x0C
#define HAP_TLV_TYPE_FRAGMENT_LAST 0x0D
#define HAP_TLV_TYPE_SEPARATOR 0xFF


// Apple HAP UUIDs
#define APPL_HAP_UUID_BASE "%8X-0000-1000-8000-0026BB765291"

// Services
#define APPL_SRVC_UUID_ACCESSORY_INFO 0x3E
#define APPL_SRVC_UUID_FAN 0x40
#define APPL_SRVC_UUID_GARAGE_DOOR_OPENER 0x41
#define APPL_SRVC_UUID_LIGHTBULB 0x43
#define APPL_SRVC_UUID_LOCK_MANAGEMENT 0x44
#define APPL_SRVC_UUID_LOCK_MECHANISM 0x45
#define APPL_SRVC_UUID_OUTLET 0x47
#define APPL_SRVC_UUID_SWITCH 0x49
#define APPL_SRVC_UUID_THERMOSTAT 0x41
#define APPL_SRVC_UUID_AIR_QUALITY_SENSOR 0x8D
#define APPL_SRVC_UUID_SECUIRTY_SYSTEM 0x7E
#define APPL_SRVC_UUID_CARBON_MONOXIDE_SENSOR 0x7F
#define APPL_SRVC_UUID_CONTECT_SENSOR 0x80
#define APPL_SRVC_UUID_DOOR 0x81
#define APPL_SRVC_UUID_HUMIDITY_SENSOR 0x82
#define APPL_SRVC_UUID_LEAK_SENSOR 0x83
#define APPL_SRVC_UUID_LIGHT_SENSOR 0x84
#define APPL_SRVC_UUID_MOTION_SENSOR 0x85
#define APPL_SRVC_UUID_OCCUPANCY_SENSOR 0x86
#define APPL_SRVC_UUID_SMOKE_SENSOR 0x87
#define APPL_SRVC_UUID_STATELESS_PROGRAMMABLE_SWITCH 0x89
#define APPL_SRVC_UUID_TEMPERATURE_SENSOR 0x8A
#define APPL_SRVC_UUID_WINDOW 0x8B
#define APPL_SRVC_UUID_WINDOW_COVERING 0x8C
#define APPL_SRVC_UUID_BATTERY_SERVICE 0x96
#define APPL_SRVC_UUID_CARBON_DIOXIDE_SENSOR 0x97
#define APPL_SRVC_UUID_SPEAKER 0x113
#define APPL_SRVC_UUID_DOORBELL 0x121
#define APPL_SRVC_UUID_FAN_V2 0xB7
#define APPL_SRVC_UUID_SLAT 0xB9
#define APPL_SRVC_UUID_FILTER_MAINTENANCE 0xBA
#define APPL_SRVC_UUID_AIR_PURIFIER 0xBB
#define APPL_SRVC_UUID_SERVICE_LABEL 0xCC
#define APPL_SRVC_UUID_PROTOCOL_INFO 0xA2

// Characteristics
#define APPL_CHAR_UUID_ADMIN_ONLY_ACCESS 0x01
#define APPL_CHAR_UUID_AUDIO_FEEDBACK 0x05
#define APPL_CHAR_UUID_BRIGHTNESS 0x08
#define APPL_CHAR_UUID_COOLING_THRESH_TEMP 0x0D
#define APPL_CHAR_UUID_CURREN_DOOR_STATE 0x0E
#define APPL_CHAR_UUID_CURRENT_HEAT_COOL_STATE 0x0F
#define APPL_CHAR_UUID_CURRENT_RELATIVE_HUMIDITY 0x10
#define APPL_CHAR_UUID_CURRENT_TEMP 0x11
#define APPL_CHAR_UUID_FIRMWARE_REV 0x52
#define APPL_CHAR_UUID_HARDWARE_REV 0x53
#define APPL_CHAR_UUID_HEATING_THRESH_TEMP 0x12
#define APPL_CHAR_UUID_HUE 0x13
#define APPL_CHAR_UUID_IDENTIFY 0x14
#define APPL_CHAR_UUID_LOCK_CONTROL_POINT 0x19
#define APPL_CHAR_UUID_LOCK_CURRENT_STATE 0x1D
#define APPL_CHAR_UUID_LOCK_LAST_KNOWN_ACTION 0x1C
#define APPL_CHAR_UUID_LOCK_MGMT_AUTO_SECUITY_TIMEOUT 0x1A
#define APPL_CHAR_UUID_LOCK_TARGET_STATE 0x1E
#define APPL_CHAR_UUID_LOGS 0x1F
#define APPL_CHAR_UUID_MANUFACTURER 0x20
#define APPL_CHAR_UUID_MODEL 0x21
#define APPL_CHAR_UUID_MOTION_DETECTED 0x22
#define APPL_CHAR_UUID_NAME 0x23
#define APPL_CHAR_UUID_OBSTRUCTION_DETECTED 0x25
#define APPL_CHAR_UUID_ON 0x25
#define APPL_CHAR_UUID_OUTLET_IN_USE 0x26
#define APPL_CHAR_UUID_ROTATION_DIRECTION 0x28
#define APPL_CHAR_UUID_ROTATION_SPEED 0x29
#define APPL_CHAR_UUID_SATURATION 0x2F
#define APPL_CHAR_UUID_SERIAL_NUMBER 0x30
#define APPL_CHAR_UUID_TARGET_DOOR_STATE 0x32
#define APPL_CHAR_UUID_TARGET_HEATING_COOLING_STATE 0x33
#define APPL_CHAR_UUID_TARGET_RELATIVE_HUMIDITY 0x34
#define APPL_CHAR_UUID_TARGET_TEMPERATURE 0x35
#define APPL_CHAR_UUID_TEMPERATURE_DISPLAY_UNITS 0x36
#define APPL_CHAR_UUID_VERSION 0x37
#define APPL_CHAR_UUID_AIR_PARTICULATE_DENSITY 0x64
#define APPL_CHAR_UUID_AIR_PARTICULATE_SIZE 0x65
#define APPL_CHAR_UUID_SECURITY_SYSTEM_CURRENT_STATE 0x66
#define APPL_CHAR_UUID_SECURITY_SYSTEM_TARGET_STATE 0x67
#define APPL_CHAR_UUID_BETTERY_LEVEL 0x68
#define APPL_CHAR_UUID_CARBON_MONOXIDE_DETECTED 0x69
#define APPL_CHAR_UUID_CONTECT_SENSOR_STATE 0x6A
#define APPL_CHAR_UUID_CURRENT_AMBIENT_LIGHT_LEVEL 0x6B
#define APPL_CHAR_UUID_CURRENT_HORIZONTAL_TILT_ANGLE 0x6C
#define APPL_CHAR_UUID_CURRENT_POSITION 0x6D
#define APPL_CHAR_UUID_CURRENT_VERTICAL_TILT_ANGLE 0x6E
#define APPL_CHAR_UUID_HOLD_POSITION 0x6F
#define APPL_CHAR_UUID_LEAK_DETECTED 0x70
#define APPL_CHAR_UUID_OCCUPANCY_DETECTED 0x71
#define APPL_CHAR_UUID_POSITION_STATE 0x72
#define APPL_CHAR_UUID_PROGRAMMABLE_SWITCH_EVENT 0x73
#define APPL_CHAR_UUID_STATUS_ACTIVE 0x75
#define APPL_CHAR_UUID_SMOKE_DETECTED 0x76
#define APPL_CHAR_UUID_STATUS_FAULT 0x77
#define APPL_CHAR_UUID_STATUS_JAMMED 0x78
#define APPL_CHAR_UUID_STATUS_LOW_BATTERY 0x79
#define APPL_CHAR_UUID_STATUS_TAMPERED 0x7A
#define APPL_CHAR_UUID_TARGET_HORIZONTAL_TILT_ANGLE 0x7B
#define APPL_CHAR_UUID_TARGET_POSITION 0x7C
#define APPL_CHAR_UUID_TARGET_VERTICAL_TILT_ANGLE 0x7D
#define APPL_CHAR_UUID_SECUIRTY_SYSTEM_ALARM_TYPE 0x8E
#define APPL_CHAR_UUID_CHARGING_STATE 0x8F
#define APPL_CHAR_UUID_CARBON_MONOXIDE_LEVEL 0x90
#define APPL_CHAR_UUID_CARBON_MONOXIDE_PEAK_LEVEL 0x91
#define APPL_CHAR_UUID_CARBON_DIOXIDE_DETECTED 0x92
#define APPL_CHAR_UUID_CARBON_DIOXIDE_LEVEL 0x93
#define APPL_CHAR_UUID_CARBON_DIOXIDE_PEAK_LEVEL 0x94
#define APPL_CHAR_UUID_AIR_QUALITY 0x95
#define APPL_CHAR_UUID_ACCESORY_FLAGS 0xA6
#define APPL_CHAR_UUID_LOCK_PHY_CONTROLS 0xA7
#define APPL_CHAR_UUID_CURRENT_AIR_PURIFIER_STATE 0xA9
#define APPL_CHAR_UUID_CURRENT_SLAT_STATE 0xAA
#define APPL_CHAR_UUID_SLAT_TYPE 0xC0
#define APPL_CHAR_UUID_FILTER_LIFE_LEVEL 0xAB
#define APPL_CHAR_UUID_FILTER_CHANGE_INDICATION 0xAC
#define APPL_CHAR_UUID_RESET_FILTER_INDICATION 0xAD
#define APPL_CHAR_UUID_TARGET_AIR_PURIFIER_STATE 0xA8
#define APPL_CHAR_UUID_TARGET_FAN_STATE 0xBF
#define APPL_CHAR_UUID_CURRENT_FAN_STATE 0xAF
#define APPL_CHAR_UUID_ACTIVE 0xB0
#define APPL_CHAR_UUID_SWING_MODE 0xB6
#define APPL_CHAR_UUID_CURRENT_TILT_ANGLE 0xC1
#define APPL_CHAR_UUID_TARGET_TILT_ANGLE 0xC2
#define APPL_CHAR_UUID_OZONE_DENSITY 0xC3
#define APPL_CHAR_UUID_NITROGEN_DIOXIDE_DENSITY 0xC4
#define APPL_CHAR_UUID_SULPHUR_DIOXIDE_DENSITY 0xC5
#define APPL_CHAR_UUID_PM2_5_DENSITY 0xC6
#define APPL_CHAR_UUID_PM10_DENSITY 0xC7
#define APPL_CHAR_UUID_VOC_DENSITY 0xC8
#define APPL_CHAR_UUID_SERVICE_LABEL_INDEX 0xCB
#define APPL_CHAR_UUID_SERVICE_LABEL_NAMESPACE 0xCD
#define APPL_CHAR_UUID_COLOR_TEMPERATURE 0xCE


// HAP SRP Params
#define HAP_SRP_MODULUS "\
FFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD129024E088A67CC74\
020BBEA63B139B22514A08798E3404DDEF9519B3CD3A431B302B0A6DF25F1437\
4FE1356D6D51C245E485B576625E7EC6F44C42E9A637ED6B0BFF5CB6F406B7ED\
EE386BFB5A899FA5AE9F24117C4B1FE649286651ECE45B3DC2007CB8A163BF05\
98DA48361C55D39A69163FA8FD24CF5F83655D23DCA3AD961C62F356208552BB\
9ED529077096966D670C354E4ABC9804F1746C08CA18217C32905E462E36CE3B\
E39E772C180E86039B2783A2EC07A28FB5C55DF06F4C52C9DE2BCBF695581718\
3995497CEA956AE515D2261898FA051015728E5A8AAAC42DAD33170D04507A33\
A85521ABDF1CBA64ECFB850458DBEF0A8AEA71575D060C7DB3970F85A6E1E4C7\
ABF5AE8CDB0933D71E8C94E04A25619DCEE3D2261AD2EE6BF12FFA06D98A0864\
D87602733EC86A64521F2B18177B200CBBE117577A615D6C770988C0BAD946E2\
08E24FA074E5AB3143DB5BFCE0FD108E4B82D120A93AD2CAFFFFFFFFFFFFFFFF\
"
#define HAP_SRP_GENERATOR "5"


#endif // HAP_DEFS_HPP