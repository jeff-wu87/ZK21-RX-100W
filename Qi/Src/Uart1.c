#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_Define.h"


typedef unsigned          char UINT8;

//////////////////////////////////////////////////////
unsigned char temp _at_ 0x08;
unsigned char idata itemp _at_ 0x80;
unsigned char xdata xtemp _at_ 0x80;
//////////////////////////////////////////////////////



void InitialUART1_Timer3(uint32_t u32Baudrate) //use timer3 as Baudrate generator
{
    P02_Quasi_Mode;
    P16_Quasi_Mode;

    SCON_1 = 0x42;   //UART1 Mode1,REN_1=1,TI_1=1
    T3CON &= 0xF8;   //T3PS2=0,T3PS1=0,T3PS0=0(Prescale=1)
#ifdef FOSC_110592
    RH3    = HIBYTE(65536 - (691200/u32Baudrate));  /*11.0592 MHz */
    RL3    = LOBYTE(65536 - (691200/u32Baudrate));  /*11.0592 MHz */
#endif
#ifdef FOSC_184320
    RH3    = HIBYTE(65536 - (1152000/u32Baudrate)); /*18.4320 MHz */
    RL3    = LOBYTE(65536 - (1152000/u32Baudrate)); /*18.4320 MHz */
#endif
#ifdef FOSC_160000
    RH3    = HIBYTE(65536 - (1000000/u32Baudrate)-1);  		/*16 MHz */
    RL3    = LOBYTE(65536 - (1000000/u32Baudrate)-1);			/*16 MHz */
#endif
#ifdef FOSC_200000
    RH3    = HIBYTE(65536 - (1250000/u32Baudrate)); /*20.0000 MHz */
    RL3    = LOBYTE(65536 - (1250000/u32Baudrate)); /*20.0000 MHz */
#endif
#ifdef FOSC_221184
    RH3    = HIBYTE(65536 - (1382400/u32Baudrate)); 	/*22.1184 MHz */
    RL3    = LOBYTE(65536 - (1382400/u32Baudrate)); 	/*22.1184 MHz */
#endif
    set_TR3;         //Trigger Timer3
}

char putchar (char c)
{
    while (!TI_1);  /* wait until transmitter ready */
    TI_1 = 0;
    SBUF_1 = c;      /* output character */
    return (c);
}