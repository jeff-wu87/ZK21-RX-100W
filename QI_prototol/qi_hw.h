/*
 * qi_hw.h
 * Qi Wireless Power - Hardware Abstraction Layer
 *
 * All COM/COM1 GPIO operations are defined here.
 * Port to a new MCU by implementing these 6 functions only.
 */

#ifndef QI_HW_H
#define QI_HW_H

#include <stdint.h>
#define  QI_HW_USE_STM32 1
/* ===================== User Config: GPIO Pins =============================
 *
 * Define these macros before including this header:
 *
 *   QI_HW_COM_GPIO_PORT   - GPIO port for COM pin
 *   QI_HW_COM_PIN         - GPIO pin for COM pin
 *   QI_HW_COM1_GPIO_PORT  - GPIO port for COM1 pin
 *   QI_HW_COM1_PIN        - GPIO pin for COM1 pin
 *
 * Example (STM32G431):
 *   #define QI_HW_COM_GPIO_PORT    GPIOA
 *   #define QI_HW_COM_PIN          GPIO_PIN_5
 *   #define QI_HW_COM1_GPIO_PORT   GPIOB
 *   #define QI_HW_COM1_PIN         GPIO_PIN_6
 */

/* ===================== Public API ========================================= */

/**
 * @brief  Initialize COM/COM1 GPIO pins
 * @note   Call once at system startup
 */
void qi_hw_init(void);

/**
 * @brief  Set COM pin high  (equivalent to PIN_COM_1)
 */
void qi_hw_com_set(void);

/**
 * @brief  Clear COM pin low (equivalent to PIN_COM_0)
 */
void qi_hw_com_clear(void);

/**
 * @brief  Toggle COM pin   (equivalent to PIN_COM_flip)
 */
void qi_hw_com_flip(void);

/**
 * @brief  Set COM1 pin high (equivalent to PIN_COM1_1)
 */
void qi_hw_com1_set(void);

/**
 * @brief  Clear COM1 pin low (equivalent to PIN_COM1_0)
 */
void qi_hw_com1_clear(void);

/**
 * @brief  Toggle COM1 pin  (equivalent to PIN_COM1_flip)
 */
void qi_hw_com1_flip(void);

#endif /* QI_HW_H */
