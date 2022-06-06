#include "leon3_types.h"

#include "leon3_timer_unit_drv.h"
#include "leon3_ev_handling.h"
#include "leon3_hw_irqs.h"

#include "leon3_timing_service.h"
#include "leon3_monotonic_clk.h"
//TODO
#define LEON3_FREQ_MHZ 				20
#define TIMER_ID				 	 0
#define TIMER_IRQ_LEVEL				 8

//***************************************************************************************
void timertick_irq_handler (void){
	leon3_timer_clear_irq(TIMER_ID);
	irq_handler_update_monotonic_clock();
}

//***************************************************************************************
//APRENDERSE ESTE PROCESO!
void init_timing_service(uint32_t currentTime_in_Y2K){
	//1) TODO:
	//Llamada al sistema para deshabilitar interrupciones
	//SysCall to Disable irqs
	leon3_sys_call_disable_irqs(); // de leon3_ev_handling.h

	
	//2) TODO:
	//enmascarar la irq del timer usando la macro TIMER_IRQ_LEVEL
	//mask timer irq level using TIMER_IRQ_LEVEL macro
	leon3_mask_irq (TIMER_IRQ_LEVEL); // leon3_hw_irqs.c

	//3) TODO:
    // Deshabilitar el timer usando la macro TIMER_ID
    // Disable timer using TIMER_ID macro
	leon3_timer_disable(TIMER_ID); // leon3_timer_unit_drv.c

    //4) Deshabilitar la interrupción del Timer usando la macro TIMER_ID
    //Disable Timer IRQ using TIMER_ID macro
	leon3_timer_disable_irq(TIMER_ID); //de leon3_timer_unit_drv.c

	//5) Configuración la TimerUnit:
	  //-Salida del prescaler con frecuencia de 1 Hz
	  //-Se habilita el freeze durante la depuración
	  //-Se generan interrupciones separadas para los dos timers

	//TimerUnit Config
	  //-Prescaler generates a signal of 1Hz
	  //-Freeze during debugging enabled
	  //-Independent interrupts for timer1 and timer0

	leon3_timerunit_set_configuration( LEON3_FREQ_MHZ-1, true , true ); // leon3_timer_unit_drv.c


	//6) Configuración del timer:
	  //-Se genere un underflow periódico con periodo 1000000UL/ticks_per_second
	  //-No se encadenan los timers
	  //-Se reinicia el timer tras el underflow

	leon3_timer_config(TIMER_ID, 1000000UL/TIMING_SERVICE_TICKS_PER_SECOND -1, false, true);// leon3_timer_unit_drv.c

	//7) TODO:
	//Instalar timertick_irq_handler como manejador de usuario de la interrupción del timer. Usar la macro TIMER_IRQ_LEVEL
	//Install timertick_irq_handler as user handler of timer irq using TIMER_IRQ_LEVEL macro

	//se le pasa el nivel de interrupción y el manejador de usuario...
	leon3_install_user_hw_irq_handler(TIMER_IRQ_LEVEL,timertick_irq_handler); //de leon3_ev_handling.c

	//8) Inicializar el reloj monotónico
	//Init monotonic clock
	init_monotonic_clock(currentTime_in_Y2K); //leon3_monotonic_clk.c


	//9) TODO:
	//Hacer un clear la interrupción del timer usando la macro TIMER_ID
	//Clear timer irq using TIMER_ID macro
	leon3_timer_clear_irq(TIMER_ID); //leon3_timer_unit_drv.c

	//10) TODO:
	//Desenmascarar la interrupción externa del timer. Usar la macro TIMER_IRQ_LEVEL
	//Unmask timer irq using TIMER_IRQ_LEVEL macro
	leon3_unmask_irq(TIMER_IRQ_LEVEL); //leon_hw_irqs.c

	//11) TODO:
	//Habilitar la interrupción del timer usando la macro TIMER_ID
	//Enable timer irq using TIMER_ID macro
	leon3_timer_enable_irq(TIMER_ID); //leon3_timer_unit_drv.c

	//12) TODO:
	//Habilitar el timer usando la macro TIMER_ID
	//Enable timer using TIMER_ID macro
	leon3_timer_enable(TIMER_ID); //leon3_timer_unit_drv.c

	//13) TODO:
	//Llamada al sistema para habilitar interrupciones
	//SysCall to Enable Interrupts

	leon3_sys_call_enable_irqs(); //leon3_ev_handling.h
}
