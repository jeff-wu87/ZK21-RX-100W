/**
 * @file cw32l010_uart.c
 * @author WHXY
 * @brief
 * @version 0.1
 * @date 2024-08-07
 *
 * @copyright Copyright (c) 2022
 *
 */
/*******************************************************************************
*
* 代码许可和免责信息
* 武汉芯源半导体有限公司授予您使用所有编程代码示例的非专属的版权许可，您可以由此
* 生成根据您的特定需要而定制的相似功能。根据不能被排除的任何法定保证，武汉芯源半
* 导体有限公司及其程序开发商和供应商对程序或技术支持（如果有）不提供任何明示或暗
* 含的保证或条件，包括但不限于暗含的有关适销性、适用于某种特定用途和非侵权的保证
* 或条件。
* 无论何种情形，武汉芯源半导体有限公司及其程序开发商或供应商均不对下列各项负责，
* 即使被告知其发生的可能性时，也是如此：数据的丢失或损坏；直接的、特别的、附带的
* 或间接的损害，或任何后果性经济损害；或利润、业务、收入、商誉或预期可节省金额的
* 损失。
* 某些司法辖区不允许对直接的、附带的或后果性的损害有任何的排除或限制，因此某些或
* 全部上述排除或限制可能并不适用于您。
*
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "cw32l010_uart.h"
//#include "system_cw32l010.h"
#include "cw32l010_sysctrl.h"
#include "cw32l010_gpio.h"
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include "cw32l010_flash.h"

//#include "variable.h"
#if 0
int end_send=0;
int DATA_LEN=0;
unsigned char UART_RX_DATA[16];
 uint8_t  bchargelessflg;
unsigned char rec_sum=0;
unsigned char UART_TX_DATA[64];
unsigned char UART_RX_DATA[16];
unsigned short UART_temp;
 uint8_t FirstFrameFlag;
 uint8_t Recdly;
 uint16_t Current_1Afull;
 uint8_t Received_data_length;
uint16_t crc16_modbus(const uint8_t *data, uint16_t length);


void module_basicstatus_infamation(void);
void module_fault_infamation(void);
void module_internstate_infamation(void);
void module_startstop_infamation(void);
void module_serialnum_infamation(void);
void CRC16_RevdataPro(void);


uint16_t crc16_modbus(const uint8_t *data, uint16_t length) {
    uint16_t crc = 0xFFFF;
    for (uint16_t i = 0; i < length; i++) {
        crc ^= (uint16_t)data[i];  // 隐含反转：实际按位处理时等效于反转
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x0001) {
                crc = (crc >> 1) ^ 0xA001;  // 0xA001 是 0x8005 的反转
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;  // 输出已隐含反转（因计算中多项式已反转）
}



void UART1_Polling(void)  
{

	if(end_send==1)
	{
		end_send=0;
  	 UART1_receivedata();
		DATA_LEN=0;
		Recdly=0;
		memset(UART_RX_DATA,0,sizeof(UART_RX_DATA));
	}

  if(Recdly)
   {
        Recdly--;
		  if(Recdly<=40&&(UART_RX_DATA[0]==0xC5)&&(UART_RX_DATA[1]==0x5C)&&(UART_RX_DATA[2]==0x0A))
			 {
				  CRC16_RevdataPro();
				  	Recdly=0;
         // Send_Data_Start_charging();				 
		 	 }				 
			  if(Recdly==0)
				{
				
							memset(UART_RX_DATA,0,sizeof(UART_RX_DATA));
							FirstFrameFlag=0; 
             // Send_Data_Start_charging();						
				}
   	}
}

extern uint8_t  RX_Error_Status_Return_TX;
extern uint16_t  wOutVolt;
extern uint16_t  wCurrent;
extern unsigned short Error_Status;
extern uint8_t bNowTempValue;
extern uint16_t gAdcResult[8];
void CRC16_RevdataPro(void)
{
	unsigned short temp_CRC16;
  if(Received_data_length == (UART_RX_DATA[4]+7)) 
	 {
         temp_CRC16=crc16_modbus(&UART_RX_DATA[2], Received_data_length-4); 
		     #if 0
		        UART_TX_DATA[0]=temp_CRC16>>8;
		        UART_TX_DATA[1]=temp_CRC16;
	         	UART_TX_DATA[2]=UART_RX_DATA[Received_data_length-2];
		        UART_TX_DATA[3]=UART_RX_DATA[Received_data_length-1];
		        UART1_Send(UART_TX_DATA, 4);
       	  #endif
		 
       if((temp_CRC16>>8== UART_RX_DATA[Received_data_length-1])&&((temp_CRC16&0x00ff)== UART_RX_DATA[Received_data_length-2]))	
			  {
					
				     switch(UART_RX_DATA[3])  
						 {
							 case 1:             
								  module_basicstatus_infamation();   
							  break;
							 case 2:
								   module_fault_infamation();
							  break; 
							 case 3:
								   module_internstate_infamation();
							  break;
							 case 4:
								   module_startstop_infamation();
							  break; 
							 case 5:
								   module_serialnum_infamation();
							  break; 
							 default:
								 break;
								
						 }
					  memset(UART_RX_DATA,0,sizeof(UART_RX_DATA));
						FirstFrameFlag=0;
       			 				 
			  }
     		
	 }		 
     	
}

extern uint8_t	b29vSt;
extern uint8_t b18v_lowcurrcnt ;
extern uint16_t wChglessTime;
void module_basicstatus_infamation(void)
{
	  unsigned short check_CRC16;
    UART_TX_DATA[0]=0x6B;
		UART_TX_DATA[1]=0xB6;
		UART_TX_DATA[2]=0x0A;
	
		UART_temp=0x01;     //命令字
		UART_TX_DATA[3]=UART_temp;		 
	
    	UART_temp=0x06;        //负载数据长度
		UART_TX_DATA[4]=UART_temp;
	                                //模组充电状态/阶段
	   if(RX_Error_Status_Return_TX>1||(wChglessTime> 9*600))  //0x07：充电异常
     	  UART_temp=0x07;
     else if( b29vSt==2)       //	0x05：充满
        UART_temp=0x05;
		 else if( b29vSt==1)       //	0x04：：恒压充电
        UART_temp=0x04;
		 else if(b18v_lowcurrcnt>3)  //涓流充电
		    UART_temp=0x02;
		 else if(wCurrent>225)      //恒流充电
		     UART_temp=0x03;
		  else 
     		 UART_temp=0x01;         //握手中			
      UART_TX_DATA[5]=UART_temp;
		 
	     	UART_temp=0x00;         //指示接收机、发射机发生了异常
			 if(RX_Error_Status_Return_TX>1)
				  UART_temp=0x01;
       UART_TX_DATA[6]=UART_temp;
	
		   UART_temp=wOutVolt*185/10;  //当前输出电压
			 if(UART_temp>22500&&UART_temp<24000)
				  UART_temp*=0.997;
			 else if(UART_temp>21000&&UART_temp<22500)
				   UART_temp*=0.996;
			 else if(UART_temp<21000&&b18v_lowcurrcnt==0)
			       UART_temp*=0.995;
			  else if(b18v_lowcurrcnt>3)
					   UART_temp*=1.005;
    if( b29vSt==2||(b29vSt==1&&wCurrent<60))
			UART_temp*=1.013;
		else if(b29vSt==1&&wCurrent>60&&wCurrent<140)
		{
			 	UART_temp*=1.008;
		}
		else if(b29vSt==1&&wCurrent>140&&wCurrent<210)
		{
			 	UART_temp*=1.006;
		}
		else if(b29vSt==1&&wCurrent>210&&wCurrent<280)
		{
			 	UART_temp*=1.004;
		}
		
		UART_TX_DATA[8]=UART_temp>>8;	
    UART_TX_DATA[7]=UART_temp;
	

	  UART_temp=wCurrent*150/16*Current_1Afull/300;   //当前输出电流
		UART_TX_DATA[10]=UART_temp>>8;	
		UART_TX_DATA[9]=UART_temp;
	
	//	UART_temp=Error_Status;
	//	UART_TX_DATA[8]=UART_temp;		
	
	  check_CRC16=crc16_modbus(&UART_TX_DATA[2], 9); 
		UART_TX_DATA[12] = check_CRC16>>8;
		UART_TX_DATA[11] = check_CRC16;
		UART1_Send(UART_TX_DATA, 13); 	  
	 
}


void module_fault_infamation(void)
{
	  unsigned short check_CRC16;
    UART_TX_DATA[0]=0x6B;
		UART_TX_DATA[1]=0xB6;
		UART_TX_DATA[2]=0x0A;
	
		UART_temp=0x02;     //命令字
		UART_TX_DATA[3]=UART_temp;		 
	
    	UART_temp=0x05;        //负载数据长度
		  UART_TX_DATA[4]=UART_temp;
                                         //充电异常码
       	UART_temp=0x00;                                 //bit0：充电过压 bit1：充电过流 bit2：充电超时
	 if(RX_Error_Status_Return_TX==7)
	   	UART_temp=0x01; 
	 else if(RX_Error_Status_Return_TX==4)
	   	UART_temp=0x02; 
	 else if(wChglessTime> 9*600)
		 	UART_temp=0x04; 
    UART_TX_DATA[5]=UART_temp;  
	                                  //接收机故障代码
                                    //bit0：接收机控制器温度过温
                                    //bit1：接收机输出短路  
	     UART_temp=0x00;
	  if(RX_Error_Status_Return_TX==3)
	   	  UART_temp=0x01; 
	  else if(RX_Error_Status_Return_TX==6)
	    	UART_temp=0x02;            
    UART_TX_DATA[6]=UART_temp;
		
	
		UART_temp=0;                //发射机故障代码
		UART_TX_DATA[7]=UART_temp;
		
		
		UART_temp=0;                //最近的接收机的故障代码
    UART_TX_DATA[8]=UART_temp;
	

	  UART_temp=0;                //最近的发射机故障代码
		UART_TX_DATA[9]=UART_temp;	
	
	
	//	UART_temp=Error_Status;
	//	UART_TX_DATA[8]=UART_temp;		
	
	  check_CRC16=crc16_modbus(&UART_TX_DATA[2], 8); 
		UART_TX_DATA[11] = check_CRC16>>8;
		UART_TX_DATA[10] = check_CRC16;
		UART1_Send(UART_TX_DATA, 12); 	
	 
}

void module_internstate_infamation(void)
{
		  unsigned short check_CRC16;
    UART_TX_DATA[0]=0x6B;
		UART_TX_DATA[1]=0xB6;
		UART_TX_DATA[2]=0x0A;
	
		UART_temp=0x03;     //命令字
		UART_TX_DATA[3]=UART_temp;		 
	
    	UART_temp=0x04;        //负载数据长度
		  UART_TX_DATA[4]=UART_temp;
                                        
	   	UART_temp=bNowTempValue;            //接收机控制器温度   
     UART_TX_DATA[5]=UART_temp;  
	                                  //接收机线圈盘温度                             
	     	UART_temp=129;            
     UART_TX_DATA[6]=UART_temp;
		
	
		UART_temp=129;                //发射机控制器温度
		UART_TX_DATA[7]=UART_temp;
		
		
		UART_temp=129;                //发射机线圈盘温度 
    UART_TX_DATA[8]=UART_temp;
		
	  check_CRC16=crc16_modbus(&UART_TX_DATA[2], 7); 
		UART_TX_DATA[10] = check_CRC16>>8;
		UART_TX_DATA[9] = check_CRC16;
		UART1_Send(UART_TX_DATA, 11); 
	 
}


void module_startstop_infamation(void)
{
		unsigned short check_CRC16;
    UART_TX_DATA[0]=0x6B;
		UART_TX_DATA[1]=0xB6;
		UART_TX_DATA[2]=0x0A;
	
		UART_temp=0x04;     //命令字
		UART_TX_DATA[3]=UART_temp;		 
	
    	UART_temp=0x02;        //负载数据长度
		  UART_TX_DATA[4]=UART_temp;
                                        
	   	UART_temp=UART_RX_DATA[5];            //充电启停操作 回复的操作 
      UART_TX_DATA[5]=UART_temp;
   		 
	                                  //回复结果充电启停操作    
       if(UART_RX_DATA[5]==1)
			 {				 
	       bchargelessflg=UART_temp=UART_RX_DATA[6];          
         UART_TX_DATA[6]=0x01;
			 }
			 else if(UART_RX_DATA[5]==0) 
			 {
				 UART_TX_DATA[6]=bchargelessflg; 
			 }		
			 else
			 {
		     UART_TX_DATA[6]=  0x01; //UART_RX_DATA[6];   	 
			 }
		
		
	  check_CRC16=crc16_modbus(&UART_TX_DATA[2], 5); 
		UART_TX_DATA[8] = check_CRC16>>8;
		UART_TX_DATA[7] = check_CRC16;
		UART1_Send(UART_TX_DATA, 9); 
	 
}
void module_serialnum_infamation(void)
{
	 		unsigned short check_CRC16;
    UART_TX_DATA[0]=0x6B;
		UART_TX_DATA[1]=0xB6;
		UART_TX_DATA[2]=0x0A;
	
		UART_temp=0x05;     //命令字
		UART_TX_DATA[3]=UART_temp;		 
	  if(UART_RX_DATA[5]==0)   //保留
		{
    	 UART_temp=0x01;        //负载数据长度
		   UART_TX_DATA[4]=UART_temp;
		 	 UART_temp=0;            
       UART_TX_DATA[5]=UART_temp;  
			 		check_CRC16=crc16_modbus(&UART_TX_DATA[2], 4); 
		   UART_TX_DATA[6] = check_CRC16>>8;
		   UART_TX_DATA[7] = check_CRC16;
		   UART1_Send(UART_TX_DATA, 8); 
		}
		else if(UART_RX_DATA[5]==1||(UART_RX_DATA[5]==2))  //1 接收机软件版本  0x02: 接收机硬件版本
		{
    	 UART_temp=0x05;        //负载数据长度
		   UART_TX_DATA[4]=UART_temp;           
       UART_TX_DATA[5]='1';  
			 UART_TX_DATA[6]='.';
			 UART_TX_DATA[7]='0';
			 UART_TX_DATA[8]='.';
			 UART_TX_DATA[9]='0';
			 		check_CRC16=crc16_modbus(&UART_TX_DATA[2], 8); 
		   UART_TX_DATA[10] = check_CRC16>>8;
		   UART_TX_DATA[11] = check_CRC16;
		   UART1_Send(UART_TX_DATA, 12); 
		}
		else if(UART_RX_DATA[5]==3)  //
		{
    	 UART_temp=0x06;        //负载数据长度
		   UART_TX_DATA[4]=UART_temp;           
       UART_TX_DATA[5]='Z';  
			 UART_TX_DATA[6]='C';
			 UART_TX_DATA[7]='0';
			 UART_TX_DATA[8]='2';
			 UART_TX_DATA[9]='0';
			 UART_TX_DATA[10]='9';
			 		check_CRC16=crc16_modbus(&UART_TX_DATA[2], 9); 
		   UART_TX_DATA[12] = check_CRC16>>8;
		   UART_TX_DATA[11] = check_CRC16;
		   UART1_Send(UART_TX_DATA, 13); 
		}
                                        

	                                                          

		


}

void UART1_receivedata()
{
         uint16_t tempfactor_wCurr;
         uint16_t tempfactor_wVolt;
		if (UART_RX_DATA[0] == 0xA5) 
																 
		{
			if (UART_RX_DATA[1] == 0x01||(UART_RX_DATA[1] == 0x07)||(UART_RX_DATA[1] == 0x08))
			{
				if (UART_RX_DATA[DATA_LEN-1] == CHK_SumU8ToU8(UART_RX_DATA,DATA_LEN-1))
				 {

				  switch(UART_RX_DATA[2])
					{
					case 0x10:
						Send_Data_query();
						
						break;
					case 0x11:
						
						bchargelessflg=0;
				//		GPIO4->DO_f.P0 = 1;  
						Send_Data_Stop_charging();
						
						break;
					case 0x12:
						bchargelessflg=1;
				//		GPIO4->DO_f.P0 = 0;
						Send_Data_Start_charging();
						break;
					
					
					case 0x13:
						//testvariable=1;
						break;
					
					case 0x14:
						
						if((UART_RX_DATA[1]==0x07)&&(UART_RX_DATA[3]==0x05)&&(UART_RX_DATA[4]==0xE3)&&(UART_RX_DATA[5]==0x01)&&(UART_RX_DATA[6]==0x16)&&(DATA_LEN==11))
						{
						   tempfactor_wCurr=UART_RX_DATA[7];
						   tempfactor_wVolt=UART_RX_DATA[8]*256+UART_RX_DATA[9];
						   if((tempfactor_wCurr>=50)&&(tempfactor_wCurr<=230)&&(tempfactor_wVolt>=1500)&&(tempfactor_wVolt<=2500))
							  Send_Data_Start_calibration();
						}

						break;
					
					case 0x15:
						 Send_Data_TEST();
						break;

					
					}
					rec_sum=0;
				}
			}
		}
	}

	
	
	
unsigned char CHK_SumU8ToU8(unsigned char *buf,unsigned short len)
{
    unsigned short i;
    unsigned char sum = 0;

    for(i=0;i<len;i++){
        sum += *buf++;
    }
	
    return sum;
}




void Send_Data_query(void)
{
		UART_TX_DATA[0]=0xA5;//Í·Âë
		UART_TX_DATA[1]=0x0B;//³¤¶È
		UART_TX_DATA[2]=0x10;//ÃüÁî	
	
		UART_temp=0;
		UART_TX_DATA[3]=UART_temp;		 //Èí¼þ°æ±¾ÐÅÏ¢
	
	// 	FMC_ErasePage(FMC_USERDATA_ADDR);
	//	fmc_write_bytes(UART_RX_DATA,11);
	
		UART_temp=wOutVolt*185/100;
	//  UART_temp=gAdcResult[0];
		UART_TX_DATA[4]=UART_temp>>8;	 //³äµçµçÑ¹
    UART_TX_DATA[5]=UART_temp;
	
		//UART_temp=wCurrent*48/10*97/95;
	//  	UART_temp=wCurrent*1000/377;
	//  UART_temp=gAdcResult[2];
	//  UART_temp=wCurrent*48/10*8/15;
	    UART_temp=wCurrent*150/16*Current_1Afull/300;
		UART_TX_DATA[6]=UART_temp>>8;	 //³äµçµçÁ÷
		UART_TX_DATA[7]=UART_temp;
	
		UART_temp=Error_Status;
		UART_TX_DATA[8]=UART_temp;		//¹ÊÕÏ´úÂë
	
		UART_temp=bNowTempValue;
		UART_TX_DATA[9]=UART_temp;		//¿ØÖÆÆ÷ÎÂ¶È
	
		UART_temp=0;
    UART_TX_DATA[10]=UART_temp;		//ÏßÈ¦ÅÌÎÂ¶È£¨±£Áô£©
		
		UART_temp=0;
		UART_TX_DATA[11]=UART_temp;		//433Í¸´«Êý¾Ý±êÖ¾
		
		UART_temp=1;
		UART_TX_DATA[12]=UART_temp;		//¶ÔÎ»×´Ì¬ 
		
		UART_temp=CHK_SumU8ToU8(UART_TX_DATA,13);
		UART_TX_DATA[13]=UART_temp;
		UART1_Send(UART_TX_DATA, 14);	

}
void Send_Data_Start_charging(void)
{
		UART_TX_DATA[0]=0xA5;//Í·Âë
		UART_TX_DATA[1]=0x01;//³¤¶È
		UART_TX_DATA[2]=0x12;//ÃüÁî	
		UART_temp=CHK_SumU8ToU8(UART_TX_DATA,3);
		UART_TX_DATA[3]=UART_temp;
		UART1_Send(UART_TX_DATA, 4);	
}

void Send_Data_Stop_charging(void)
{
		UART_TX_DATA[0]=0xA5;//Í·Âë
		UART_TX_DATA[1]=0x01;//³¤¶È
		UART_TX_DATA[2]=0x11;//ÃüÁî	
		UART_temp=CHK_SumU8ToU8(UART_TX_DATA,3);
		UART_TX_DATA[3]=UART_temp;
		UART1_Send(UART_TX_DATA, 4);	
}
extern uint16_t calibration_factor_wCurrent;
extern uint16_t calibration_factor_wOutVolt;
extern uint16_t wPcbTemperature;


void Send_Data_Start_calibration(void)
{
		//Volt25_2vfull=  UART_RX_DATA[3]*256+UART_RX_DATA[4];	
		//Current_1Afull= UART_RX_DATA[5]*256+UART_RX_DATA[6]-128;
	
	
		calibration_factor_wCurrent=UART_RX_DATA[7];
		calibration_factor_wOutVolt=UART_RX_DATA[8]*256+UART_RX_DATA[9];
	
	 	FMC_ErasePage(FMC_USERDATA_ADDR);
		fmc_write_bytes(UART_RX_DATA,11);
	

		UART_TX_DATA[0]=0xA5;//Í·Âë
		UART_TX_DATA[1]=0x01;//³¤¶È
		UART_TX_DATA[2]=0x14;//ÃüÁî	
		UART_temp=CHK_SumU8ToU8(UART_TX_DATA,3);
		UART_TX_DATA[3]=UART_temp;
		UART1_Send(UART_TX_DATA, 4);	
}

extern uint16_t Volt25_2vfull;
extern uint16_t Current_1Afull;

void Send_Data_TEST(void)
{
	

		UART_TX_DATA[0]=0xA5;//Í·Âë
		UART_TX_DATA[1]=0x0E;//³¤¶È
		UART_TX_DATA[2]=0x15;//ÃüÁî	
	
		UART_temp=Volt25_2vfull;
		UART_TX_DATA[3]=UART_temp>>8;
		UART_TX_DATA[4]=UART_temp;
	
		UART_temp=Current_1Afull;
		UART_TX_DATA[5]=UART_temp>>8;
		UART_TX_DATA[6]=UART_temp;
	
		UART_temp=wOutVolt;
		UART_TX_DATA[7]=UART_temp>>8;
		UART_TX_DATA[8]=UART_temp;
	
		UART_TX_DATA[9]=end_send;
		UART_TX_DATA[10]=0;
	
	//	UART_temp=wCurrent*48/10*97/95;
	//    UART_temp=wCurrent*48/10*8/15;
	    UART_temp=wCurrent*150/16*Current_1Afull/300;
		UART_TX_DATA[11]=UART_temp>>8;	 //³äµçµçÁ÷
    UART_TX_DATA[12]=UART_temp;
		UART_temp=wOutVolt*185/100;
		UART_TX_DATA[13]=UART_temp>>8;	 //³äµçµçÑ¹
		UART_TX_DATA[14]=UART_temp;
		
		UART_temp=calibration_factor_wOutVolt;
		UART_TX_DATA[15]=UART_temp>>8; 
		UART_TX_DATA[16]=UART_temp;
		UART_temp=calibration_factor_wCurrent;
		UART_TX_DATA[17]=UART_temp;
		
		UART_temp=bNowTempValue;
		UART_TX_DATA[18]=UART_temp;	
		
		UART_temp=wPcbTemperature;
		UART_TX_DATA[19]=UART_temp;
		
		UART_temp=CHK_SumU8ToU8(UART_TX_DATA,20);
		UART_TX_DATA[20]=UART_temp;
		UART1_Send(UART_TX_DATA, 21);	
}


#endif

/* Private_TypesDefinitions --------------------------------------------------*/

