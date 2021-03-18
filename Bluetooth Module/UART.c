#include "UART.h"

void UART1_Init(void) {
	// enable clock
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	
	// select USART1 clock source
	RCC->CFGR |= RCC_CFGR_MCOSEL_0;
}

void UART2_Init(void) {
	// enable clock
	RCC->APB1ENR1 |= 1UL<<17;
	
	// select USART2 clock source
	RCC->CFGR |= RCC_CFGR_MCOSEL_0;
}

void UART1_GPIO_Init(void) {
	// enable clock
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

	// alternate function mode, pin 6
	GPIOB->MODER &= ~GPIO_MODER_MODE6_0;
	GPIOB->MODER |= GPIO_MODER_MODE6_1;
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL6_0;
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL6_1;
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL6_2;
	GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL6_3;

	// alternate function mode, pin 7
	GPIOB->MODER &= ~GPIO_MODER_MODE7_0;
	GPIOB->MODER |= GPIO_MODER_MODE7_1;
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL7_0;
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL7_1;
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL7_2;
	GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL7_3;

	// very high speed
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED6;
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED7;

	// push-pull
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT6;
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT7;

	// pull-up
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD6;
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD7;
	GPIOB->PUPDR |= GPIO_PUPDR_PUPD6_0;
	GPIOB->PUPDR |= GPIO_PUPDR_PUPD7_0;
}

void UART2_GPIO_Init(void) {
	// enable clock
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;
	
	// alternate function mode, pin 5
	GPIOD->MODER &= ~GPIO_MODER_MODE5_0;
	GPIOD->MODER |= GPIO_MODER_MODE5_1;
	GPIOD->AFR[0] |= GPIO_AFRL_AFSEL5_0;
	GPIOD->AFR[0] |= GPIO_AFRL_AFSEL5_1;
	GPIOD->AFR[0] |= GPIO_AFRL_AFSEL5_2;
	GPIOD->AFR[0] &= ~GPIO_AFRL_AFSEL5_3;
	
	// alternate function mode, pin 6
	GPIOD->MODER &= ~GPIO_MODER_MODE6_0;
	GPIOD->MODER |= GPIO_MODER_MODE6_1;
	GPIOD->AFR[0] |= GPIO_AFRL_AFSEL6_0;
	GPIOD->AFR[0] |= GPIO_AFRL_AFSEL6_1;
	GPIOD->AFR[0] |= GPIO_AFRL_AFSEL6_2;
	GPIOD->AFR[0] &= ~GPIO_AFRL_AFSEL6_3;
	
	// very high speed
	GPIOD->OSPEEDR |= GPIO_OSPEEDR_OSPEED5;
	GPIOD->OSPEEDR |= GPIO_OSPEEDR_OSPEED6;
	
	// push-pull
	GPIOD->OTYPER &= ~GPIO_OTYPER_OT5;
	GPIOD->OTYPER &= ~GPIO_OTYPER_OT6;
	
	// pull-up
	GPIOD->PUPDR &= ~GPIO_PUPDR_PUPD5;
	GPIOD->PUPDR &= ~GPIO_PUPDR_PUPD6;
	GPIOD->PUPDR |= GPIO_PUPDR_PUPD5_0;
	GPIOD->PUPDR |= GPIO_PUPDR_PUPD6_0;
}

void USART_Init(USART_TypeDef* USARTx) {
	// disable USART
	USARTx->CR1 &= ~USART_CR1_UE;

	// 8 bit word length
	USARTx->CR1 &= ~USART_CR1_M;
	
	// oversample by 16
	USARTx->CR1 &= ~USART_CR1_OVER8;
	
	// 1 stop bit
	USARTx->CR2 &= ~USART_CR2_STOP;

	// 9600 baud rate
	USARTx->BRR = 80000000 / 9600;

	// enable transmitter and receiver
	USARTx->CR1 |= USART_CR1_TE;
	USARTx->CR1 |= USART_CR1_RE;

	// enable USART
	USARTx->CR1 |= USART_CR1_UE;
}

uint8_t USART_Read (USART_TypeDef * USARTx) {
	// SR_RXNE (Read data register not empty) bit is set by hardware
	while (!(USARTx->ISR & USART_ISR_RXNE));  // Wait until RXNE (RX not empty) bit is set
	// USART resets the RXNE flag automatically after reading DR
	return ((uint8_t)(USARTx->RDR & 0xFF));
	// Reading USART_DR automatically clears the RXNE flag 
}

void USART_Write(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t nBytes) {
	int i;
	// TXE is cleared by a write to the USART_DR register.
	// TXE is set by hardware when the content of the TDR 
	// register has been transferred into the shift register.
	for (i = 0; i < nBytes; i++) {
		while (!(USARTx->ISR & USART_ISR_TXE));   	// wait until TXE (TX empty) bit is set
		// Writing USART_DR automatically clears the TXE flag 	
		USARTx->TDR = buffer[i] & 0xFF;
		USART_Delay(300);
	}
	while (!(USARTx->ISR & USART_ISR_TC));   		  // wait until TC bit is set
	USARTx->ISR &= ~USART_ISR_TC;
}   

void USART_Delay(uint32_t us) {
	uint32_t time = 100*us/7;    
	while(--time);   
}
