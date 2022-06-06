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

//PL3 APARTADO B: declaraciones

#define MAX_WAIT_TIME 0xAAAAA

//!LEON3 UART CTRL RX ENABLE MASK
#define LEON3_UART_CTRL_RX_ENABLE (0x001)

//!LEON3 UART CTRL RX IRQ ENABLE MASK
#define LEON3_UART_CTRL_RX_IRQ_ENABLE (0x004)

//!LEON3 UART SET_LOOP_BACK MASK
#define LEON3_UART_CTRL_SET_LOOP_BACK  (0x080)

// FIN PL3 Apartado B


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

//Apartado B

char leon3_getchar() {
	//Devuelve el valor (convertido a uint8_t) alojado en el registro Data de la UART-A
	return (uint8_t) pLEON3_UART_REGS->Data;
}

void leon3_uart_ctrl_rx_enable() {
	/*Pone a 1 el campo Receiver_enable del registro de control de la UART-A
	  sin modificar el resto de campos de ese registro
	  habilitando la recepción de datos a través de la UART. */
	pLEON3_UART_REGS->Ctrl |= LEON3_UART_CTRL_RX_ENABLE;
}

void leon3_uart_ctrl_rx_irq_enable() {
	/*Pone a 1 el campo Receiver_interrupt_enable del registro de control de la UART-A
	  sin modificar el resto de campos de ese registro
	  habilitando las interrupciones tras la recepción de datos por la UART. */
	pLEON3_UART_REGS->Ctrl |= LEON3_UART_CTRL_RX_IRQ_ENABLE;
}

void leon3_uart_ctrl_rxtx_loop(uint8_t set_rxtxloop) {
	/*Recibe el valor que se quiere fijar en el campo loop_back del registro de control de la UART-A
	  de modo que si set_rxtxloop vale 1 se habilita el modo LOOP_BACK de la UART
	  y si vale 0, se deshabilita. */
	if(set_rxtxloop){
		pLEON3_UART_REGS -> Ctrl |= LEON3_UART_CTRL_SET_LOOP_BACK;
	}else{
		pLEON3_UART_REGS -> Ctrl &= ~LEON3_UART_CTRL_SET_LOOP_BACK;
	}
}