/* Private_Defines -----------------------------------------------------------*/

/* Private_Variables ---------------------------------------------------------*/

/* Private_FunctionPrototypes ------------------------------------------------*/

/* Private_Functions ---------------------------------------------------------*/

/* Public_Functions ----------------------------------------------------------*/



void UART_SendBuf_Polling(UART_TypeDef *UARTx, uint8_t *TxBuf, uint8_t TxCnt)
{
    while (TxCnt)
    {
        UART_SendData_8bit(UARTx, *TxBuf);
        while (UART_GetFlagStatus(UARTx, UART_FLAG_TXE) == RESET);
        TxBuf++;
        TxCnt--;
    }
    while (UART_GetFlagStatus(UARTx, UART_FLAG_TXBUSY) == SET);
}


void UART1_Send(unsigned char *buf,unsigned int len) //·¢Éä°ü
{
	UART_SendBuf_Polling(DEBUG_UARTx, buf, len);
	
	
	#if 0
  unsigned int  i;
	for(i=0;i<len;i++)
	{
		UART1_Sent_Data_Buff[i]=*buf++;
	}
	Cnt_Sent_Data_Start=0;
	Cnt_Sent_Data_End =len;
	UART1->THR =UART1_Sent_Data_Buff[Cnt_Sent_Data_Start];
	Cnt_Sent_Data_Start++;
	#endif
}



