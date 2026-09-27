


#include  "config.h"
//#include  "variable_extern.h"
#include  "variable.h"

#include <stdio.h>
#include "cw32l010_flash.h"
#include <math.h>
#include "cw32l010_uart.h"
#include "string.h"
#include "stdlib.h"


//#include "fmc.h"
int Led_error=0;
//uint16_t wCapTimeCnt;
extern uint16_t gAdcResult[8];
uint8_t bDelay1ms;
uint8_t fgDelay1msFlag;
uint16_t wBreathLEDDami;
uint8_t bfgBreathLEDDir;
uint8_t bfgBreathLEDFlag;
uint8_t over_wCurrent=0;
uint8_t lowcurstbit=0;
 uint16_t ctlmostm=0;
extern unsigned short Error_Status;
unsigned short Error_Status=0;

int Error_Status_count=0;

unsigned char UART_TX_DATA_test[64];

extern uint16_t calibration_factor_wCurrent;
extern uint16_t calibration_factor_wOutVolt;
uint16_t calibration_factor_wCurrent=128;
uint16_t calibration_factor_wOutVolt=2048;

extern unsigned int AFC_DelayCnt;
unsigned char AFC_Do_indx;
unsigned char AFC_Do_times;

//#define  Volt25_2vfull   (5*256+96-28)

uint8_t bvltcurr_buf[10]=  {0};

//extern void New_Clear(u8 indx);
extern void LED_SetMode(LED_MODE_TypeDef mode);
extern void Uart0revProc(void);
extern void In_band_communication(void);
void F_Breath_LED_Do(void);
void adcInit(uint8_t ch);

extern void ATIM_Code_Count_Config(void);
//extern void UART_SendString(UART_TypeDef *UARTx, char *String);

void F_Phase_Packet_Control(void);
uint8_t F_GetNowTemp(uint16_t adc_val);



//uint16_t ADC_GetADCResult(void);


//(uint16_t)(((uint32_t)(100*4095)/(6*D_ADC_V_MAX)))
#define D_VIN_12V_AD_V      1365
//(uint16_t)(((uint32_t)(80*4095)/(6*D_ADC_V_MAX)))
#define D_VIN_9V_AD_V       1092  




#define D_QCDM_0_SET_1  	  {GPIO3->DO_f.P0 = 1;}//P26=1;
#define D_QCDM_0_SET_0  	  {GPIO3->DO_f.P0 = 0;}//P26=0;
#define D_QCDM_1_SET_1  	  {GPIO4->DO_f.P4 = 1;}//P03=1;
#define D_QCDM_1_SET_0  	  {GPIO4->DO_f.P4 = 0;}//P03=0;


#define D_QCDP_0_SET_1  	  {GPIO3->DO_f.P6 = 1;}//P25=1;
#define D_QCDP_0_SET_0  	  {GPIO3->DO_f.P6 = 0;}//P25=0;

#define D_AFC_SET_1  	  {}//P36=1;}
#define D_AFC_SET_0  	  {}//P36=0;}

#define D_AFC_INIT  	  {GPIO_CONFIG_IO_MODE(GPIO3,GPIO_PIN_0,GPIO_MODE_OUTPUT);}//GPIO_ENABLE_OUTPUT(P3TRIS, GPIO_PIN_6);}
#define D_AFC_INPUT  	  {GPIO_CONFIG_IO_MODE(GPIO3,GPIO_PIN_0,GPIO_MODE_INPUT);}//GPIO_ENABLE_INPUT(P3TRIS, GPIO_PIN_6);}

extern uint8_t  RX_Error_Status_Return_TX;
uint8_t  RX_Error_Status_Return_TX=1;
extern uint8_t  normal_operation;
uint8_t  normal_operation=0;
uint8_t  normal_operation_count=0;
int  Led_error_count=0;
void TimerDo_fun(void)
{       
      //      TMR_SetPeriod(TMR0,48000); 
            fgDelay1msFlag = 1;
}




void adcInit(uint8_t ch)
{

}


uint16_t ADC_GetADCResult(uint8_t ch)
{
    uint16_t value;
    value = 0;
    switch(ch)
    {
    case CON_ADC_CH_VPP:
        //测线圈电压
     //   value = ADC1_GetResult(ADC1_CH_8);
        break;
    case CON_ADC_CH_Current:
        //测电流
    //    value = ADC1_GetResult(ADC1_CH_30);
     //    value = ADC1_GetResult(ADC1_CH_7);
        break;
    case CON_ADC_CH_PcbTemperature:
        //测温度
     //   #if  (defined(CFG_TEMPERATURE)&&(CFG_TEMPERATURE==1))
     //    value = ADC1_GetResult(ADC1_CH_16);
       //   value = ADC1_GetResult(ADC1_CH_2);
      //  #endif
        break;
	  case CON_ADC_CH_CoilTemperature:
        //测温度
     //   #if  (defined(CFG_TEMPERATURE)&&(CFG_TEMPERATURE==1))
       // value = ADC1_GetResult(ADC1_CH_14);
   //       value = ADC1_GetResult(ADC1_CH_9);
      //  #endif
        break;  
    case CON_ADC_CH_Vref12:
		//  value = ADC1_GetResult(ADC1_CH_10); 
        break;
    case CON_ADC_CH_VDD:
        //测VDD
        ///value = ADC1_GetResult(ADC1_CH_6);
        break;
    }
    return value;

}

void adcStart(void)
{
    uint8_t i;

    for(i=0; i<3; i++);
    
   // ADC1_Go();
}
void adcWait(void)
{
  //  while(ADC1_IS_BUSY());
}






#ifdef DEBUG_PRINT
void my_itoa(int16_t n,uint8_t flag)
{
    int8_t i,j,minus;
    uint16_t np;
    char s[10];

    minus = 0;
    np = n;

    if(flag==1)
    {
        if(n<0)
        {
            minus = 1;
            np = -n;
        }
    }

    i=0;
    do {
        s[i++]=np%10+'0';
    }
    while ((np/=10)>0);
    if(minus)
    {
        s[i++] = '-';
    }
    s[i]='\0';

    for(j=i-1; j>=0; j--)
        putchar(s[j]);

    putchar(',');
}

