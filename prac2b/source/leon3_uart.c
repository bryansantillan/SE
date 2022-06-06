#include "leon3_uart.h"
//COMPLETAR MACROS:
//! LEON3 UART A Transmit FIFO is FULL
#define LEON3_UART_TFF (0x200)
//Definir el bit de TFF en 1 que es el bit de la posicion 9, es decir, 1000000000 = 200

//! LEON3 UART A Transmit FIFO is EMPTY
#define LEON3_UART_TFE (0x004)
//Definir el bit de TFE en 1 que es el bit de la posicion 2, es decir, 0000000100 = 004
#define leon3_UART_TF_IS_FULL() (pLEON3_UART_REGS->Status & LEON3_UART_TFF)

//PL3 APARTADO B: declaraciones

#define MAX_WAIT_TIME 0xAAAAA

//!LEON3 UART CTRL RX ENABLE MASK
#define LEON3_UART_CTRL_RX_ENABLE (0x001)

//!LEON3 UART CTRL RX IRQ ENABLE MASK
#define LEON3_UART_CTRL_RX_IRQ_ENABLE (0x004)

//!LEON3 UART SET_LOOP_BACK MASK
#define LEON3_UART_CTRL_SET_LOOP_BACK  (0x080)

// FIN PL3 Apartado B

//Estructura de datos que permite acceder a los registros de la
//UART de LEON3

struct UART_regs {
	/** \brief UART Data Register */
	volatile uint32_t Data; /* 0x80000100 */

	/** \brief UART Status Register */
	volatile uint32_t Status; /* 0x80000104 */

	/** \brief UART Control Register */
	volatile uint32_t Ctrl; /* 0x80000108 */

	/** \brief UART Scaler Register */
	volatile uint32_t Scaler; /* 0x8000010C */
};


struct UART_regs * const pLEON3_UART_REGS = 0x80000100;
//0x80000100, que es la dirección donde se encuentra ubicados los registros de la UART-A

int8_t leon3_putchar(char c) {
	uint32_t write_timeout = 0;
	while (leon3_UART_TF_IS_FULL() && (write_timeout < 0xAAAAA)) {
		write_timeout++;
	} //Espera mientras la cola de transmisión esté llena
	if (write_timeout < 0xAAAAA) {
//COMPLETAR. Escribir el carácter en el registro Data
		pLEON3_UART_REGS->Data = c;
	}
	return (write_timeout == 0xAAAAA);
}
int8_t leon3_uart_tx_fifo_is_empty() {
	int valor;
	if ((pLEON3_UART_REGS->Status & LEON3_UART_TFE)) {
		valor = 1;
	} else {
		valor = 0;
	}
	return valor;
}