/**
 * @brief 通过UARTx发送一个数据(16bit)
 *
 * @param UARTx :UARTx外设
 *        参数可以是:
 *           CW_UART1、CW_UART2、
 * @param Data :待发送的数据
 */
void UART_SendData(UART_TypeDef* UARTx, uint16_t Data)
{
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_DATA_16BIT(Data));

    UARTx->TDR = (Data & (uint16_t)0x01FF);
    
}

/**
 * @brief 通过UARTx发送一个数据(8bit)
 *
 * @param UARTx :UARTx外设
 *        参数可以是:
 *           CW_UART1、CW_UART2、
 * @param Data :待发送的数据
 */
void UART_SendData_8bit(UART_TypeDef* UARTx, uint8_t Data)
{
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_DATA_8BIT(Data));

    UARTx->TDR = Data;
     while (UART_GetFlagStatus(UARTx, UART_FLAG_TXBUSY) == SET);
}
/**
 * @brief 发送字符串
 *
 * @param UARTx :UARTx外设
 *        参数可以是:
 *           CW_UART1、CW_UART2、、
 * @param String :待发送的字符串
 */
void UART_SendString(UART_TypeDef* UARTx, char* String)
{
    while (*String != '\0')
    {
        UART_SendData_8bit(UARTx, *String);
        while (UART_GetFlagStatus(UARTx, UART_FLAG_TXE) == RESET);
        String++;
    }
    while (UART_GetFlagStatus(UARTx, UART_FLAG_TXBUSY) == SET);
}

