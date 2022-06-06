/*
 * leon3_bprint.c
 *
 *  Created on: Mar 15, 2020
 *      Author: EdelDiaz UAH
 */
//*******************************INCLUDES***************************************
#include "leon3_uart.h"

//*******************************DEFINES****************************************
//*******************************VARIABLES**************************************
//*******************************FUNCTIONS**************************************
int8_t leon3_print_string(char* str){

	int8_t error=0;
  
	while(*str!='\0'&&(!error)){
		error=leon3_putchar(*str);
		str++;
	}

	while(!leon3_uart_tx_fifo_is_empty());
	
  return error; 
}

//uint8 [255-0]
int8_t leon3_print_uint8(uint8_t i){
	
	int8_t error=0;
	uint8_t aux = 100;//centenas
	int8_t first_digit = 0;

	if(i==0)
		error=leon3_putchar('0');
	else{
		while(aux&&(!error)){
			uint8_t digit;

			digit=i/aux;
			i-=aux*digit;
			aux=aux/10;
			if((first_digit)||digit){
				error=leon3_putchar('0'+digit);
				first_digit=1;
			}
		}
	}

	while(!leon3_uart_tx_fifo_is_empty());
	
  return error;
}

/*inicio bucle: numero 204

204/100
digit=2
4=204-200
----
4/10
digit=0
4=4-0
----
4/1
digit=4
0=4-4

fin bucle
*/

int8_t leon3_print_uint32( uint32_t i){
	//HAY que coficar esta parte
	//Se explica paso por paso lo que se ha ido haciendo
	int8_t e=0; // Error
		uint32_t aux = 1000000000; // Ira conteniendo 10⁹, 10⁸, 10⁷, etc
		int8_t first=0; // Cuando el primer dígito se ha escrito cambia a 1
						// De esta forma evitamos que se escriban 0
						// antes del primer dígito

		if(i==0)
			leon3_putchar('0');
		else{
			while(aux&&(!e)){ // Mientras alguno no sean 0
				uint8_t n;

				n=i/aux; //Sacamos las centenas, decenas o unidades
				i-=aux*n; //Restamos las centena, decenas o unidades
				aux=aux/10;
				if(first||n){
					e=leon3_putchar(n+'0');
					first=1;
				}
			}
		}
		return e;
}

