#ifndef CORE_CORE_CONFIG_H
#define CORE_CORE_CONFIG_H

/*** CLOCK CONFIG PARAMETERS ***/

#define CORE_CLOCK_USE_HSE
#define CORE_CLOCK_HSE_FREQ 25000
#define CORE_CLOCK_SYSCLK_FREQ 160000


/*** CAN CONFIG PARAMETERS ***/

// CAN bitrate in Hz
#define CORE_CAN_BITRATE 1000000

// Number of CAN messages that can be stored in the CAN FreeRTOS queue
#define CORE_CAN_QUEUE_LENGTH 15

// Ports and pins for CAN communication
#define CAN1_PORT GPIOA
#define CAN2_PORT GPIOB
#define CAN3_PORT GPIOB
#define CAN1_PINS (GPIO_PIN_11 | GPIO_PIN_12)
#define CAN2_PINS (GPIO_PIN_12 | GPIO_PIN_13)
#define CAN3_PINS (GPIO_PIN_3 | GPIO_PIN_4)

// Pin alternate function declaration for CAN
#define CORE_FDCAN1_AF GPIO_AF9_FDCAN1
#define CORE_FDCAN2_AF GPIO_AF9_FDCAN2
#define CORE_FDCAN3_AF GPIO_AF11_FDCAN3

// Filters
#define CORE_FDCAN1_MAX_STANDARD_FILTER_NUM 28
#define CORE_FDCAN1_MAX_EXTENDED_FILTER_NUM 8
#define CORE_FDCAN2_MAX_STANDARD_FILTER_NUM 28
#define CORE_FDCAN2_MAX_EXTENDED_FILTER_NUM 8
#define CORE_FDCAN3_MAX_STANDARD_FILTER_NUM 28
#define CORE_FDCAN3_MAX_EXTENDED_FILTER_NUM 8


/*** SPI CONFIG PARAMETERS ***/
#define CORE_SPI1_SCK_PORT  GPIOA
#define CORE_SPI1_SCK_PIN   GPIO_PIN_5
#define CORE_SPI1_SCK_AF    5
#define CORE_SPI1_MISO_PORT GPIOA
#define CORE_SPI1_MISO_PIN  GPIO_PIN_6
#define CORE_SPI1_MISO_AF   5
#define CORE_SPI1_MOSI_PORT GPIOA
#define CORE_SPI1_MOSI_PIN  GPIO_PIN_7
#define CORE_SPI1_MOSI_AF   5

#define CORE_SPI2_SCK_PORT  GPIOB
#define CORE_SPI2_SCK_PIN   GPIO_PIN_13
#define CORE_SPI2_SCK_AF    5
#define CORE_SPI2_MISO_PORT GPIOB
#define CORE_SPI2_MISO_PIN  GPIO_PIN_14
#define CORE_SPI2_MISO_AF   5
#define CORE_SPI2_MOSI_PORT GPIOB
#define CORE_SPI2_MOSI_PIN  GPIO_PIN_15
#define CORE_SPI2_MOSI_AF   5

#define CORE_SPI3_SCK_PORT  GPIOC
#define CORE_SPI3_SCK_PIN   GPIO_PIN_10
#define CORE_SPI3_SCK_AF    6
#define CORE_SPI3_MISO_PORT GPIOC
#define CORE_SPI3_MISO_PIN  GPIO_PIN_11
#define CORE_SPI3_MISO_AF   6
#define CORE_SPI3_MOSI_PORT GPIOC
#define CORE_SPI3_MOSI_PIN  GPIO_PIN_12
#define CORE_SPI3_MOSI_AF   6

#define CORE_SPI4_SCK_PORT  GPIOE
#define CORE_SPI4_SCK_PIN   GPIO_PIN_12
#define CORE_SPI4_SCK_AF    5
#define CORE_SPI4_MISO_PORT GPIOE
#define CORE_SPI4_MISO_PIN  GPIO_PIN_13
#define CORE_SPI4_MISO_AF   5
#define CORE_SPI4_MOSI_PORT GPIOE
#define CORE_SPI4_MOSI_PIN  GPIO_PIN_14
#define CORE_SPI4_MOSI_AF   5


/*** USART CONFIG PARAMETERS ***/
#define CORE_USART_RXBUFLEN 128
#define CORE_USART_RX_TIMEOUT 16

#define CORE_USART1_PORT GPIOC
#define CORE_USART1_PINS (GPIO_PIN_4 | GPIO_PIN_5)
#define CORE_USART2_PORT GPIOB
#define CORE_USART2_PINS (GPIO_PIN_3 | GPIO_PIN_4)
#define CORE_USART3_PORT GPIOB
#define CORE_USART3_PINS (GPIO_PIN_8 | GPIO_PIN_9)

#endif //CORE_CORE_CONFIG_H
