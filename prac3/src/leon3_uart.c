/*
 * leon3_uart.c
 *
 *  Created on: Feb 2, 2021
 *      Author: EdelDiaz UAH
 */
//*******************************INCLUDES***************************************
#include "leon3_uart.h"

//********************************DEFINES***************************************
//STATUS REGISTER MASKS
//!LEON3 UART A TX FIFO is full
#define LEON3_UART_TFF (0x200)

//!LEON3 UART A TX FIFO is empty
#define LEON3_UART_TFE  (0x004)

#define leon3_UART_TF_IS_FULL() (LEON3_UART_TFF&pLEON3_UART_REGS->Status)

#define MAX_WAIT_TIME 0xAAAAA

//********************************VARIABLES*************************************
//Estructura de datos que permite acceder a los registros de la //UART de LEON3
struct UART_regs{
	/** \brief UART  Data Register */
	volatile uint32_t Data;   	/* 0x80000100 */
	/** \brief UART  Status Register */
	volatile uint32_t Status; 	/* 0x80000104 */
	/** \brief UART  Control Register */
	volatile uint32_t Ctrl; 	/* 0x80000108 */
	/** \brief UART  Scaler Register */
	volatile uint32_t Scaler; 	/* 0x8000010C */
};

struct  UART_regs * const pLEON3_UART_REGS= (struct   UART_regs *)0x80000100;

//*******************************FUNCTIONS**************************************

int8_t leon3_putchar(char c){
	uint32_t write_timeout=0;

	while(
			(leon3_UART_TF_IS_FULL())
			&&(write_timeout < MAX_WAIT_TIME)
		){
			write_timeout++;

	}
	if(write_timeout <  MAX_WAIT_TIME)
		pLEON3_UART_REGS->Data=c;

	return (write_timeout ==  MAX_WAIT_TIME);
}

int8_t leon3_uart_tx_fifo_is_empty(){
	return (LEON3_UART_TFE&pLEON3_UART_REGS->Status);
}