/**
 * @brief 通过UARTx接收一个数据(16bit)
 *
 * @param UARTx :UARTx外设
 *        参数可以是:
 *           CW_UART1、CW_UART2、
 * @return uint16_t :接收的数据
 */
uint16_t UART_ReceiveData(UART_TypeDef* UARTx)
{
    assert_param(IS_UART_ALL_PERIPH(UARTx));

    return (uint16_t)(UARTx->RDR & (uint16_t)0x01FF);
}

/**
 * @brief 通过UARTx接收一个数据(8bit)
 *
 * @param UARTx :UARTx外设
 *        参数可以是:
 *           CW_UART1、CW_UART2、
 * @return uint8_t :接收的数据
 */
uint8_t UART_ReceiveData_8bit(UART_TypeDef* UARTx)
{
    assert_param(IS_UART_ALL_PERIPH(UARTx));

    return (uint8_t)(UARTx->RDR);
}

/**
 * @brief 根据UART_InitStruct中指定的参数初始化UARTx外设
 *
 * @param UARTx :UARTx外设
 *        参数可以是:
 *           CW_UART1、CW_UART2、
 * @param UART_InitStruct :指向UART_InitTypeDef类型结构体的指针
 */
void UART_Init(UART_TypeDef* UARTx, UART_InitTypeDef* UART_InitStruct)
{
    uint16_t integerdivider = 0x00;
    uint16_t fractionaldivider = 0x00;
    float32_t temp = 0;

    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_BAUDRATE(UART_InitStruct->UART_BaudRate));
    assert_param(IS_UART_Over(UART_InitStruct->UART_Over));
    assert_param(IS_UART_Source(UART_InitStruct->UART_Source));
    assert_param(IS_UART_UCLKFREQ(UART_InitStruct->UART_UclkFreq));
    assert_param(IS_UART_StartBit(UART_InitStruct->UART_StartBit));
    assert_param(IS_UART_STOPBITS(UART_InitStruct->UART_StopBits));
    assert_param(IS_UART_PARITY(UART_InitStruct->UART_Parity));
    assert_param(IS_UART_MODE(UART_InitStruct->UART_Mode));
    assert_param(IS_UART_HARDWARE_FLOW_CONTROL(UART_InitStruct->UART_HardwareFlowControl));
    
    switch((uint32_t)UARTx)
    {
        case (uint32_t)CW_UART1:
            CW_SYSCTRL->APBEN1 = (CW_SYSCTRL->APBEN1 & 0x0000FFFF) | 0x5a5a0000 | SYSCTRL_APBEN1_UART1_Msk; 
            break;
        case (uint32_t)CW_UART2:
            CW_SYSCTRL->APBEN1 = (CW_SYSCTRL->APBEN1 & 0x0000FFFF) | 0x5a5a0000 | SYSCTRL_APBEN1_UART2_Msk; 
            break;
        default:
            break;
    }

    /* UARTx_CR1 */
    REGBITS_MODIFY(UARTx->CR1, UARTx_CR1_SOURCE_Msk | UARTx_CR1_OVER_Msk | UARTx_CR1_START_Msk |\
                   UARTx_CR1_STOP_Msk | UARTx_CR1_PARITY_Msk |\
                   UARTx_CR1_RXEN_Msk | UARTx_CR1_TXEN_Msk    \
                   ,\
                  UART_InitStruct->UART_Source | \
                  (uint32_t)UART_InitStruct->UART_Over |\
                   UART_InitStruct->UART_StartBit       |\
                   UART_InitStruct->UART_StopBits       |\
                   UART_InitStruct->UART_Parity         |\
                   UART_InitStruct->UART_Mode);

    /* UARTx_CR2 */
    REGBITS_MODIFY(UARTx->CR2,  UARTx_CR2_RTSEN_Msk |\
                   UARTx_CR2_CTSEN_Msk  \
                   ,\
                   UART_InitStruct->UART_HardwareFlowControl);
    /* 校验位 */
    if (UART_InitStruct->UART_Parity != UART_Parity_No)
    {
        UARTx->CR1_f.CHLEN = 1;
    }
    else
    {
        UARTx->CR1_f.CHLEN = 0;
    }

    /* 波特率设置 */
    if(UART_Over_16 == UART_InitStruct->UART_Over)
    {
        /* 16倍采样 */
        temp = (float32_t)(UART_InitStruct->UART_UclkFreq) / (16 * (UART_InitStruct->UART_BaudRate));
        integerdivider = (uint16_t)temp;
        fractionaldivider = (uint16_t)((temp - integerdivider)*16 + 0.5);
        UARTx->BRRI = (uint16_t)integerdivider;
        UARTx->BRRF = (uint16_t)fractionaldivider;
    }
    else if(UART_Over_8 == UART_InitStruct->UART_Over)
    {
        /* 8倍采样 */
        integerdivider = (uint16_t)((float32_t)(UART_InitStruct->UART_UclkFreq) / (8 * (UART_InitStruct->UART_BaudRate)) + 0.5);
        UARTx->BRRI = (uint16_t)integerdivider;
    }
    else if(UART_Over_4 == UART_InitStruct->UART_Over)
    {
        /* 4倍采样 */
        integerdivider = (uint16_t)((float32_t)(UART_InitStruct->UART_UclkFreq) / (4 * (UART_InitStruct->UART_BaudRate)) + 0.5);
        UARTx->BRRI = (uint16_t)integerdivider;
    }
    else
    {
        /* 专用采样 */
        /* 专用采样仅适合传输时钟源为LSE或者LSI时，进行2400bps、4800bps或9600bps波特率下的UART通信 */
        integerdivider = (uint16_t)((float32_t)(UART_InitStruct->UART_UclkFreq)*256 / (UART_InitStruct->UART_BaudRate) + 0.5);
        UARTx->BRRI = (uint16_t)integerdivider;
    }
}

