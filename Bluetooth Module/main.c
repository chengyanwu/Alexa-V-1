#include "stm32l476xx.h"

#include "LED.h"
#include "SysClock.h"
#include "UART.h"
#include "EXTI.h"

#include <string.h>
#include <stdio.h>

// Initializes USARTx
// USART2: UART Communication with Termite
// USART1: Bluetooth Communication with Phone
void Init_USARTx(int x) {
	if(x == 1) {
		UART1_Init();
		UART1_GPIO_Init();
		USART_Init(USART1);
	} else if(x == 2) {
		UART2_Init();
		UART2_GPIO_Init();
		USART_Init(USART2);
	} else {
		// Do nothing...
	}
}

int main(void) {
	System_Clock_Init(); // Switch System Clock = 80 MHz
	LED_Init();
	EXTI_Init();
	
	// Initialize UART -- change the argument depending on the part you are working on
	Init_USARTx(1);
	
	char rxByte;
	while(1) {
		printf("Please enter a command: ");
		scanf("%s", &rxByte);
		
		if (rxByte == 'Y' || rxByte == 'y'){
			Red_LED_On();
			printf("The red LED is now on.\n");
		} else if (rxByte == 'N' || rxByte == 'n'){
			Red_LED_Off();
			printf("The red LED is now off.\n");
		} else {
			printf("Invalid command. Possible commands: 'Y', 'y', 'N', 'n'\n");
		}
	}
}
