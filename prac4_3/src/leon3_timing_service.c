#include "leon3_types.h"

#include "leon3_timer_unit_drv.h"
#include "leon3_ev_handling.h"
#include "leon3_hw_irqs.h"

#include "leon3_timing_service.h"
#include "leon3_monotonic_clk.h"

#define LEON3_FREQ_MHZ 				20
#define TIMER_ID				 	 0
#define TIMER_IRQ_LEVEL				 8

static uint64_t TickCounterFromReset = 0;

//***************************************************************************************
void timertick_irq_handler (void){
	leon3_timer_clear_irq(TIMER_ID);
	TickCounterFromReset++;
	irq_handler_update_monotonic_clock();
}

//***************************************************************************************
uint64_t get_tick_counter_from_reset (void){

	uint64_t tick_counter_from_reset;

	/* Enmascaro la interrupción del timer para evitar condiciones de carrera */
	leon3_mask_irq(TIMER_IRQ_LEVEL);

	tick_counter_from_reset = TickCounterFromReset;

	/* Desenmascaro para retornar a la situación preview */
	leon3_unmask_irq(TIMER_IRQ_LEVEL);

	return tick_counter_from_reset;
}

//***************************************************************************************
uint8_t wait_until (uint64_t ticks_from_reset){

	uint8_t error = 0;
	uint64_t tick_counter_from_reset = 0;

	tick_counter_from_reset = get_tick_counter_from_reset();

	if (ticks_from_reset < tick_counter_from_reset) {
		error = 1;
	} else {
		while (tick_counter_from_reset < ticks_from_reset) {
			tick_counter_from_reset = get_tick_counter_from_reset();
		}
	}

	return error;

}

//***************************************************************************************
void init_timing_service(uint32_t currentTime_in_Y2K){
	//1) TODO: Llamada al sistema para deshabilitar interrupciones
	leon3_sys_call_disable_irqs();
	
	//2) TODO: Enmascarar la irq del timer usando la macro TIMER_IRQ_LEVEL
	leon3_mask_irq(TIMER_IRQ_LEVEL);

	//3) TODO: Deshabilitar el timer usando la macro TIMER_ID
	leon3_timer_disable(TIMER_ID);

    //4) Deshabilitar la interrupción del Timer usando la macro TIMER_ID
	leon3_timer_disable_irq(TIMER_ID);

	//5) Configuración la TimerUnit:
	  // - Salida del prescaler con frecuencia de 1 Hz
	  // - Se habilita el freeze durante la depuración
	  // - Se generan interrupciones separadas para los dos timers

	leon3_timerunit_set_configuration( LEON3_FREQ_MHZ-1, true , true );

	//6) Configuración del timer:
	  // - Se genere un underflow periódico con periodo 1000000UL/ticks_per_second
	  // - No se encadenan los timers
	  // - Se reinicia el timer tras el underflow

	leon3_timer_config(TIMER_ID, 1000000UL/TIMING_SERVICE_TICKS_PER_SECOND -1, false, true);

	//7) TODO: Instalar timertick_irq_handler como manejador de usuario de la interrupción del timer. Usar la macro TIMER_IRQ_LEVEL
	leon3_install_user_hw_irq_handler(TIMER_IRQ_LEVEL, timertick_irq_handler);

	//8) Inicializar el reloj monotónico
	init_monotonic_clock(currentTime_in_Y2K);

	//9) TODO: Hacer un clear la interrupción del timer usando la macro TIMER_ID
	leon3_timer_clear_irq(TIMER_ID);

	//10) TODO: Desenmascarar la interrupción externa del timer. Usar la macro TIMER_IRQ_LEVEL
	leon3_unmask_irq(TIMER_IRQ_LEVEL);

	//11) TODO: Habilitar la interrupción del timer usando la macro TIMER_ID
	leon3_timer_enable_irq(TIMER_ID);

	//12) TODO: Habilitar el timer usando la macro TIMER_ID
	leon3_timer_enable(TIMER_ID);

	//13) TODO: Llamada al sistema para habilitar interrupciones
	leon3_sys_call_enable_irqs();

}
