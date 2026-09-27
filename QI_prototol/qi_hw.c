/*
 * qi_hw.c
 * Qi Wireless Power - Hardware Abstraction Layer Implementation
 *
 * STM32 HAL GPIO implementation for COM/COM1 control pins.
 */

#include "qi_hw.h"

#if defined(QI_HW_USE_STM32)

#include "../inc/main.h"

/* ===================== GPIO Configuration ================================= */

//#ifndef QI_HW_COM_GPIO_PORT
//#define QI_HW_COM_GPIO_PORT    GPIOA
//#define QI_HW_COM_PIN          GPIO_PIN_5
//#endif

//#ifndef QI_HW_COM1_GPIO_PORT
//#define QI_HW_COM1_GPIO_PORT   GPIOA
//#define QI_HW_COM1_PIN         GPIO_PIN_6
//#endif

/* ===================== Initialization ===================================== */

void qi_hw_init(void)
{
     
    
}

/* ===================== Hardware Functions ================================= */
//设置1
void qi_hw_com_set(void)
{
    PA06_SETHIGH();//HAL_GPIO_WritePin(QI_HW_COM_GPIO_PORT, QI_HW_COM_PIN, GPIO_PIN_SET);
}
//清零
void qi_hw_com_clear(void)
{
    PA06_SETLOW();//HAL_GPIO_WritePin(QI_HW_COM_GPIO_PORT, QI_HW_COM_PIN, GPIO_PIN_RESET);
}
/*翻转*/
void qi_hw_com_flip(void)
{
    PA06_TOG();//HAL_GPIO_TogglePin(QI_HW_COM_GPIO_PORT, QI_HW_COM_PIN);
}

void qi_hw_com1_set(void)
{
    PA05_SETHIGH();//HAL_GPIO_WritePin(QI_HW_COM1_GPIO_PORT, QI_HW_COM1_PIN, GPIO_PIN_SET);
}

void qi_hw_com1_clear(void)
{
    PA05_SETLOW();//HAL_GPIO_WritePin(QI_HW_COM1_GPIO_PORT, QI_HW_COM1_PIN, GPIO_PIN_RESET);
}

void qi_hw_com1_flip(void)
{
    PA05_TOG();//HAL_GPIO_TogglePin(QI_HW_COM1_GPIO_PORT, QI_HW_COM1_PIN);
}

#else

/* ===================== Fallback: user must implement ======================
 *
 * If QI_HW_USE_STM32 is not defined, implement these functions:
 */

void qi_hw_init(void)
{
    /* Implement GPIO init for your platform */
}

void qi_hw_com_set(void)
{
    /* COM = 1 */
}

void qi_hw_com_clear(void)
{
    /* COM = 0 */
}

void qi_hw_com_flip(void)
{
    /* COM = !COM */
}

void qi_hw_com1_set(void)
{
    /* COM1 = 1 */
}

void qi_hw_com1_clear(void)
{
    /* COM1 = 0 */
}

void qi_hw_com1_flip(void)
{
    /* COM1 = !COM1 */
}

#endif /* QI_HW_USE_STM32 */