/**
 * @brief 默认值赋值UART_InitStruct结构体成员
 *
 * @param UART_InitStruct :指向UART_InitTypeDef类型结构体的指针
 */
void UART_StructInit(UART_InitTypeDef* UART_InitStruct)
{
    /* UART_InitStruct members default value */
    UART_InitStruct->UART_BaudRate = 9600;
    UART_InitStruct->UART_Over = UART_Over_16;
    UART_InitStruct->UART_Source = UART_Source_PCLK;
    UART_InitStruct->UART_StartBit = UART_StartBit_FE;
    UART_InitStruct->UART_StopBits = UART_StopBits_1;
    UART_InitStruct->UART_Parity = UART_Parity_No ;
    UART_InitStruct->UART_Mode = UART_Mode_Rx | UART_Mode_Tx;
    UART_InitStruct->UART_HardwareFlowControl = UART_HardwareFlowControl_None;
}

/**
 * @brief UARTx同步模式初始化，CPOL=1、CPHA=1，只能做主机
 *
 * @param UARTx :UARTx外设
 *        参数可以是:
 *           CW_UART1、CW_UART2、
 * @param UART_ClockInitStruct :指向UART_ClockInitTypeDef类型结构体的指针
 */
void UART_ClockInit(UART_TypeDef* UARTx, UART_ClockInitTypeDef* UART_ClockInitStruct)
{
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_CLOCK(UART_ClockInitStruct->UART_Clock));
    assert_param(IS_UART_Source(UART_ClockInitStruct->UART_Source));

    REGBITS_MODIFY(UARTx->CR1, UARTx_CR1_SYNC_Msk | UARTx_CR1_SOURCE_Msk,\
                   (uint32_t)UART_ClockInitStruct->UART_Clock | \
                   (uint32_t)UART_ClockInitStruct->UART_Source);
}