void my_htoa(uint8_t n)
{
    int8_t i,j;
    uint16_t np;

  // minus = 0;
    np = n;


    j = np%16;
    i = np/16;
    if(j<10)
    {
        j+='0';
    }
    else
    {
        j+=('A'-10);
    }

    if(i<10)
    {
        i+='0';
    }
    else
    {
        i+=('A'-10);
    }

    puts("0x");

    putchar(i);
    putchar(j);
    putchar(',');

}
#endif





uint8_t bSend28Times=0;

void Fun_SendCode_250us(void)
{
    //TMR_ConfigTimerPeriod(TMR0, 0xFC, 0x17);
   // TMR_ConfigTimerPeriod(TMR0, (65536-1000)>>8, 65536-1000);
  

    if( fgHalf )
    {
      //  GPIO0->DO_f.P5 = ~GPIO0->DO_f.P5;	
        if( fgNextbit )
        {
            if(fsendctlbit)
						{
            PIN_COM_flip;
						}
		    	else
					 {
		        PIN_COM1_flip;
					  }

        }
    }
    else
    {
      //  _nop_();
      //  _nop_();
      //  _nop_();
         fgHalf=fgHalf;

           if(fsendctlbit)
				{
         PIN_COM_flip;
				}
		    else
				{
		      PIN_COM1_flip;
				}
	//	PIN_COM1_flip;

        if ( bPacketArrayIndex == 0 )    //Header
        {
            bPacketBitIndex ++;
			   

            if( D_Header == bPacketBitIndex )
            {
                bPacketBitIndex = 0;
                fgNextbit = 0;
                bPacketArrayIndex =1;
            }
            else
                fgNextbit = 1;
        }
        else
        {
            bPacketBitIndex++;
            if( bPacketBitIndex >= 11)
            {
			     
                if( bPacketArrayIndex == bPacketFinal )
                {
                    fgSendPacketEnd = 1;

                       if(fsendctlbit)
						{
                          PIN_COM_0;
						}
				        else
						{
                          PIN_COM_1;
						}
                  //  PIN_COM_1;
                  /*  TMR_Stop(TMR0);//TR0 = 0;*/
						          		  __disable_irq();
                          NVIC_DisableIRQ(ATIM_IRQn);
                          __enable_irq();
                }
                else
                {

                    bPacketBitIndex = 0;
                    bPacketArrayIndex++;
                }

                fgNextbit = 0;
            }
            else
            {

                if( wx_Packet[bPacketArrayIndex-1] & 1<< bPacketBitIndex )
                    fgNextbit = 1;
                else
                    fgNextbit = 0;
            }
        }

    }
      if(fgHalf)
	    fgHalf=0;
	 else
	    fgHalf=1; 

 //   fgHalf = ~fgHalf;

}

void F_Reset_Packet_Variables() //using 1
{
    bPacketArrayIndex = 0;   //0 Header
    bPacketBitIndex = 0;
    fgHalf = 1;
    fgSendPacketEnd = 0;

    fgNextbit = 1;
	lowcurstbit++;
	if(lowcurstbit>3)
	 	lowcurstbit=0;
        
    // 实现 2:1 发码比例 (发两次P31，发一次P32)
    // lowcurstbit 循环序列: 1 -> 2 -> 0 -> 1 -> 2 -> 0
    // 对应 fsendctlbit 赋值: 1 -> 1 -> 0 -> 1 -> 1 -> 0 (1代表P31，0代表P32)
    if(lowcurstbit == 3)
        fsendctlbit = 1; // 发 P32
    else
        fsendctlbit = 0; // 发 P31
    
   // TMR0_Period_Count_Config();
	   ATIM_Code_Count_Config();

  //  TMR_Start(TMR0);

    //EA = 1;
}

uint16_t F_Count_Byte_Code(uint8_t bByte) //using 1
{
    uint16_t wResult=0x400;
    uint8_t bi,bj=0;
    for(bi = 0 ; bi <=7; bi++)
    {
        if(bByte & 1<<bi)
        {
            bj++;
            wResult |= 1<<( bi+1 );
        }
    }

    if( bj & 0x1 );
    else
        wResult |= 0x200;

    return wResult;
}



uint16_t F_Check_Vrect() //using 1
{
       uint16_t ADtemp;
	   adcStart();

        adcWait();

        ADtemp = ADC_GetADCResult(CON_ADC_CH_VPP);
      return ADtemp;
}


uint16_t F_Check_OutputVolt() //using 1
{
       uint16_t ADtemp;
	   adcStart();

        adcWait();

        ADtemp = ADC_GetADCResult(CON_ADC_CH_Vref12);//         CON_ADC_CH_VDD);
      return ADtemp;
}



uint16_t F_Check_Current() //using 1
{
       uint16_t ADtemp;
	   adcStart();

       adcWait();

       ADtemp = ADC_GetADCResult(CON_ADC_CH_Current);
       return ADtemp;
}




uint16_t F_Check_CoilTemperature() //using 1
{
       uint16_t ADtemp;
	   adcStart();

       adcWait();

       ADtemp = ADC_GetADCResult(CON_ADC_CH_CoilTemperature);
       return ADtemp;
}



uint16_t F_Check_PcbTemperature() //using 1
{
       uint16_t ADtemp;
	   adcStart();

       adcWait();

       ADtemp = ADC_GetADCResult(CON_ADC_CH_PcbTemperature);
       return ADtemp;
}






#define DC_OUT_LOW       1150// 1450//1266//20*1*4095/11/5
#define DC_OUT_HIGH      1300//1340//22*1*4095/11/5



