#include "EXTI.h"
#include "UART.h"

#include <string.h>
#include <stdio.h>

void EXTI_Init(void) {
   // Initialize Joystick
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; // mask: 0x00000001; value: 0x00000001
	//center
	GPIOA->MODER &= ~3UL;
	GPIOA->PUPDR &= ~3UL;
	GPIOA->PUPDR |= 2UL;
	//up
	GPIOA->MODER &= ~(3UL<<6);
	GPIOA->PUPDR &= ~(3UL<<6);
	GPIOA->PUPDR |= 2UL<<6;
	
	// Configure SYSCFG EXTI
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0;	// center
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA;
	
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI3; // up
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PA;
	
	
	// Configure EXTI Trigger
	 EXTI->RTSR1 |= EXTI_RTSR1_RT0;	// center
	//EXTI->FTSR1 |= EXTI_FTSR1_FT0;

	EXTI->RTSR1 |= EXTI_RTSR1_RT3;	// up
	//EXTI->FTSR1 |= EXTI_FTSR1_FT3;
	
	// Enable EXTI
	EXTI->IMR1 |= EXTI_IMR1_IM0;	// center
	EXTI->IMR1 |= EXTI_IMR1_IM3;	// up
	
	// Configure and Enable in NVIC
	NVIC_EnableIRQ(EXTI0_IRQn);	// center
	NVIC_SetPriority(EXTI0_IRQn, 0);
	
	NVIC_EnableIRQ(EXTI3_IRQn);	// up
	NVIC_SetPriority(EXTI3_IRQn, 0);
}

#define DAC_MIN 0
#define DAC_MAX 4095
#define DAC_INCREMENT 256

void EXTI0_IRQHandler(void) {
	// Clear interrupt pending bit
	EXTI->PR1 |= EXTI_PR1_PIF0;
	
	printf("CENTER");
}

void EXTI3_IRQHandler(void) {
	// Clear interrupt pending bit
	EXTI->PR1 |= EXTI_PR1_PIF3;

	printf("UP");


}