/**
 * @brief 默认值赋值UART_ClockInitStruct结构体成员
 *
 * @param UART_ClockInitStruct :指向UART_ClockInitTypeDef类型结构体的指针
 */
void UART_ClockStructInit(UART_ClockInitTypeDef* UART_ClockInitStruct)
{
    /* UART_ClockInitStruct members default value */
    UART_ClockInitStruct->UART_Clock = UART_Clock_Disable;
    UART_ClockInitStruct->UART_Source = UART_Source_PCLK;
}

/**
 * @brief UART1去初始化
 *
 */
void UART1_DeInit(void)
{
    CW_SYSCTRL->APBRST1_f.UART1 = 0;
    CW_SYSCTRL->APBRST1_f.UART1 = 1;
}

/**
 * @brief UART2去初始化
 *
 */
void UART2_DeInit(void)
{
    CW_SYSCTRL->APBRST1_f.UART2 = 0;
    CW_SYSCTRL->APBRST1_f.UART2 = 1;
}


/**
 * @brief 使能/失能UARTx中断
 *
 * @param UARTx :UARTx外设
 *        参数可以是:
 *           CW_UART1、CW_UART2、
 * @param UART_IT :中断源
 *   参数可以是:
 *     @arg UART_IT_RXBRK: 间隔段接收完成中断
 *     @arg UART_IT_BAUD:  自动波特率检测完成中断
 *     @arg UART_IT_TIMOV: 定时器溢出中断
 *     @arg UART_IT_CTS:   CTS信号变化中断
 *     @arg UART_IT_PE:    奇偶校验错误中断
 *     @arg UART_IT_FE:    帧结构错误中断
 *     @arg UART_IT_RC:    接收完成中断
 *     @arg UART_IT_TC:    发送完成中断
 *     @arg UART_IT_TXE:   发送缓冲空中断
 * @param NewState :ENABLE or DISABLE
 */
void UART_ITConfig(UART_TypeDef* UARTx, uint16_t UART_IT, FunctionalState NewState)
{
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_IT(UART_IT));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    NewState ? (UARTx->IER |= UART_IT) : (UARTx->IER &= (uint16_t)~UART_IT);

    // if (NewState != DISABLE)
    // {
    //   UARTx->IER |= UART_IT;
    // }
    // else
    // {
    //   UARTx->IER &= (uint16_t)~UART_IT;
    // }
}

