#include <stdint.h>
#include <stdio.h>
#include "timerGeneral2_5_STM32F407.h"
#include "gpio_STM32F407.h"

//Static Prototypes--------------------------------------------------------------
static void PWM_PinInit(uint8_t timerNum);
static void Activate_Clock(uint8_t timerNum);
static GEN_TIMER_2_5x* Get_Timer(uint8_t timerNum);

//Global Variables------------------------------------------------------------------
TIM2_5_CLOCK *const Clock2_5 = ADDR_TIM2_5_CLOCK;
//TIMER 2
GEN_TIMER_2_5x *const TIM2 = ADDR_TIM2;
GEN_TIMER_2_5x *const TIM3 = ADDR_TIM3;
GEN_TIMER_2_5x *const TIM4 = ADDR_TIM4;
GEN_TIMER_2_5x *const TIM5 = ADDR_TIM5;

uint8_t GeneralTimer2_5_Start(uint8_t timerNum, uint16_t prescaler, uint16_t time) {
	
	Activate_Clock(timerNum);
	GEN_TIMER_2_5x *const TIMER = Get_Timer(timerNum);
	TIMER->PrescalerReg.rw_PrescalerValue = prescaler;
	TIMER->AutoReloadReg.NormalAutoReload.rw_AutoReloadValue = time;
	TIMER->CounterReg.NormalCounter.rw_CounterValue = 0x00;
	TIMER->ControlReg1.enable_AutoReloadPreload = 1;
	TIMER->ControlReg1.enable_Counter = 1;
	
	return 1;
}

void GeneralTimer2_5_PWM_Start(uint8_t timerNum, uint8_t captCompNum, uint16_t prescaler, uint16_t time, float dutycycle) {
	
	Activate_Clock(timerNum);
	GEN_TIMER_2_5x *const TIMER = Get_Timer(timerNum);
	PWM_PinInit(timerNum);
	
	TIMER->PrescalerReg.rw_PrescalerValue = prescaler;
	TIMER->AutoReloadReg.NormalAutoReload.rw_AutoReloadValue = time;
	TIMER->CounterReg.NormalCounter.rw_CounterValue = 0x00;
	TIMER->ControlReg1.enable_AutoReloadPreload = 1;
	TIMER->ControlReg1.enable_Counter = 1;

	switch (captCompNum) {
		case 1 :
			TIMER->CaptureCompModeReg1.OutputCompareMode.rw_OutputComp1Mode = PWM_ACTIVE_UNTIL_MATCH;
			TIMER->CaptureComp1Reg.NormalCaptureComp.rw_CaptureCompValue = (dutycycle * time);
			TIMER->CaptureCompEnableReg.enable_CaptComp1 = 1;
			break;
		case 2 :
			TIMER->CaptureCompModeReg1.OutputCompareMode.rw_OutputComp2Mode = PWM_ACTIVE_UNTIL_MATCH;
			TIMER->CaptureComp2Reg.NormalCaptureComp.rw_CaptureCompValue = (dutycycle * time);
			TIMER->CaptureCompEnableReg.enable_CaptComp2 = 1;
			break;	
		case 3 :
			TIMER->CaptureCompModeReg2.OutputCompareMode.rw_OutputComp3Mode = PWM_ACTIVE_UNTIL_MATCH;
			TIMER->CaptureComp3Reg.NormalCaptureComp.rw_CaptureCompValue = (dutycycle * time);
			TIMER->CaptureCompEnableReg.enable_CaptComp3 = 1;
			break;	
		case 4 :
			TIMER->CaptureCompModeReg2.OutputCompareMode.rw_OutputComp4Mode = PWM_ACTIVE_UNTIL_MATCH;
			TIMER->CaptureComp4Reg.NormalCaptureComp.rw_CaptureCompValue = (dutycycle * time);
			TIMER->CaptureCompEnableReg.enable_CaptComp4 = 1;
			break;
	}
		
	TIMER->ControlReg1.enable_Counter = 1;		
}