signed char F_Determine_Control_Error() //using 1
{

    signed int iTemp;

    uint16_t dc_out;
   
	
	   if(bCtrlmos)
	     
	       dc_out = ( Volt25_2vfull+16); //1422;//820*9/10-15;
	   else
		   dc_out = ( Volt25_2vfull+17); //1429; //820*21/10+5;
	   		     	
         iTemp = 0;
	            
          iTemp = (signed int)dc_out - wDC_OUT;
		  blowpwcnt=0;
		   		 
           if(iTemp<-210)
		  	  iTemp = iTemp/4;
		   else if(iTemp<-90)
             iTemp = iTemp/3;
		   else if(iTemp<-20)
		     iTemp = iTemp/2;		   		 		 
		   else if(iTemp>0)
		   	{

			  if(bCtrlmos)
			  	{
		   	     iTemp = iTemp/3;
			  	}
			  else
			  	{
			  	      if(bchargelessflg)
				  	     iTemp = iTemp/2;
					  else
					  	iTemp = iTemp/5;
			  	}
		    }		       

	      if( iTemp > D_Control_Error_max )
	      	{

			   
	      	  //  if(wDC_OUT<1170)
	      	      if(wDC_OUT<1095)
	      	      	{
	                    iTemp = 55;
					 

	      	      	}
				else
					{
				       iTemp =D_Control_Error_max; //65;	
					}

	      	}
	      else if(iTemp <= D_Control_Error_min)
	      	{
			   if(bchargelessflg)
	             iTemp = D_Control_Error_min;
			   else
			   	 iTemp = -35;
			   
						
	      	}
		 if(bchargelessflg==0)
		 	{
		 	   if(wCurrent>40)
				  iTemp =-10;
			   else if(wCurrent>25)
			   	 iTemp =-1;
			   else if(wDC_OUT<Volt25_2vfull)
			   	  iTemp =2;
			   else
			   	  iTemp =0;

		 	}
		 if(bCtrlmos)
		  	{
		  	     //if(wCurrent<5) 
                 //  bInitialCounter=0;
				Voltchgfull = (Volt25_2vfull-10);
                 if(bInitialCounter<15)
                 	{
                      bInitialCounter++;
					   bexit25vSt=0;
					  if(bInitialCounter<5)
					  	  iTemp = 0;
			//		  if(wCurrent>110||(wOutVolt>1390))  //3////25.25v
			         if(wCurrent>35||(wOutVolt>(Volt25_2vfull-5))) 
					  	 bInitialCounter=15;
					//  if(iTemp>5)
					  //	iTemp=5;
					  b29vStcnt=0;
                 	}
			#if 1	 
				// else if(b29vSt&&wOutVolt<1355)     //24.8v 
				  else if(b29vSt&&(wOutVolt<(Voltchgfull-18))) 
				 	{
                       if(b29vStcnt>0)
                       	{
					   	   b29vStcnt--;
						   if(b29vStcnt<2)
						   	{
						      b29vSt=0;
                              b29vStcnt=0;
							  bexit25vSt=1;
						   	}
                       	}
					  
					   if(iTemp<0)
					   	{
                           iTemp =-1;
					   	}
					   else
					   	{
					   	   iTemp=0;
					   	}
				 	}
				 else if(b29vSt||((wOutVolt>(Voltchgfull+3)||((wOutVolt>(Voltchgfull-12))&&(wCurrent<145)))&&b29vSt==0))
				 	{
				 	         bexit25vSt=0;
                            b29vStcnt++;
						   if(b29vStcnt>58)
						   	{
						     	if(wOutVolt>(Voltchgfull-15)&&(wCurrent<29))
									{
                                          b29vSt=2;
									}
								else
									{
                                      b29vSt=1;
								     }
				              b29vStcnt=60;
                              // iTemp = 0;    
						   	}
						   else
						   	{
						   	  if(b29vStcnt>25||wCurrent>172||(wOutVolt>(Voltchgfull+10)&&wCurrent>35))
                                  iTemp = 0;

						   	}
						  if(b29vSt)
						  	{
                               b2ACurrentcnt++;  
                  if(wOutVolt<(Voltchgfull-7)&&b2ACurrentcnt>15&&wCurrent<38)    //25.4v
							  	 {
							       iTemp=1;
								     b2ACurrentcnt=0;
							  	}
							    else if(((wDC_OUT<(frontend_factor_wDC_OUT-7))&&wOutVolt>(Voltchgfull-14)&&((wCurrent>75&&b2ACurrentcnt>60)||(wCurrent>5&&b2ACurrentcnt>110&&wCurrent<75)))\
									||(wOutVolt>(Voltchgfull-5)&&b2ACurrentcnt>30)||(wOutVolt>(Voltchgfull+9)&&b2ACurrentcnt>0)||(wOutVolt>(Voltchgfull-1)&&(b2ACurrentcnt>8)))          
							  	{
                               
								      if((wOutVolt>(Voltchgfull+15)&&(wCurrent>36))||wOutVolt>(Voltchgfull+20))
								      	{
								      	  if(wCurrent>16||(wOutVolt>(Voltchgfull+30)))
								      	  	{
                                                iTemp =-35; 
									
								      	  	}
										  else if(wCurrent>4)
								  	      	{
		                                      iTemp =-1; //iTemp/2;
								  	      	}
		                                  else if(wCurrent<3)
		                                  	{
										  	   iTemp=1;
		                                  	}
										  else 
										  	{
	                                           iTemp=0;
	                                         
										  	}
								      	}
							  	      else if(wCurrent>4)
							  	      	{
	                                      iTemp =-1; //iTemp/2;
							  	      	}
	                                  else if(wCurrent<3)
	                                  	{
									  	  iTemp=1;
	                                  	}
									  else 
									  	{
                                           iTemp=0;
                                         
									  	}
	                                  b2ACurrentcnt=0;

		

							  	}
							  else
							  	{
							  	      iTemp = 0; 

								    if(wCurrent>219)
			                          	{
									      iTemp=-1;
										  b2ACurrentcnt=0;

			                          	}
                                 
								 //  if(b2ACurrentcnt>260)
                                  //   b2ACurrentcnt--;  
							  	}
							   
						  	}
				 	}
				#endif 
                
				 else if(wCurrent>370)
						iTemp=-35;
			     else if(wCurrent>358)
						iTemp=-20;
				 else if(wCurrent>333)
						iTemp=-3;
				 #if 0
				 else if((wCurstatusCnt>80)&&(abs(wCurrent-wbackCur)<3)&&(wCurrent>32)&&(wCurrent<160)&&b18v_lowcurrcnt<=1)
				   {
					        iTemp=0;
						//	wbackCur=wCurrent;
							
					  if(bconstant_curcnt<15)
						{
		                  bconstant_curcnt++;	
		                }
		              if(bconstant_curcnt>=15)
						 {
						   if(abs(wOutVolt-wbackupVlt)>325||bconstant_curcnt>=20)
						   {
			                if(wOutVolt<(Volt25_2vfull-265))
											{
			                  iTemp=55;  
			                }	
			               else if(wOutVolt<(Volt25_2vfull-145))
											{
			                  iTemp=35;  
			                }	
			               else if(wOutVolt<(Volt25_2vfull-62))
											{
			                  iTemp=25;  
			                }	
							else if(wOutVolt<(Volt25_2vfull-22))
											{
			                  iTemp=10;  
			                }	
			                else if(wOutVolt<(Volt25_2vfull-11))
											{
			                  iTemp=1;  
			                }	
			                else if(wOutVolt>(Volt25_2vfull-6))
											{
			                  iTemp=-1;  
		                 }
						
					   }
					  else
					  	{
                           iTemp=8; 
						   bconstant_curcnt++;
					  	}
		             }
							 
				   }		
				#endif	

        #if 1
			  else if((wOutVolt<972&&wCurrent<120&&wCurrent>20)||(wOutVolt<985&&wCurrent>220)||b18v_lowcurrcnt>3)    //volt< 18v   0.5a charge
				    	{
                   b2ACurrentcnt++;  
                 bconstant_curcnt = 0;								
						    if(wCurrent>150*2)
								{									
                         iTemp=-45;
								}
							  else if(wCurrent>112*2)
								{									
                        iTemp=-35; 
								}
								else if(wCurrent>95*2)
								{									
                        iTemp=-8; 
								}
								else if(wCurrent>82*2)
								{									
                        iTemp=-3; 
								}
                else if(wCurrent>72*2)
								{													
								    if(b2ACurrentcnt>0)
											{
                         iTemp=-1;
											   b2ACurrentcnt=0;	
											}
										  else
											{
										     iTemp=0;			
											}						
								}									
				    	  else if(wCurrent>70*2)               //0.5a
								 { 
									    if(b2ACurrentcnt>10)
											{
                       iTemp=-1;
											 b2ACurrentcnt=0;	
											}
										  else
											{
										     iTemp=0;			
											}
								 
								 }
						       else if(wCurrent<50*2)
								{
							         iTemp=30;
									      b2ACurrentcnt=0;
								}			
						   	  else if(wCurrent<62*2)
								{
							         iTemp=15;
									      b2ACurrentcnt=0;
								 
								}			
								else if(wCurrent<65*2)
								{
							         iTemp=1;
									      b2ACurrentcnt=0;
								}									
							  else if(wCurrent<67*2)
								{
									    if(b2ACurrentcnt>25)
											{												
							          iTemp=1;
											 b2ACurrentcnt=0;	
											}
											else
											{
										     iTemp=0;			
											}
								}
							 else
							 {
							   iTemp=0;	
								  b2ACurrentcnt=0;
							 }
							 
							  if(wOutVolt<986&&b18v_lowcurrcnt<15)
							 	{
							     b18v_lowcurrcnt++;
							 	}
							 else if(wOutVolt>986&&b18v_lowcurrcnt)
							 	{
                                  b18v_lowcurrcnt--;    

							 	}
							 if(wCurrent<12)
							 	{
                      b18v_lowcurrcnt=0;     
							 	}
		

				    	}
					 #endif
					 
				  else if(wCurrent>320)     //
					 {
                            b2ACurrentcnt++;
                          if(wCurrent>328||b2ACurrentcnt>13)
                          	{
						     iTemp=-1;
							 b2ACurrentcnt=0;

                          	}
						  else
						  	{
						     iTemp=0;
						  	}

					 }		
					
					else if(wCurrent<60&&(wOutVolt<(Volt25_2vfull-25)))//&&iTemp>0)
						{
					       iTemp=55;                       
						}
					else if(wCurrent<180&&(wOutVolt<(Volt25_2vfull-20)))//&&iTemp>0)
						{
                           if(wOutVolt>(4*245+105))						   
					           iTemp=45; 
						   else
						   	   iTemp=35; 
						   if(Current_1Afull<Current_limitpw)
						   	{
						   	   
						   	       iTemp=20; 
							   if(Current_1Afull>259)
							   	  iTemp+=(Current_1Afull-259)/2;  

						   	}
							   bconstant_curcnt=0;
						}
					else if(wCurrent<235&&(wOutVolt<(Volt25_2vfull-10)))//&&iTemp>0)
						{
					       if(wOutVolt>(4*245+135))						   
					           iTemp=35; 
						   else
						   	   iTemp=13; 
							   bconstant_curcnt=0;
						   if(Current_1Afull<Current_limitpw)
						   	{
						   	       iTemp=10; 
                                if(Current_1Afull>259)
							   	  iTemp+=(Current_1Afull-259)/2;  
						   	}
						}
					else if(wCurrent<265&&(wOutVolt<(Volt25_2vfull-4))&&(b29vStcnt==0))//&&iTemp>0)
						{
					      if(wOutVolt>(4*245+198))						   
					           iTemp=25; 
						   else
						   	   iTemp=11; 
						  if(Current_1Afull<Current_limitpw)
						  	{
						   	    iTemp=5; 
                              if(Current_1Afull>259)
							    iTemp+=(Current_1Afull-259)/2;  
						  	}
						   bexit25vSt=0;
							   bconstant_curcnt=0;
						}
					else if(wCurrent<299&&(wOutVolt>(Volt25_2vfull-230))&&(b29vStcnt==0))//&&iTemp>0)
						{
					       iTemp=6;
						  if(Current_1Afull<Current_limitpw)
						   	   iTemp=3; 
						   bexit25vSt=0;
							  bconstant_curcnt=0;
						}
					
					else if(wCurrent<317)                   //1.5a
					{
						    bconstant_curcnt=0;
						   if(b29vStcnt)
					   	     b29vStcnt--;
						     b2ACurrentcnt++;
						#if 0	 
						  if(wDC_OUT<1490)
						  	{
                               iTemp=iTemp;   

						  	}
						   	 
						 else if(wDC_OUT>1517&&b2ACurrentcnt>5||wDC_OUT>1539)
						  	{
						  	  if(wDC_OUT<1535) 
                                 iTemp=-1;
							   b2ACurrentcnt=0;
						  	}											    
						 else 
						 #endif
                            
						 
						if(b2ACurrentcnt>13||wCurrent<310)
						  	{
						      iTemp=1;
							  b2ACurrentcnt=0;
						  	}
						  else 
						  	{
                              iTemp=0;

						  	}						  	
					}
				   else
					 {
					     //if(wDC_OUT>1540)  
						 	//iTemp=-1;
						 //else
                             iTemp=0;
						  if(b29vStcnt)
					   	    b29vStcnt--; 
					 }
						

		  	}
		 else
		 	{
		 	     bInitialCounter++;  
               if(bInitialCounter<4)
			   	 iTemp=0;

		 	}
     #if 0
		   if(abs(wCurrent-wbackCur)<3&&wCurstatusCnt<90)    //const current mode
		   	{
		   	   if(wOutVolt>1015||bconstvlt_cnt>7)
		   	    wCurstatusCnt++;
			   else if(wOutVolt<990)
			   	 wCurstatusCnt=0;
		   	}
		   else if(abs(wCurrent-wbackCur)>8&&wCurstatusCnt)
		   	{
               wCurstatusCnt--;
			   bconstvlt_cnt=0;
		   	}
				
				#endif
		
       	 	wbackCur = wCurrent ; 
		// if( bPhasePacketIndex == D_QI_RectifiedPower)	
		 //     wbackupVlt=wOutVolt; 
 	 	  	
	  	
	// if(wCurrent>605)//||wDC_OUT>(820*22/10+10))   //||(wOutVolt>(820*25/10+50)))//&&ctlmostm>100)       //3.1a
	  // if(wCurrent>496)                      //2.7a
	   if(bNowTempValue>79||bNowTempValueBackup)
	   	 {
	   	   if(wCurrent>126)
		     iTemp=-1;
		   else if(iTemp>0)
		   	 iTemp=0;
		     bNowTempValueBackup=1;
		   if(bNowTempValue<75)
              bNowTempValueBackup=0;   
	   	 }
	   //  if(wDC_OUT>(Volt25_2vfull+41)&&wCurrent>10)
//	    if(wDC_OUT>(frontend_factor_wDC_OUT+31)&&(wCurrent>10))	
      //   if(wDC_OUT>(frontend_factor_wDC_OUT+1))	
			//  iTemp =-43;
	
	 //  if((wDC_OUT>2190)&&ctlmostm>660&&bCtrlmos&&bchargelessflg)                      //2.3a zheng delete over volt protecting
		//	 iTemp=-55;
		#if 0 
		 if(wCurrent>570)                            //5.5a
		 {
			 over_wCurrent++;
			 if(over_wCurrent>=3)
			 {
				 iTemp=-55;
			 }
		 }
		 else{over_wCurrent=0;}
    #endif 
	
	   	
    return iTemp;
}






