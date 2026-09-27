
#ifndef __APP_PROTECT_H
#define __APP_PROTECT_H
#include <stdint.h>
#include <stdbool.h>


//TX
#define  SYS_OVP_V     2800      //输入过压值
#define  SYS_UVP_V     2120   //2120  //输入欠压值  21.5
#define  SYS_OCP_V     5000     // 过流 0.001A
#define  SYS_OTP_V     109   //86     //过温
#define  SYS_IN_OVP_V   7800     //输入过压
#define  SYS_IN_UVP_V   2300     //输入欠压 
 


//恢复电压
//TX
#define  REC_SYS_OVP_V     2550      //输入过压值 
#define  REC_SYS_UVP_V     0  //2280    //输入欠压值 
#define  REC_SYS_OCP_V     1000     //过流 0.001A
#define  REC_SYS_OTP_V     50     //过温
#define  REC_SYS_IN_OVP_V   6000     //输入过压 恢复
#define  REC_SYS_IN_UVP_V   2600     //输入过压 


#define  BUCK_OPEN_VAL 2500

typedef enum
{
    PROTECT_OVER = 0,     // > 过压
    PROTECT_UNDER          // < 
}ProtectType;



typedef struct
{
    uint16_t *value;          // 指向检测值
    uint16_t threshold;       // 门限
    uint16_t recover;             // 恢复值(回差)
    uint16_t delay;           // 延时次数
    uint16_t recover_delay;       // 恢复延时
    uint16_t recover_counter;
    uint16_t counter;         // 内部计数
    ProtectType type;         // 过压/欠压
    uint8_t fault_id;         // 对应故障bit

}ProtectItem;


void Protect_Scan(uint16_t *pt);

#endif


