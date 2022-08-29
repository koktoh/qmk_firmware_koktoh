#include_next "mcuconf.h"

/* OLED */
#ifdef OLED_ENABLE
#    undef RP_I2C_USE_I2C0
#    define RP_I2C_USE_I2C0 FALSE
#    undef RP_I2C_USE_I2C1
#    define RP_I2C_USE_I2C1 TRUE
#    define I2C_DRIVER I2CD2
#    define I2C1_SDA_PIN GP2
#    define I2C1_SCL_PIN GP3
#endif