extern uint8_t  testvariable;


void F_Phase_Packet_Control(void)
{
    uint8_t  bTemp,bTemp1,bTemp2;
     uint8_t  bpower_temp;
	  uint16_t  volt_temp;
    switch(bPhase)
    {
    case PING:
			normal_operation=1;
	    bCtrlmos=0;
		 blowpwcnt=0;
		    blowcurSt=0;
        wT1_1ms_Counter++;
		    
        if( wT1_1ms_Counter >= 10 )
        {

            // UART_SendString(DEBUG_UARTx, "\r\n ping\r\n");
              wx_Packet[0] = F_Count_Byte_Code(0x01);
            
             wx_Packet[1] = F_Count_Byte_Code(0x80);        // Count Byte by function.
            
             wx_Packet[2] = F_Count_Byte_Code(0x81);      

            bPacketFinal = 3;   //the last byte
            F_Reset_Packet_Variables(); //Start to Send preamble first

            //clear 1ms counter , Change Phase here
            wT1_1ms_Counter = D_bOverFlowCode- D_QI_tsilent;
            bPhase = ID_CONF;
            bPhasePacketIndex = D_QI_Identification;
            //bPhasePacketIndex = D_QI_Configuration;//D_QI_Identification;
            bPhasePacketIndexBefore = D_QI_SignalStrength;


;

            bInitialCounter = 0;
        }
        break;
    case ID_CONF:
			    
        if(fgSendPacketEnd)
        {
            wT1_1ms_Counter++;
			    //  UART_SendString(DEBUG_UARTx, "\r\n config\r\n");
            if( wT1_1ms_Counter >= 50) //D_bOverFlowCode)
            {


			     	                         wx_Packet[0] =  F_Count_Byte_Code(D_QI_Proprietary_0xF2);   //D_11bit_ControlError;  
                             if(bCtrlmos)
                             	{
                                   bIdentificationPacket_10W[1] = wOutVolt/256; //wDC_OUT/256; //wOutVolt/256;
						           bIdentificationPacket_10W[2] = wOutVolt%256;  //wDC_OUT/256; //wOutVolt%256;
                             	}
							  else
							  	{
						         bIdentificationPacket_10W[1] = wDC_OUT/256; //wDC_OUT/256; //wOutVolt/256;
					             bIdentificationPacket_10W[2] = wDC_OUT%256;  //wDC_OUT/256; //wOutVolt%256;
							  	}
	              
	                        wx_Packet[1] = F_Count_Byte_Code(bIdentificationPacket_10W[1]);        // Count Byte by function.
	                        wx_Packet[2] = F_Count_Byte_Code(bIdentificationPacket_10W[2]);  
	                  
	                    bTemp = bIdentificationPacket_10W[1] ^ D_QI_Proprietary_0xF2 ^ bIdentificationPacket_10W[2];
					    
						wx_Packet[3] = F_Count_Byte_Code(bTemp);		// Check sum.
						
						 bPacketFinal = 4;	 //the last byte
						 //if(bControlError!=0)
					    F_Reset_Packet_Variables();
						                  bPhase = POWERTRANSFER;
                    bPhasePacketIndex = D_QI_ControlError;  //D_QI_ControlError;
                    bPhasePacketIndexBefore = D_QI_Configuration;
                    bPowerTransferPacketIndex = 0;

                    bSend28Times = 0;

													fmc_read_bytes(bvltcurr_buf,11);
			#if 1
							//Volt25_2vfull  = (6*256+96-41);   
					    Volt25_2vfull  =  1349; //1335;     //1347; //1368;  //1584;
				      Current_1Afull=300;					  
					  frontend_factor_wDC_OUT  = 1415;		  //26.2v
			
				  if(bvltcurr_buf[0]==0xA5)
						
				  	{
							
							calibration_factor_wCurrent=bvltcurr_buf[7];
							calibration_factor_wOutVolt=bvltcurr_buf[8]*256+bvltcurr_buf[9];
							
							// UART_SendString(DEBUG_UARTx, "\r\n adjust factor\r\n");
						  // 	UART1_Send(bvltcurr_buf, 11);	
						//	Send_Data_TEST();              
				    }
						else
						{
							calibration_factor_wCurrent=185;
							calibration_factor_wOutVolt=2048;
						
							
             //  	UART_SendString(DEBUG_UARTx, "\r\n no factor\r\n");
  
	             
						}
				//  	UART1_Send(bvltcurr_buf, 11);	
				  //else
				#endif
//				  	{
//				      Volt25_2vfull  = (6*256+96-41);
//				      Current_1Afull=150;
//				  	}
//				  my_itoa(bvltcurr_buf[3],0);
//				  my_itoa(bvltcurr_buf[4],0);
//				  my_itoa(bvltcurr_buf[5],0);
//				  my_itoa(bvltcurr_buf[6],0);
//				  puts("wy02 ID_CONF!\n\r");
               
			  
             #if 0
						 
                if( D_QI_Identification == bPhasePacketIndex ) //First Packet at ID_CONF Phase , ID Phase
                {

//                    //Copy to Packet array
//                    for( bTemp= 0; bTemp<9;bTemp++)
//                        wx_Packet[bTemp] = wIdentificationPacket[bTemp];

                    for( bTemp= 0; bTemp<8; bTemp++)
                    {
                        if( 0 == bTemp )
                            bTemp1 =  bIdentificationPacket_10W[bTemp];
                        else
                            bTemp1 = bTemp1 ^ bIdentificationPacket_10W[bTemp];
                        wx_Packet[bTemp] = F_Count_Byte_Code(bIdentificationPacket_10W[bTemp]) ;
                    }
                    wx_Packet[8] = F_Count_Byte_Code(bTemp1) ;

                    bPacketFinal = 9;   //the last byte
                    F_Reset_Packet_Variables(); //Start to Send preamble first

                    //Reset silent time
                    wT1_1ms_Counter = D_bOverFlowCode- D_QI_tsilent;
                    bPhasePacketIndex =D_QI_Configuration;
                    bPhasePacketIndexBefore =D_QI_Identification;


                }
                else if( D_QI_Configuration == bPhasePacketIndex) //Second Packet at ID_CONF Phase , Configuration Phase
                {
                    wZeroCurrent =0;// wCurrent;


                 

                    for( bTemp= 0; bTemp<6; bTemp++)
                    {
                        if( 0 == bTemp )
                        {
                            wx_Packet[bTemp] = F_Count_Byte_Code(0x51) ;
                            bTemp1 = 0x51;
                        }
                        else
                        {
                            bTemp2 =  bConfigurationPacket_10W[bTemp];
                            bTemp1 = bTemp1 ^ bTemp2;
                            wx_Packet[bTemp] = F_Count_Byte_Code(bTemp2) ;
                        }
                    }



                    wx_Packet[6] = F_Count_Byte_Code(bTemp1) ;


                    bPacketFinal = 7;   //the last byte
                    F_Reset_Packet_Variables(); //Start to Send preamble first


                    //clear 1ms counter , Change Phase here
                    bControlError = 0x00;//F_Determine_Control_Error(); not action at first packet

                    wT1_1ms_Counter = 0;//D_QI_tsilent;
                    bPhase = POWERTRANSFER;
                    bPhasePacketIndex = D_QI_ControlError;  //D_QI_ControlError;
                    bPhasePacketIndexBefore = D_QI_Configuration;
                    bPowerTransferPacketIndex = 0;

                    bSend28Times = 0;
										fmc_read_bytes(bvltcurr_buf,11);
			#if 1
							//Volt25_2vfull  = (6*256+96-41);   
					    Volt25_2vfull  =  1349; //1335;     //1347; //1368;  //1584;
				      Current_1Afull=300;					  
					  frontend_factor_wDC_OUT  = 1415;		  //26.2v
			
				  if(bvltcurr_buf[0]==0xA5)
						
				  	{
							
							calibration_factor_wCurrent=bvltcurr_buf[7];
							calibration_factor_wOutVolt=bvltcurr_buf[8]*256+bvltcurr_buf[9];
							
							// UART_SendString(DEBUG_UARTx, "\r\n adjust factor\r\n");
						  // 	UART1_Send(bvltcurr_buf, 11);	
						//	Send_Data_TEST();              
				    }
						else
						{
							calibration_factor_wCurrent=185;
							calibration_factor_wOutVolt=2048;
						
							
             //  	UART_SendString(DEBUG_UARTx, "\r\n no factor\r\n");
  
	             
						}
				//  	UART1_Send(bvltcurr_buf, 11);	
				  //else
				#endif
//				  	{
//				      Volt25_2vfull  = (6*256+96-41);
//				      Current_1Afull=150;
//				  	}
//				  my_itoa(bvltcurr_buf[3],0);
//				  my_itoa(bvltcurr_buf[4],0);
//				  my_itoa(bvltcurr_buf[5],0);
//				  my_itoa(bvltcurr_buf[6],0);
//				  puts("wy02 ID_CONF!\n\r");
                }
			  #endif
            }
        }
		  
	      Current_limitpw=275;
		   wT1_1ms_Counter = D_bOverFlowCode- D_QI_tsilent;
        break;
    case POWERTRANSFER:
      
        if(fgSendPacketEnd)
        {
            wT1_1ms_Counter++;
			 
				//Uart0revProc();
			if(wT1_1ms_Counter%2==0)
				{
          
					 wDC_OUT +=   gAdcResult[1];  //F_Check_Vrect();//
				     wDC_OUT /= 2;
						
                    
					wCurrent+=gAdcResult[3]*30/100*300/Current_1Afull*calibration_factor_wCurrent/128;   //72
					wCurrent /= 2;
				//	wCurrent *= 2;

                    gAdcResult[0]/=1.876f;
					volt_temp = gAdcResult[0]*calibration_factor_wOutVolt/2048;
                    wOutVolt = (4*wOutVolt+volt_temp)/5;

					 wCoilTemperature+=   gAdcResult[2];  //F_Check_CoilTemperature();					
				     wCoilTemperature /= 2;

					//wPcbTemperature+=F_Check_PcbTemperature();					
						//	wPcbTemperature /= 2;


						if(wCoilTemperature>3425) 
           		     
						 	{
						       bNowTempValue=25;
						 	}
						 else
						 	{
								 bNowTempValue= (3425-wCoilTemperature)/30;  
								 bNowTempValue+=25;
							    if(bNowTempValue>65)
							  	  {

								  if(bNowTempValue>72)
                                    bNowTempValue*=100/93;
								  else
								    bNowTempValue*=100/96;
							   

							  	  }
						 	}
							
				     if(wCurrent> 345)  //3.1a   //369)  //&&bCtrlmos)  // current     350/300*312    3.5a
					    {	
               							
								Error_Status_count++;
							if((Error_Status_count>=2500||(wCurrent>410&&Error_Status_count>500)||(wCurrent>369&&Error_Status_count>1500))&&bCtrlmos)    ///3s     //zheng delete over wCurrent>510 function
							   {							
								  RX_Error_Status_Return_TX=4;
								  Error_Status=0x01;																								
							      PB05_SETLOW();	     
								  bchargelessflg=0;	
                                  ctlmostm	=0;								
								  bCtrlmos=0;						
								}
								
							}		
				    else if(wDC_OUT>1245)                 //zheng front volt >20v 
					 {
						 if(ctlmostm<2500)
						    ctlmostm++;
                         if((ctlmostm==150&&(Error_Status==0))||(ctlmostm==1000&&Error_Status))           //400ms	
							 {

							   if(RX_Error_Status_Return_TX==3&&(bNowTempValue>75))   //over temperature  75
							   	{
                                  ctlmostm=990;      
							   	}
							   else
							   	{
					              PB05_SETHIGH();                //open mos 
								  bCtrlmos=1;
								  RX_Error_Status_Return_TX=1;
								  bchargelessflg = 1;
							   	}

							    Error_Status_count=0;
							 }

						   
						  
					  }
	                 
					  
            #if 1
				  		if((wDC_OUT>2050*14/10)&&bCtrlmos)//接收到的电压（AD采样检测）
							{
								Error_Status_count++;
								if(Error_Status_count>=50)
								{
									RX_Error_Status_Return_TX=2;
									Error_Status=0x02;
																 								
									  bchargelessflg=0;	
									 PB05_SETLOW();	     
								  bchargelessflg=0;	
                                  ctlmostm	=0;								
								  bCtrlmos=0;	
									
								}

								
							}
							else if(bNowTempValue>=85&&bCtrlmos)//应该是大于75度  85
							{
								Error_Status_count++;
								if(Error_Status_count>=50)
								{
								    RX_Error_Status_Return_TX=3;
									Error_Status=0x04;																									  
								    bchargelessflg=0;
								  PB05_SETLOW();	     
								  bchargelessflg=0;	
                                  ctlmostm	=0;								
								  bCtrlmos=0;	
																		
								}

							}
	
							#if 1
							else if(wCurrent>100&&wOutVolt<=270&&bCtrlmos)//短路
							{
							      Error_Status_count++;
								if(Error_Status_count>=20)
									{
								RX_Error_Status_Return_TX=6;
						
								Error_Status=0x10;
															
								bchargelessflg=0;
								PB05_SETLOW();	     
								  bchargelessflg=0;	
                                  ctlmostm	=0;								
								  bCtrlmos=0;	
							
									}
								
							
							}
							#endif
							else if(wOutVolt>1550*14/10&&bCtrlmos)//输出过压
							{
								
								Error_Status_count++;
								if(Error_Status_count>=30||(wOutVolt>1750*14/10))
								{
									Led_error=1;
								   RX_Error_Status_Return_TX=7;
																						
								  bchargelessflg=0;
								  PB05_SETLOW();	     
								  bchargelessflg=0;	
                                  ctlmostm	=0;								
								  bCtrlmos=0;	
																									 
								}

							}

							else if(bchargelessflg&&(wCurrent<340))
							{
								//Led_error_count=0;
								Error_Status_count=0;
								Error_Status=0x00;
								RX_Error_Status_Return_TX=1;
							}
					 #endif
							
					
				}


              if( wT1_1ms_Counter >= D_bOverFlowCode )
                 {
            //    if( D_QI_ControlError == bPhasePacketIndex &&bchargelessflg)
                
							  if(fsendctlbit==1)
								{
								   fsendctlbit=0;
								}
              else  if(fsendctlbit==0)	
							  {
                   fsendctlbit=1;
							  }								
									
               // fsendctlbit=~fsendctlbit;
                   
               if( D_QI_ControlError == bPhasePacketIndex)
                {
                   // bControlError = F_Determine_Control_Error();
									
					
                     if((bPowerTransferPacketIndex==3||bPowerTransferPacketIndex==1||bPowerTransferPacketIndex==5)&&bCtrlmos)//||(bControlError<25&&bPowerTransferPacketIndex==1))
                     	{
                               bpower_temp=  wCurrent*93/100*wOutVolt*185/10000%100*Current_1Afull/300;
						
                           
                               bControlError=  wCurrent*93/100*wOutVolt*185/10000/100*Current_1Afull/300; 
						      if(bpower_temp>50)
							 	bControlError|=0x80;
                                wx_Packet[0] = D_11bit_SignalStrength;
						         wx_Packet[1] = F_Count_Byte_Code(bControlError);        // Count Byte by function.
						         bTemp = bControlError ^ D_QI_SignalStrength;
														if(fsendctlbit==1)
														{
															 fsendctlbit=0;
														}
													   else  if(fsendctlbit==0)	
														{
															 fsendctlbit=1;
														}
								 
								 wx_Packet[2] = F_Count_Byte_Code(bTemp);		 // Check sum.
								 
								  bPacketFinal = 3;   //the last byte
								  //if(bControlError!=0)
									 F_Reset_Packet_Variables();
                 	       }
					     else
							{
							 
								 bControlError = F_Determine_Control_Error();
								 
								if(bControlError==0&&(b29vStcnt>8))   //8
									{
										 if(b29vSt==1)
												bControlError|=0x40;
										 else if(b29vSt==2)
												bControlError|=0x60;
										 
									}

	                   
	                         wx_Packet[0] =  F_Count_Byte_Code(D_QI_Proprietary_0xF2);   //D_11bit_ControlError;  
                             if(bCtrlmos)
                             	{
                                   bIdentificationPacket_10W[1] = wOutVolt/256;        //wDC_OUT/256; //wOutVolt/256;
						           bIdentificationPacket_10W[2] = wOutVolt%256;         //wDC_OUT/256; //wOutVolt%256;
						            if(fsendctlbit)
                                   {
								     bIdentificationPacket_10W[1] =   wCurrent*Current_1Afull/300/256|0x80;   // wCurrent/256;
					                 bIdentificationPacket_10W[2] =   wCurrent*Current_1Afull/300%256;        // wCurrent%256;
                                   }
                             	}
							  else
							  	{
							  	  
						          bIdentificationPacket_10W[1] = wDC_OUT/256;             //wDC_OUT/256; //wOutVolt/256;
					              bIdentificationPacket_10W[2] = wDC_OUT%256;                //wDC_OUT/256; //wOutVolt%256;
                           
							  	}
                                bIdentificationPacket_10W[3] = bNowTempValue; 
							    
	              
	                            wx_Packet[1] = F_Count_Byte_Code(bIdentificationPacket_10W[1]);        // Count Byte by function.
	                            wx_Packet[2] = F_Count_Byte_Code(bIdentificationPacket_10W[2]); 
								wx_Packet[3] = F_Count_Byte_Code(bIdentificationPacket_10W[3]);
	                  
	                        bTemp = bIdentificationPacket_10W[1] ^ D_QI_Proprietary_0xF2 ^ bIdentificationPacket_10W[2] ^ bIdentificationPacket_10W[3];
					    
						    wx_Packet[4] = F_Count_Byte_Code(bTemp);		// Check sum.
						
						     bPacketFinal = 5;	 //the last byte
						 //if(bControlError!=0)
					    F_Reset_Packet_Variables();
					
					 }
					  
					  
 
                        if( bPowerTransferPacketIndex >= 7)//||(RX_Error_Status_Return_TX>1))//||wCurrent>308)//5)   //12  4
                        {
                            //Clear 1ms Counter
                       
                              wT1_1ms_Counter = 130;//D_bOverFlowCode - D_Save_Control_error_Delay_Long;

							 if(ctlmostm<1000)	
                                wT1_1ms_Counter = 150;
							
                            bPhasePacketIndex = D_QI_RectifiedPower;
                            bPhasePacketIndexBefore = D_QI_ControlError;
                            bPowerTransferPacketIndex = 0;
							// fgPrint = 1;

                        }
                        else
                        {                          
                                wT1_1ms_Counter = 160; //D_bOverFlowCode - D_Save_Control_error_Delay_Long;
                                  bPhasePacketIndex = D_QI_ControlError;
                            
							 	
                         
                            bPhasePacketIndexBefore = D_QI_ControlError;
                            bPowerTransferPacketIndex++;
                        }      
         				         
					
                }
				
                else if( D_QI_RectifiedPower == bPhasePacketIndex )
                {

                       
				     if(bchargelessflg)
				     	{
				   	        bIdentificationPacket_10W[1] = wOutVolt/256; //wDC_OUT/256; //wOutVolt/256;
						    bIdentificationPacket_10W[2] = wOutVolt%256;  //wDC_OUT/256; //wOutVolt%256;
				     	}
					 else
					 	{
                           bIdentificationPacket_10W[1] = wDC_OUT/256; //wDC_OUT/256; //wOutVolt/256;
					       bIdentificationPacket_10W[2] = wDC_OUT%256;  //wDC_OUT/256; //wOutVolt%256;

					 	}
					 // if(Current_1Afull==300)
					 	//{
					    //  bIdentificationPacket_10W[3] =   wCurrent/256;            
					     // bIdentificationPacket_10W[4] =   wCurrent%256;
					 //	}
					 //else
					 	//{

					      bIdentificationPacket_10W[3] =   wCurrent*Current_1Afull/300/256;  // wCurrent/256;
					      bIdentificationPacket_10W[4] =   wCurrent*Current_1Afull/300%256; // wCurrent%256;
					 	//}
					  
					  bIdentificationPacket_10W[5] =  b29vSt;  // wCoilTemperature/256; //b29vSt;
					  bIdentificationPacket_10W[6] = 1; //RX_Error_Status_Return_TX;// wCoilTemperature%256; //bNowTempValue; //blowcurSt;
					  

				   for( bTemp= 0; bTemp<8; bTemp++)
				     {
						
					   if( 0 == bTemp )
						   bTemp1 =  bIdentificationPacket_10W[bTemp];
					   else
						   bTemp1 = bTemp1 ^ bIdentificationPacket_10W[bTemp];
					   wx_Packet[bTemp] = F_Count_Byte_Code(bIdentificationPacket_10W[bTemp]) ;
				       }
				   
				   wx_Packet[8] = F_Count_Byte_Code(bTemp1) ;
				
				   bPacketFinal = 9;   //the last byte
                     					  if(fsendctlbit==1)
																{
																	 fsendctlbit=0;
																}
															else  if(fsendctlbit==0)	
																{
																	 fsendctlbit=1;
																}								

             
                    F_Reset_Packet_Variables();

          
                        wT1_1ms_Counter = 120;//D_bOverFlowCode - D_Save_Control_error_Delay_Long;

                        bPhasePacketIndexBefore = bPhasePacketIndex;
					//	if(RX_Error_Status_Return_TX<2)
                        bPhasePacketIndex = D_QI_ControlError;
						UART_TX_DATA_test[0]=wOutVolt/256;
						UART_TX_DATA_test[1]= wOutVolt%256; 

						UART_TX_DATA_test[2]=wbackupVlt/256;
						UART_TX_DATA_test[3]= wbackupVlt%256; 
                        UART_TX_DATA_test[4]=  bconstvlt_cnt;
           #if 0
						if(abs(wOutVolt-wbackupVlt)>3&&b18v_lowcurrcnt>3)
								{

								  // b18v_lowcurrcnt--;
								   bconstvlt_cnt++;
								  if(b18v_lowcurrcnt>8)
								  	{
								  	  b18v_lowcurrcnt=0;
									  wCurstatusCnt=83;

								  	}
                                       
								} 	
              #endif								
						
						//UART1_Send(UART_TX_DATA_test, 5);
						 wbackupVlt=wOutVolt; 
                    //   printf("wbackupVlt %d, %d",wOutVolt,wbackupVlt);
				     //  printf("\n\r");
						
                    
                }


            }
			
        }
        break;
    default:
        break;
    }
}







void Fun_RxPhaseControl_1ms(void)
{


    

    wDelayTimeCnt++;

    if(wDelayTimeCnt>=6000)   //6 second
    {
        wDelayTimeCnt = 0;
        bDelay1sFlag |= 0x01;
			  wChglessTime++;
			  if( b29vSt==2)
				  wChglessTime=0;	
			  
    }
    wDelay10msTimeCnt++;

    if(wDelay10msTimeCnt>=10)
    {
        wDelay10msTimeCnt = 0;
        
    }
}





