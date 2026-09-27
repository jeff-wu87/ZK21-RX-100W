#include "config.h"
#include "constant.h"
#include "variable_extern.h"
//#include "SFR_Macro.h"
//#include "Function_Define.h"

uint8_t  bTempScalarFactor;
int16_t   iMDPRDNegMax,iMDPRDPosMax;
extern uint8_t b15WMOD;



extern uint16_t Get_PWM_Frequency_Value(void);
extern void PWM_SetFreq(unsigned int freq);
extern void PWM_SetFreq_(unsigned int freq);
extern void PWM_SetDami(unsigned int dami0,unsigned int dami1);
extern void PWM_Duty_Set(uint16_t Value);

void F_QI_PowerTransfer_Control_Status_apple7_5()
{
//    unsigned int value,dami0,dami2;

//    value = (((uint16_t)PWMD0H)<<8) + PWMD0L ;
//    dami0 = 0xffff-value;
//    //dami0 = D_QI_AI_Sv_apple_Code-dami0;

//    //value = (((uint16_t)CCAP2H)<<8) + CCAP2L ;
//    value = (((uint16_t)PWMD1H)<<8) + PWMD0L ;
//    dami2 = 0xffff-value;
//    //dami2 = D_QI_AI_Sv_apple_Code-dami2;

    //bCRCLbackup = (((uint16_t)PWMD0H)<<8) + PWMD0L   +  (uint16_t)(D_QI_AI_Sv_apple_Code) - (((uint16_t)PWMD1H)<<8) -PWMD1L ;

    //xxxxxxxxxxxxxxxxxxxxxxxx
    //bCRCLbackup = (((uint16_t)PWMD0H)<<8) + PWMD0L   +  (((uint16_t)PWMD1H)<<8) + PWMD1L ;
    //xxxxxxxxxxxxxxxxxxxxxxxx
    //bCRCLbackup = dami0   +  (uint16_t)(D_QI_AI_Sv_apple_Code) - dami2 ;
    if(i_QI_Error<=-6)
    {
        bCRCLbackup-=4;
    }
    else if(i_QI_Error<0)
    {
        bCRCLbackup--;
    }

    if(i_QI_Error>=6)
    {
        bCRCLbackup+=4;
    }
    else if(i_QI_Error>0)
    {
        bCRCLbackup++;
    }
    if(bCRCLbackup>=D_QI_AI_Sv_apple_Code) //PWM frequency minimum limit
        bCRCLbackup=D_QI_AI_Sv_apple_Code;
    if(bCRCLbackup<=D_QI_AI_Sv_apple_Code/10) //PWM frequency maxmum limit
        bCRCLbackup=D_QI_AI_Sv_apple_Code/10;

    if(bCRCLbackup<=D_QI_AI_Sv_apple_Code/2)
    {
        //PWM_SetDami(D_QI_AI_Sv_apple_Code-bCRCLbackup,0);
//        D_PWM_OpenHalf;
        PWM_SetDami(bCRCLbackup,0);
        //D_PWM_OpenHalf;
        //CCAPM0 &= ~PWM0;//??
        //CCAPM1 &= ~PWM1;//??

//    CCAPM2 &= ~PWM2;//??
//    CCAPM3 &= ~PWM3;//??

//  				  while(LOAD);
//        	  PWM0H = 0;
//            PWM0L = 0;
//  					while(LOAD);
//  				  PWM2H = (uint8_t)((D_QI_AI_Sv_apple_Code-bCRCLbackup) >> 8);
//        	  PWM2L = (uint8_t)(D_QI_AI_Sv_apple_Code-bCRCLbackup);
//            set_LOAD;
//
//  				  while(LOAD);
//						set_PMEN0;
//            set_PMEN1;
//            clr_PMD0;
//            set_PMD1;

    }
    else//if(bCRCLbackup>D_QI_AI_Sv_apple_Code/2)
    {
//        D_PWM_OpenAll;
        PWM_SetDami(D_QI_AI_Sv_apple_Code/2,bCRCLbackup-D_QI_AI_Sv_apple_Code/2);
        //D_PWM_OpenAll;
        //CCAPM0 |= PWM0;//??
        //CCAPM1 |= PWM1;//??
        // CCAPM2 |= PWM2;//??
        // CCAPM3 |= PWM3;//??

//  					while(LOAD);
//  					PWM0H = (uint8_t)((bCRCLbackup-D_QI_AI_Sv_apple_Code/2) >> 8);
//  					PWM0L = (uint8_t)(bCRCLbackup-D_QI_AI_Sv_apple_Code/2);
//  					while(LOAD);
//  					PWM2H = (uint8_t)((D_QI_AI_Sv_apple_Code/2) >> 8);
//  					PWM2L = (uint8_t)(D_QI_AI_Sv_apple_Code/2);
//  					set_LOAD;
//
//					  while(LOAD);
//						clr_PMEN0;
//            clr_PMEN1;
    }
    //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
    //PWMLOADEN |= (EPWM_CH_0_MSK+EPWM_CH_1_MSK);
    //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//    PWMCNTCLR = 0x03;

    fgEndThisControl=1;

}



