#include "leon3_uart.h"
#include "leon3_hw_irqs.h"
#include "leon3_ev_handling.h"
#include "leon3_bprint.h"


void device_hw_irq_level_1_handler(void){
	leon3_print_string("Device HW IRQ user handler \n");
}

//Ejercicio 7..
void device_hw_divide_exception(void){
	leon3_print_string("error, división por cero");
}
//..Ejercicio 7

int main()
{
	//Instalar como manejador del trap 0x83 la rutina
	// que habilita las interrupciones

	leon3_set_trap_handler(0x83,leon3_trap_handler_enable_irqs);
	//Instalar el manejador del trap que 0x83 la rutina
	// que deshabilita las interrupciones
	leon3_set_trap_handler(0x84,leon3_trap_handler_disable_irqs);
	//anado
	leon3_set_trap_handler(0x82,leon3_trap_handler_disable_irqs);
	//termina
	 //Llamada al sistema para deshabilitar las interrupciones
	 leon3_sys_call_disable_irqs();


	//COMPLETAR
	//
	//
	//Enmascarar todas las interrupciones
	 //leon3_mask_irq(1);
	leon3_mask_all_irqs();


	//Instalar la función device_hw_irq_level_1_handler como
	// manejador de usuario de la interrupción de nivel 1
	 leon3_install_user_hw_irq_handler(1,device_hw_irq_level_1_handler);
	 leon3_install_user_hw_irq_handler(15,device_hw_divide_exception);//Ejercicio 7
	//Desenmascarar la interrupción de nivel 1
	leon3_unmask_irq(1);
	leon3_unmask_irq(15);//Ejercicio 7
	//Llamada al sistema para habilitar las interrupciones
	leon3_sys_call_enable_irqs();//Ejercicio 6- comentarlo, comprobando que tampoco se genera ningún mensaje.
	//Ejercicio 5- Al enmascarar antes del force, vemos que no se muestra nada por la terminal
	//leon3_mask_irq(1);

	//acaba el ejercicio 5
	//Fuerza la interrupción
	leon3_force_irq(1);

	//Ejercicio 7 Ejecutar el siguiente código al final del programa. ¿Qué ocurre? Sabiendo que
	//cuando se produce una división por 0 la rutina de la biblioteca que implementa la
	//división llama al trap 0x82, ¿cómo utilizarías la función
	//leon3_set_trap_handler para conseguir que el programa no se cuelgue y en su
	//lugar imprima un mensaje que diga “error, división por cero”?
	uint8_t i;
	uint8_t j;


	for(i=10; i>0; i--) //va decrementando de 10 a 1
		j=j/(i-9);  //se va dividiendo
		leon3_force_irq(15);//Ejercicio 7, fuerza la interrupcion

	//FIN COMPLETAR
	return 0;
}
