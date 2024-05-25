
/* User includes */
#include "Mcal.h"
#include "S32K312.h"
#include "S32K312_MC_ME.h"
#include "Clock_Ip.h"
#include "Siul2_Port_Ip.h"
#include "Siul2_Dio_Ip.h"
#include "Siul2_Icu_Ip.h"
#include "Siul2_Icu_Ip_Defines.h"
#include "Pit_Ip.h"
#include "IntCtrl_Ip.h"

/* Macro definitions */
#define PIT0_INST 0
#define PIT0_CH0 0
#define SIUL2_ICU_IP_INSTANCE 0

/* External functions and variables */
extern ISR(PIT_0_ISR);
extern ISR(SIUL2_EXT_IRQ_8_15_ISR);

/* Callback functions definition (Interrupt Handler) */

void Pit0_Ch0_Callback (uint8 channel)
{
	(void)channel;
//	Siul2_Dio_Ip_TogglePins(LED_GREEN_PORT, (1<<LED_GREEN_PIN));
//	Siul2_Dio_Ip_TogglePins(LED_BLUE_PORT, (1<<LED_BLUE_PIN));
	Siul2_Dio_Ip_TogglePins(LED_RED_PORT, (1<<LED_RED_PIN));
	return;
}

void User_SW0_Callback(void)
{
	Siul2_Dio_Ip_TogglePins(LED_BLUE_PORT, LED_BLUE_PIN);
	return;
}


int main(void)
{
	//Clock Initialization
    Clock_Ip_Init(&Clock_Ip_aClockConfig[0]);
    Clock_Ip_EnableModuleClock(SIUL2_CLK);

    //SIUL2 Port Configuration
    Siul2_Port_Ip_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

    //USER_SW0
    Siul2_Port_Ip_SetInputBuffer(PORTB_H_HALF, 26, TRUE, 58, PORT_INPUT_MUX_ALT0); //PTB26 as External Interrupt  Input
    Siul2_Port_Ip_SetPullSel(PORTB_H_HALF, 26, PORT_INTERNAL_PULL_DOWN_ENABLED); //PullDown

    //LEDS
    Siul2_Port_Ip_SetOutputBuffer(PORTA_H_HALF, 29, TRUE, PORT_MUX_AS_GPIO); //PTA29 as Output
    Siul2_Port_Ip_SetOutputBuffer(PORTA_H_HALF, 30, TRUE, PORT_MUX_AS_GPIO); //PTA30 as Output
    Siul2_Port_Ip_SetOutputBuffer(PORTA_H_HALF, 31, TRUE, PORT_MUX_AS_GPIO); //PTA31 as Output

    /*OsIf initialization */
    OsIf_Init(NULL);

    /* Periodic Interrupt Timer (PIT)  and interrupt configuration */
    Pit_Ip_Init(PIT0_INST, &PIT_0_InitConfig_PB);
    Pit_Ip_InitChannel(PIT0_INST, &PIT_0_ChannelConfig_PB[0]);

    Pit_Ip_StartChannel(PIT0_INST, PIT0_CH0, 30000000);
    Pit_Ip_EnableChannelInterrupt(PIT0_INST, PIT0_CH0);

    IntCtrl_Ip_InstallHandler(PIT0_IRQn, PIT_0_ISR, NULL_PTR);
    IntCtrl_Ip_EnableIrq(PIT0_IRQn);

    /*SIUL_Input Capture Unit (ICU) configuration */
    Siul2_Icu_Ip_Init(SIUL2_ICU_IP_INSTANCE, &Siul2_Icu_Ip_0_Config_PB);

//    // Interrupt configuration Enable SIUL2_EIRQ13
//    Siul2_Icu_Ip_SetActivationCondition(SIUL2_ICU_IP_INSTANCE, (*Siul2_Icu_Ip_0_Config_PB.pChannelsConfig)[0].hwChannel, SIUL2_ICU_RISING_EDGE);
//    Siul2_Icu_Ip_EnableNotification(SIUL2_ICU_IP_INSTANCE, (*Siul2_Icu_Ip_0_Config_PB.pChannelsConfig)[0].hwChannel);
//    Siul2_Icu_Ip_EnableInterrupt(SIUL2_ICU_IP_INSTANCE, (*Siul2_Icu_Ip_0_Config_PB.pChannelsConfig)[0].hwChannel);
//
//    IntCtrl_Ip_InstallHandler(SIUL_1_IRQn, SIUL2_EXT_IRQ_8_15_ISR , NULL_PTR);
//    IntCtrl_Ip_EnableIrq(SIUL_1_IRQn);

    while(1)
    {
    	if (Siul2_Dio_Ip_ReadPin(USER_SW0_PORT,USER_SW0_PIN)!=0){
    		Siul2_Dio_Ip_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, 1); //Turn On Green LED
    	}else{
    		Siul2_Dio_Ip_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, 0); //Turn On Green LED
    	}
    }

}

/** @} */