//void F_QI_PowerTransfer_Control_Status_Full_Bridge()
//{
////    unsigned int value,dami0;
//    uint16_t bDamibackup=0;

//    bCRCLbackup = Get_PWM_Frequency_Value();//(((uint16_t)PWMPH)<<8) + PWMPL;
//    if(i_QI_Error<=-50)
//    {
//        bCRCLbackup-=3;
//    }
//    else if(i_QI_Error<=-18)
//    {
//        bCRCLbackup-=2;//3;
//    }
//    else if(i_QI_Error<=-2)
//    {
//        bCRCLbackup-=1;//3;
//    }
//    else if(i_QI_Error<0)
//    {
//        bCRCLbackup--;
//    }
//    if(i_QI_Error>=50)
//    {
//        bCRCLbackup+=3;
//    }
//    else if(i_QI_Error>=18)
//    {
//        bCRCLbackup+=2;//3;
//    }
//    else if(i_QI_Error>=2)
//    {
//        bCRCLbackup+=1;//3;
//    }
//    else if(i_QI_Error>0)
//    {
////        value = (((uint16_t)CCAP0H)<<8) + CCAP0L ;
////        dami0 = 0xffff-value;

//        bDamibackup = PWM_Duty_Get(0);

//        if(bDamibackup<bCRCLbackup/2)
//        {
//            bDamibackup++;

//            PWM_SetFreq_(bCRCLbackup);
//            PWM_Duty_Set(bDamibackup);

//            PWMLOADEN |= (EPWM_CH_0_MSK+EPWM_CH_1_MSK);
////            PWMCNTCLR = 0x03;
//            i_QI_Error=0;
//        }
//        else
//        {
//            bCRCLbackup++;
//        }
//    }
//    if(bCRCLbackup>=D_QI_AI_Sv_Lv0_Code) //PWM frequency minimum limit
//        bCRCLbackup=D_QI_AI_Sv_Lv0_Code;

//    if(bCRCLbackup<=D_QI_AI_Sv_Lv4_Code) //PWM frequency maxmum limit
//    {
//        bCRCLbackup=D_QI_AI_Sv_Lv4_Code;

////        value = (((uint16_t)CCAP0H)<<8) + CCAP0L ;
////        dami0 = 0xffff-value;


//        bDamibackup = PWM_Duty_Get(0);

//        if(i_QI_Error<=-50)
//        {
//            bDamibackup -= 9;
//        }
//        else if(i_QI_Error<=-6)
//        {
//            bDamibackup -= 3;
//        }
//        else if(i_QI_Error<0)
//        {
//            bDamibackup--;
//        }
//        if(i_QI_Error>=50)
//        {
//            bDamibackup += 9;
//        }
//        else if(i_QI_Error>=6)
//        {
//            bDamibackup += 3;
//        }
//        else if(i_QI_Error>0)
//        {
//            bDamibackup++;
//        }
////					if(i_QI_Error<0)
////					{
////							bDamibackup = dami0-10;
////					}
////					else
////					{
////							bDamibackup = dami0+10;
////					}
////        if(bDamibackup<bCRCLbackup/20)
////        {
////            bDamibackup = bCRCLbackup/20;
////        }
//        if(bDamibackup<bCRCLbackup/5)
//        {
//            bDamibackup = bCRCLbackup/5;
//        }
//        if(bDamibackup>bCRCLbackup/2)
//        {
//            bDamibackup = bCRCLbackup/2;
//        }


