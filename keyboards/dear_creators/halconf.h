#include_next "halconf.h"

/* OLED */
#ifdef OLED_ENABLE
#    undef HAL_USE_I2C
#    define HAL_USE_I2C TRUE
#endif

/* Split Keyboard */
#undef HAL_USE_SIO
#define HAL_USE_SIO TRUE
#undef RP_SIO_USE_UART0
#define RP_SIO_USE_UART0 TRUE
#define SERIAL_USART_FULL_DUPLEX
#undef SERIAL_USART_TX_PIN
#define SERIAL_USART_TX_PIN GP12
#undef SERIAL_USART_RX_PIN
#define SERIAL_USART_RX_PIN GP13
#undef SOFT_SERIAL_PIN