/**
 * @brief 获取UARTx中断标志位
 *
 * @param UARTx :UARTx外设
 *        参数可以是:
 *           CW_UART1、CW_UART2、
 * @param UART_IT :中断源
 *   参数可以是：
 *     @arg UART_IT_RXBRK: 间隔段接收完成中断
 *     @arg UART_IT_BAUD:  自动波特率检测完成中断
 *     @arg UART_IT_TIMOV: 定时器溢出中断
 *     @arg UART_IT_CTS:   CTS信号变化中断
 *     @arg UART_IT_PE:    奇偶校验错误中断
 *     @arg UART_IT_FE:    帧结构错误中断
 *     @arg UART_IT_RC:    接收完成中断
 *     @arg UART_IT_TC:    发送完成中断
 *     @arg UART_IT_TXE:   发送缓冲器空中断
 * @return ITStatus :UART_IT当前状态 (SET or RESET)
 */
ITStatus UART_GetITStatus(UART_TypeDef* UARTx, uint16_t UART_IT)
{
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_IT(UART_IT));

    return (REGBITS_GET(UARTx->ISR, UART_IT) ? SET : RESET);
}

/**
 * @brief 清除UARTx中断标志位
 *
 * @param UARTx :UARTx外设
 *        参数可以是:
 *           CW_UART1、CW_UART2、
 * @param UART_IT :中断源
 *   参数可以是：
 *     @arg UART_IT_RXBRK: 间隔段接收完成中断
 *     @arg UART_IT_BAUD:  自动波特率检测完成中断
 *     @arg UART_IT_TIMOV: 定时器溢出中断
 *     @arg UART_IT_CTS:   CTS信号变化中断
 *     @arg UART_IT_PE:    奇偶校验错误中断
 *     @arg UART_IT_FE:    帧结构错误中断
 *     @arg UART_IT_RC:    接收完成中断
 *     @arg UART_IT_TC:    发送完成中断
 * @note
 *    -TXE中断标志位只能通过写UARTx_TDR寄存器来清除
 *    -RC中断标志位只能软件清除，不能通过读UARTx_RDR寄存器来清除
 */
void UART_ClearITPendingBit(UART_TypeDef* UARTx, uint16_t UART_IT)
{
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_CLEAR_IT(UART_IT));

    REGBITS_CLR(UARTx->ICR, UART_IT);
}

/**
 * @brief 获取UARTx标志位
 *
 * @param UARTx :UARTx外设
 *        参数可以是:
 *           CW_UART1、CW_UART2、
 * @param  UART_FLAG :标志
 *   参数可以是：
 *     @arg UART_FLAG_RXBRK:   间隔段接收完成中断
 *     @arg UART_FLAG_BAUD:    自动波特率检测完成中断
 *     @arg UART_FLAG_TIMOV:   定时器溢出中断
 *     @arg UART_FLAG_TXBUSY:  发送忙标志
 *     @arg UART_FLAG_CTSLV:   CTS信号电平状态标志
 *     @arg UART_FLAG_CTS:     CTS信号变化标志
 *     @arg UART_FLAG_MATCH:   从机地址匹配标志
 *     @arg UART_FLAG_PE:      奇偶校验错误标志
 *     @arg UART_FLAG_FE:      帧结构错误标志
 *     @arg UART_FLAG_RC:      接收完成标志
 *     @arg UART_FLAG_TC:      发送完成标志
 *     @arg UART_FLAG_TXE:     发送缓冲器空标志
 * @return FlagStatus :UART_FLAG当前状态 (SET or RESET)
 */
FlagStatus UART_GetFlagStatus(UART_TypeDef* UARTx, uint16_t UART_FLAG)
{
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_FLAG(UART_FLAG));

    return (REGBITS_GET(UARTx->ISR, UART_FLAG) ? SET : RESET);
}

/**
 * @brief 清除UARTx标志位
 *
 * @param UARTx :UARTx外设
 *        参数可以是:
 *           CW_UART1、CW_UART2、
 * @param  UART_FLAG :标志
 *   参数可以是：
 *     @arg UART_FLAG_RXBRK:   间隔段接收完成中断
 *     @arg UART_FLAG_BAUD:    自动波特率检测完成中断
 *     @arg UART_FLAG_TIMOV:   定时器溢出中断
 *     @arg UART_FLAG_CTS:     CTS信号变化标志
 *     @arg UART_FLAG_PE:      奇偶校验错误标志
 *     @arg UART_FLAG_FE:      帧结构错误标志
 *     @arg UART_FLAG_RC:      接收完成标志
 *     @arg UART_FLAG_TC:      发送完成标志
 * @note
 *    -TXBUSY、CTSLV、MATCH标志位由硬件自动设置和清除
 *    -TXE标志位只能通过写UARTx_TDR寄存器来清除
 *    -RC标志位只能软件清除，不能通过读UARTx_RDR寄存器来清除
 */
void UART_ClearFlag(UART_TypeDef* UARTx, uint16_t UART_FLAG)
{
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_CLEAR_FLAG(UART_FLAG));

    REGBITS_CLR(UARTx->ICR, UART_FLAG);
}

/**
 * @brief 使能/失能TXD输出、RXD输入
 *
 * @param UARTx :UARTx外设
 *        参数可以是:
 *           CW_UART1、CW_UART2、
 * @param UART_DirectionMode :传输方向
 *   参数可以是：
 *     @arg UART_Mode_Tx: TXD输出
 *     @arg UART_Mode_Rx: RXD输入
 * @param NewState :ENABLE or DISABLE
 */
void UART_DirectionModeCmd(UART_TypeDef* UARTx, uint16_t UART_DirectionMode, FunctionalState NewState)
{
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_MODE(UART_DirectionMode));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    NewState ? (UARTx->CR1 |= UART_DirectionMode) : (UARTx->CR1 &= (uint16_t)~UART_DirectionMode);

    // if (NewState != DISABLE)
    // {
    //   UARTx->CR1 |= UART_DirectionMode;
    // }
    // else
    // {
    //   UARTx->CR1 &= (uint16_t)~UART_DirectionMode;
    // }
}

/**
 * @brief 使能/失能TXD输出信号、RXD输入信号反相
 *
 * @param UARTx :UARTx外设
 *        参数可以是:
 *           CW_UART1、CW_UART2、
 * @param UART_InvPin :反相引脚
 *   参数可以是：
 *     @arg UART_InvPin_Tx: TXD引脚输出信号反相
 *     @arg UART_InvPin_Rx: RXD引脚输入信号反相
 * @param NewState :ENABLE or DISABLE
 */
