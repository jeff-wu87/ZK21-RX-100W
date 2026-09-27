#ifndef __BSP_FLASH_H
#define __BSP_FLASH_H
#include <stdint.h>
#include <stdbool.h>
uint8_t bsp_flash_write( uint8_t *flash_buf,uint16_t len);
void bsp_flash_read(uint8_t *flash_buf,uint16_t len);

#endif

