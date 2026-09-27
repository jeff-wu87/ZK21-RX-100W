#include "bsp_flash.h"
 
#include "cw32l010_flash.h"

#define BSP_FLASH_ADDR 512*127

uint8_t bsp_flash_write( uint8_t *flash_buf,uint16_t len )
{
    uint8_t  Flag, tmp8;
    uint16_t i;
    
    if(flash_buf == NULL) return FALSE;
    
    FLASH_UnlockPage(80);
    Flag = CW_FLASH->ISR;
    FLASH_UnlockPages(BSP_FLASH_ADDR, BSP_FLASH_ADDR);    //解锁127页
    Flag = FLASH_ErasePages(BSP_FLASH_ADDR, BSP_FLASH_ADDR);    //擦除最后一页面
    FLASH_LockAllPages();
    if( Flag )
    {
        return FALSE;  //擦除失败
    }
    // blank check
    for( i=0; i<512; i++ )
    {
        tmp8 =  *((volatile uint8_t *)( BSP_FLASH_ADDR + i ));
        if( tmp8 != 0xFF )
        {
            return FALSE;  //对擦除后的结果进行验证失败
        }
    }
     
    FLASH_UnlockPages( BSP_FLASH_ADDR, BSP_FLASH_ADDR );    //解锁最后一个页面
    Flag = FLASH_WriteBytes(BSP_FLASH_ADDR, flash_buf, len );   // 127页按字节写入
    if( Flag )
    {
        return FALSE ; //写入失败
    }
    
    return TRUE;
}

void bsp_flash_read(uint8_t *flash_buf,uint16_t len)
{
    uint8_t i;
    
    for( i=0; i <= len; i ++)
    {
        *flash_buf =  *((volatile uint8_t *)( 512*127 + i ));
         flash_buf++;
          
    }
    
    
    
}