//        PWM_SetFreq_(bCRCLbackup);
//        PWM_Duty_Set(bDamibackup);
//        PWMLOADEN |= (EPWM_CH_0_MSK+EPWM_CH_1_MSK);
////        PWMCNTCLR = 0x03;
//    }
//    else
//    {
//        if(bSamsungIsFull==1)
//        {
//            D_PWM_OpenHalf;
//        }

//        PWM_SetFreq(bCRCLbackup);
//        PWMLOADEN |= (EPWM_CH_0_MSK+EPWM_CH_1_MSK);
////        PWMCNTCLR = 0x03;
//    }
//    fgEndThisControl=1;
//}

void F_QI_PowerTransfer_Control_Status_Full_Bridge()
{
//    unsigned int value,dami0;
    uint16_t bDamibackup=0;

    bCRCLbackup = Get_PWM_Frequency_Value();//(((uint16_t)PWMPH)<<8) + PWMPL;

	#if 0

	if(bManufactor==D_Manufactor_XiaoMi)
	{
	       if(i_QI_Error<0)
	       {
	       
		        bDamibackup = PWM_Duty_Get(0);

		        if(bDamibackup>bCRCLbackup/4)
		        {
		            bDamibackup--;
		        }
		        else
		        {
		            bCRCLbackup--;
		        }
	       
	       }
		  else
		  	{
               
			       bDamibackup = PWM_Duty_Get(0);
				   bDamibackup+=i_QI_Error;
			   
					 if(bDamibackup<bCRCLbackup/2)
					 {
						 bDamibackup++;
					 }
					 else
					 {
						 bCRCLbackup++;
						 bDamibackup-=5;
					 }
			   
					 PWM_SetFreq_(bCRCLbackup);
					 PWM_Duty_Set(bDamibackup);


		  	}
		    PWM_SetFreq_(bCRCLbackup);
	        PWM_Duty_Set(bDamibackup);

	        i_QI_Error=0;
	        fgEndThisControl=1;
	        return;

	}

	#endif
	
    if(i_QI_Error<=-50)
    {
        bCRCLbackup-=3;
    }
    else if(i_QI_Error<=-18)
    {
        bCRCLbackup-=2;//3;
    }
    else if(i_QI_Error<=-6)
    {
        bCRCLbackup-=1;//3;
    }
    else if(i_QI_Error<0)
    {
        //bCRCLbackup--;

        bDamibackup = PWM_Duty_Get(0);

        if(bDamibackup>bCRCLbackup/4)
        {
            bDamibackup--;
        }
        else
        {
            bCRCLbackup--;
        }

        PWM_SetFreq_(bCRCLbackup);
        PWM_Duty_Set(bDamibackup);

        //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
        //PWMLOADEN |= (EPWM_CH_0_MSK+EPWM_CH_1_MSK);
        //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//            PWMCNTCLR = 0x03;
        i_QI_Error=0;
        fgEndThisControl=1;
        return;
    }
    if(i_QI_Error>=50)
    {
        bCRCLbackup+=6;
    }
    else if(i_QI_Error>=30)
    {
        bCRCLbackup+=4;//3;
    }
	else if(i_QI_Error>=15)
    {
        bCRCLbackup+=2;//3;
    }
    else if(i_QI_Error>=6)
    {
        bCRCLbackup+=1;//3;
    }
    else if(i_QI_Error>0)
    {
//        value = (((uint16_t)CCAP0H)<<8) + CCAP0L ;
//        dami0 = 0xffff-value;

        bDamibackup = PWM_Duty_Get(0);

        if(bDamibackup<bCRCLbackup/2)
        {
            bDamibackup++;
        }
        else
        {
            bCRCLbackup++;
			bDamibackup-=5;
        }

        PWM_SetFreq_(bCRCLbackup);
        PWM_Duty_Set(bDamibackup);

        //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
        //PWMLOADEN |= (EPWM_CH_0_MSK+EPWM_CH_1_MSK);
        //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//            PWMCNTCLR = 0x03;
        i_QI_Error=0;
        fgEndThisControl=1;
        return;
    }
   // if(bCRCLbackup>=D_QI_AI_Sv_Lv0_Code&&find36wflg) //PWM frequency minimum limit
       //  bCRCLbackup=D_QI_AI_Sv_Lv0_Code;
	// if(bCRCLbackup>=471&&find36wflg) //PWM frequency minimum limit
      //   bCRCLbackup=471;
	if(bCRCLbackup>=64000/135)
		 bCRCLbackup=64000/135;

    if(bCRCLbackup<=D_QI_AI_Sv_Lv3) //PWM frequency maxmum limit
    {
        bCRCLbackup=D_QI_AI_Sv_Lv3;

//        value = (((uint16_t)CCAP0H)<<8) + CCAP0L ;
//        dami0 = 0xffff-value;


        bDamibackup = PWM_Duty_Get(0);

        if(i_QI_Error<=-50)
        {
            bDamibackup -= 9;
        }
        else if(i_QI_Error<=-6)
        {
            bDamibackup -= 3;
        }
        else if(i_QI_Error<0)
        {
            bDamibackup--;
        }
        if(i_QI_Error>=50)
        {
            bDamibackup += 9;
        }
        else if(i_QI_Error>=6)
        {
            bDamibackup += 3;
        }
        else if(i_QI_Error>0)
        {
            bDamibackup++;
        }
//					if(i_QI_Error<0)
//					{
//							bDamibackup = dami0-10;
//					}
//					else
//					{
//							bDamibackup = dami0+10;
//					}
//        if(bDamibackup<bCRCLbackup/20)
//        {
//            bDamibackup = bCRCLbackup/20;
//        }
        if(bDamibackup<bCRCLbackup/5)
        {
            bDamibackup = bCRCLbackup/5;
        }
        if(bDamibackup>bCRCLbackup/2)
        {
            bDamibackup = bCRCLbackup/2;
        }


        PWM_SetFreq_(bCRCLbackup);
        PWM_Duty_Set(bDamibackup);
        //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
        //PWMLOADEN |= (EPWM_CH_0_MSK+EPWM_CH_1_MSK);
        //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//        PWMCNTCLR = 0x03;
    }
    else
    {
        if(bSamsungIsFull==1)
        {
            D_PWM_OpenHalf;
        }
		if(find36wflg)
		  D_PWM_OpenAll;	

        PWM_SetFreq(bCRCLbackup);
        //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
        //PWMLOADEN |= (EPWM_CH_0_MSK+EPWM_CH_1_MSK);
        //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//        PWMCNTCLR = 0x03;
    }
    fgEndThisControl=1;
}