void UART_InvPinCmd(UART_TypeDef* UARTx, uint16_t UART_InvPin, FunctionalState NewState)
{
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_INVERSTION_PIN(UART_InvPin));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    NewState ? (UARTx->CR2 |= UART_InvPin) : (UARTx->CR2 &= (uint16_t)~UART_InvPin);

    // if (NewState != DISABLE)
    // {
    //   UARTx->CR2 |= UART_InvPin;
    // }
    // else
    // {
    //   UARTx->CR2 &= (uint16_t)~UART_InvPin;
    // }
}

/**
 * @brief 使能/失能UARTx半双工通信
 *
 * @param UARTx :UARTx外设
 *        参数可以是:
 *           CW_UART1、CW_UART2、
 * @param NewState :ENABLE or DISABLE
 */

void UART_HalfDuplexCmd(UART_TypeDef* UARTx, FunctionalState NewState)
{
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    UARTx->CR1_f.SIGNAL = NewState;
}

/**
 * @brief 使能/失能UARTx DMA
 *
 * @param UARTx :UARTx外设
 *        参数可以是:
 *           CW_UART1、CW_UART2、
 * @param UART_DMAReq :DMA请求
 *   参数可以是：
 *     @arg UART_DMAReq_Tx: DMA发送请求
 *     @arg UART_DMAReq_Rx: DMA接收请求
 * @param NewState :ENABLE or DISABLE
 */
void UART_DMACmd(UART_TypeDef* UARTx, uint16_t UART_DMAReq, FunctionalState NewState)
{
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_DMAREQ(UART_DMAReq));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    NewState ? (UARTx->CR2 |= UART_DMAReq) : (UARTx->CR2 &= (uint16_t)~UART_DMAReq);
}

/**
 * @brief UARTx多机通信配置
 *
 * @param UARTx :UARTx外设
 *        参数可以是:
 *           CW_UART1、CW_UART2、
 * @param UART_Address :从机地址
 * @param UART_AddressMsK :从机地址掩码
 * @note
 *        主机和从机均需配置为自定义校验
 */
void UART_SetMultiMode(UART_TypeDef* UARTx, uint8_t UART_Address, uint8_t UART_AddressMsK)
{
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_ADDRESS(UART_Address));
    assert_param(IS_UART_ADDRESSMSK(UART_AddressMsK));

    REGBITS_SET(UARTx->CR2, UARTx_CR2_ADDREN_Msk);
    REGBITS_MODIFY(UARTx->ADDR, UARTx_ADDR_ADDR_Msk, (uint32_t)UART_Address);
    REGBITS_MODIFY(UARTx->MASK, UARTx_MASK_MASK_Msk, (uint32_t)UART_AddressMsK);
}

/**
 * @brief 使能/失能LIN模式
 * 
 * @param UARTx :UARTx外设
 *        参数可以是:
 *           CW_UART1、CW_UART2、
 * @param NewState :ENABLE or DISABLE
 */
void UART_LINCmd(UART_TypeDef* UARTx, FunctionalState NewState)
{
  assert_param(IS_UART_ALL_PERIPH(UARTx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  UARTx->CR3_f.LIN = NewState;
}


/**
 * @brief 设置LIN模式接收间隔段的长度阈值
 * 
 * @param UARTx :UARTx外设
 *        参数可以是:
 *           CW_UART1、CW_UART2、
 * @param UART_LINBreakDetectLength :间隔段长度阈值
  *   参数可以是：
 *     @arg UART_LINBreakDetectLength_10b: 10bit
 *     @arg UART_LINBreakDetectLength_11b: 11bit
 */
void UART_LINBreakDetectLengthConfig(UART_TypeDef* UARTx, uint16_t UART_LINBreakDetectLength)
{
  assert_param(IS_UART_ALL_PERIPH(UARTx));
  assert_param(IS_UART_LIN_BREAK_DETECT_LENGTH(UART_LINBreakDetectLength));
   
  REGBITS_MODIFY(UARTx->CR3, UARTx_CR3_BRKL_Msk, (uint32_t)UART_LINBreakDetectLength);
}

/**
 * @brief 定时器工作模式配置
 * 
 * @param UARTx :UARTx外设
 *        参数可以是:
 *           CW_UART1、CW_UART2、
 * @param UART_TimerMode :定时器工作模式
  *   参数可以是：
 *     @arg UART_TimerMode_Stop:          停止定时器
 *     @arg UART_TimerMode_TimeOut:       等待超时检测模式
 *     @arg UART_TimerMode_Idle:          接收空闲检测模式
 *     @arg UART_TimerMode_AutoBaudRate1: 自动波特率侦测模式1
 *     @arg UART_TimerMode_AutoBaudRate2: 自动波特率侦测模式2
 *     @arg UART_TimerMode_GeneralTimer:  通用定时功能
 */
void UART_TimerModeConfig(UART_TypeDef* UARTx, uint16_t UART_TimerMode)
{
  assert_param(IS_UART_ALL_PERIPH(UARTx));
  assert_param(IS_UART_TIMER_MODE(UART_TimerMode));

  REGBITS_MODIFY(UARTx->CR2, UARTx_CR2_TIMCR_Msk, (uint32_t)UART_TimerMode);
}

/**
 * @brief 设置定时器重载值
 * 
 * @param UARTx :UARTx外设
 *        参数可以是:
 *           CW_UART1、CW_UART2、
 * @param Autoreload :定时器重载值 范围0x000000~0xFFFFFF
 */
void UART_SetAutoReload(UART_TypeDef* UARTx, uint32_t AutoReload)
{
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_AUTORELOAD(AutoReload));

    UARTx->TIMARR = AutoReload;
}

/**
 * @brief 获取定时器当前计数值
 * 
 * @param UARTx :UARTx外设
 *        参数可以是:
 *           CW_UART1、CW_UART2、
 * @return uint32_t :定时器当前计数值
 */
uint32_t UART_GetCounter(UART_TypeDef* UARTx)
{
    assert_param(IS_UART_ALL_PERIPH(UARTx));

    return UARTx->TIMCNT;
}