void GeneralTimer2_5_PWM_Update(uint8_t timerNum, uint8_t captCompNum, uint16_t time, float dutycycle) {
	
	GEN_TIMER_2_5x *const TIMER = Get_Timer(timerNum);

	switch (captCompNum) {
		case 1 :
			TIMER->CaptureComp1Reg.NormalCaptureComp.rw_CaptureCompValue = (dutycycle * time);
			TIMER->CaptureCompEnableReg.enable_CaptComp1 = 1;
			break;
		case 2 :
			TIMER->CaptureComp2Reg.NormalCaptureComp.rw_CaptureCompValue = (dutycycle * time);
			TIMER->CaptureCompEnableReg.enable_CaptComp2 = 1;
			break;	
		case 3 :
			TIMER->CaptureComp3Reg.NormalCaptureComp.rw_CaptureCompValue = (dutycycle * time);
			TIMER->CaptureCompEnableReg.enable_CaptComp3 = 1;
			break;	
		case 4 :
			TIMER->CaptureComp4Reg.NormalCaptureComp.rw_CaptureCompValue = (dutycycle * time);
			TIMER->CaptureCompEnableReg.enable_CaptComp4 = 1;
			break;
	}
		
	TIMER->ControlReg1.enable_Counter = 1;	
}

//Helper Functions--------------------------------------------------------------------------------------------------------------

/**
TIMER Pins ---------------------------
		
		+ TIM2_CH1: PA15 (AF1)
		+ TIM2_CH2: PA1, PB3 (AF1)
		+ TIM2_CH3: PA2, PB10 (AF1)
		+ TIM2_CH4: PA3, PB11 (AF1)
		+ TIM2_CH1_ETR: PA0, PA5 (AF1)
		+ TIM2_ETR: PA15 (AF1)

		+ TIM3_CH1: PA6, PB4, PC6 (AF2)
		+ TIM3_CH2: PA7, PB5, PC7 (AF2)
		+ TIM3_CH3: PB0, PC8 (AF2)
		+ TIM3_CH4: PB1, PC9 (AF2)
		+ TIM3_ETR: PD2 (AF2)

		+ TIM4_CH1: PB6, PD12 (AF2)
		+ TIM4_CH2: PB7, PD13 (AF2)
		+ TIM4_CH3: PB8, PD14 (AF2)
		+ TIM4_CH4: PB9, PD15 (AF2)
		+ TIM4_ETR: PE0 (AF2)
		
		+ TIM5_CH1: PA0, PH10 (AF2)
		+ TIM5_CH2: PA1, PH11 (AF2)
		+ TIM5_CH3: PA2, PH12 (AF2)
		+ TIM5_CH4: PA3, PI0 (AF2)
		------------------------------------
**/
static void PWM_PinInit(uint8_t timerNum) {
	
	switch (timerNum) {
		case 2 :
			Pin_Init('A', 15, AF1); //Channel 1
			Pin_Init('A', 1, AF1); //Channel 2
			Pin_Init('A', 2, AF1); //Channel 3
			Pin_Init('A', 3, AF1); //Channel 4
			break;
		case 3 :
			Pin_Init('A', 6, AF2); //Channel 1
			Pin_Init('A', 7, AF2); //Channel 2
			Pin_Init('B', 0, AF2); //Channel 3
			Pin_Init('B', 1, AF2); //Channel 4
			break;
		case 4 :
			Pin_Init('B', 6, AF2); //Channel 1
			Pin_Init('B', 7, AF2); //Channel 2
			Pin_Init('B', 8, AF2); //Channel 3
			Pin_Init('B', 9, AF2); //Channel 4
			break;
		case 5 :
			Pin_Init('A', 0, AF2); //Channel 1
			Pin_Init('A', 1, AF2); //Channel 2
			Pin_Init('A', 2, AF2); //Channel 3
			Pin_Init('A', 3, AF2); //Channel 4
			break;
	}
}


static void Activate_Clock(uint8_t timerNum) {
	
	switch (timerNum) {
		case 2 : 
			Clock2_5->tim2_StartTick = 1;
			break;
		case 3 : 
			Clock2_5->tim3_StartTick = 1;	
			break;
		case 4 : 
			Clock2_5->tim4_StartTick = 1;	
			break;
		case 5 : 
			Clock2_5->tim5_StartTick = 1;	
			break;
	}
}

static GEN_TIMER_2_5x* Get_Timer(uint8_t timerNum) {

	switch(timerNum) {
		case 2:
			return TIM2;
		case 3:
			return TIM3;
		case 4:
			return TIM4;
		case 5:
			return TIM5;
		default:
			return ((void*)0);
	}
}