void F_QI_PowerTransfer_Control_Status_half_duty()
{
    b_PP_ControlStatus = D_PP_ConrolPhase_1;
}

/***********************************
// Standard QI Power Transfer Control Register Bank 1
************************************/
void F_QI_PowerTransfer_Control()
{
    if(fgFOD && fgFODStart &&  b15WMOD==0)
    {
        if(wSendPower > w_QI_Retified_Power_Limit)
        {
            bTempFODCnt++;

#ifdef DEBUG_PRINT
//            puts("\r\n----------");
//            my_itoa(w_QI_Retified_Power_Limit,0);
//            my_itoa(wSendPower,0);
//            puts("----------\r\n");
#endif
            if(bTempFODCnt>=200)
            {
                bTempFODCnt = 200;
                wUSBPowerCounter  = 2000;
//                if(bTargetStatus != D_Target_Status_Complete && bTargetStatus != D_Target_Status_TI_Complete)
//                    bTargetStatus = D_Target_Status_FOD;
            }
        }
        else if( i_QI_Error >0 && D_QI_Received_Power_Status_Freeze == bReceivedPowerStatus )
        {
            bTempFODCnt = 0;
            return;
        }
        else
        {
            bTempFODCnt = 0;
            if( i_QI_Error == 0x0)
            {
                return;
            }
            if( fgEndThisControl )
                return;
        }
    }
    else
    {
        // if( i_QI_Error == 0x0)
        // {
        //    return;
        // }
        if( fgEndThisControl )
            return;
    }

    if( fgEndThisControl )
        return;

 //   if((bManufactor==D_Manufactor_apple)&&(fFastcharge>=1))
   // {
     //   b_PP_ControlStatus=D_PP_ConrolPhase_3;
   // }

    if( D_PP_ConrolPhase_1 == b_PP_ControlStatus )
        F_QI_PowerTransfer_Control_Status_Full_Bridge();
    else if( D_PP_ConrolPhase_2 == b_PP_ControlStatus )
        F_QI_PowerTransfer_Control_Status_half_duty();
    else if(D_PP_ConrolPhase_3 == b_PP_ControlStatus)
    {
        F_QI_PowerTransfer_Control_Status_apple7_5();
    }
    else;
}
