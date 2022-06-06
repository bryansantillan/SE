#include "leon3_bprint.h"

//transmite a través del puerto serie la cadena de caracteres
//pasada por parámetro.

int8_t leon3_print_string(char* cadena) {
	int i = 0;//indice para recorrer la cadena
	uint32_t e = 0;
	//Mientras la cadena tenga "algo"
	while ((e == 0) && (cadena[i] != '\0')) {
		e += leon3_putchar(cadena[i]);
		i++;
	}
	return e;
}

//transmite a través del puerto serie el entero transformado en
//cadena de caracteres.

int8_t leon3_print_uint8(uint8_t aux) {
	char caract;
	uint32_t e = 0;
	int uni,dec,cen;


	while ((!e) && (aux > 0) ) {
		//dependiendo de que numero tenemos, separamos entre 1, 2 o 3 cifras

		//1 cifra
		if(aux<=10){
			uni = aux;
			aux -= uni;
			caract = uni + '0';
			e += leon3_putchar(caract);

		}//2 cifras
		else if (aux <= 100){

			dec = aux / 10;
			aux -= 10 * dec;
			caract = dec + '0';
			e += leon3_putchar(caract);

			uni = aux;
			aux -= uni;
			caract = uni + '0';
			e += leon3_putchar(caract);

		}//3 cifras
		else{

			cen = aux / 100;
			aux -= 100 * cen;
			caract = cen + '0';
			e += leon3_putchar(caract);

			dec = aux / 10;
			aux -= 100 * dec;
			caract = dec + '0';
			e += leon3_putchar(caract);

			uni = aux;
			aux -= uni;
			caract = uni + '0';
			e += leon3_putchar(caract);
		}

		return e;
	}
}
