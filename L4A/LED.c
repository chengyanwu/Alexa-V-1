#include "LED.h"

void LED_Init(void) {
	// Enable GPIO Clocks
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; // mask: 0x00000002; value: 0x00000002
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN; // mask: 0x00000010; value: 0x00000010
	
	// Initialize Red LED
	GPIOB->MODER &= ~(3UL<<4); // Clear mode bits // mask: 0x00000030
	GPIOB->MODER |= 1UL<<4; // Set mode to output // mask: 0x00000010; value: 0xFFFFFF9F
	
	GPIOB->OTYPER &= ~(1UL<<2);	// Select push-pull output // mask: 0x00000004; value: 0x00000000
	//GPIOB->ODR |= 1UL << 2;	// Output 1 to turn on LED		 // mask: 0x00000004; value: 0x00000004
	
	// Initialize Green LED
	GPIOE->MODER &= ~(3UL<<16); // mask: 0x00030000
	GPIOE->MODER |= 1UL<<16;		// mask: 0x00010000; value: 0xFFF9FFFF
	
	GPIOE->OTYPER &= ~(1UL<<8);	// mask: 0x00000100; value: 0x00000000
	//GPIOE->ODR |= 1UL << 8;			// mask: 0x00000100; value: 0x00000100
}

void Red_LED_Off(void) {
	GPIOB->ODR &= ~(1UL << 2);
}

void Red_LED_On(void) {
	GPIOB->ODR |= 1UL << 2;
}

void Red_LED_Toggle(void){
	GPIOB->ODR ^= 1UL << 2;
}

void Green_LED_Off(void) {
	GPIOE->ODR &= ~(1UL << 8);
}

void Green_LED_On(void) {
	GPIOE->ODR |= 1UL << 8;
}

void Green_LED_Toggle(void) {
	GPIOE->ODR ^= 1UL << 8;
}
