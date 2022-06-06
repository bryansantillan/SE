#include "leon3_ev_handling.h"
#include "leon3_hw_irqs.h"

#include "leon3_uart.h"
#include "leon3_bprint.h"

#include "leon3_monotonic_clk.h"
#include "leon3_timer_unit_drv.h"
#include "leon3_timing_service.h"

/* Definición de las Macros de Configuración */
#define CYCLIC_EXECUTIVE_PERIOD_INT_TICKS	20 //duración del período básico en ticks
#define CYCLIC_EXECUTIVE_HYPER_PERIOD		 6 //número de períodos básicos del hiperperiodo
#define CYCLIC_EXECUTIVE_TASKS_NUMBER		 3 //número total de tareas del ejecutivo cíclico

/* Código de la Tarea T1 */
void task_1(void) {
	leon3_print_string(" Start T1\n");
	emu_execution_time(1000); /* Execution time 1000 ms */
	leon3_putchar('\n');
	leon3_print_string(" End T1\n");
}

/* Código de la Tarea T2 */
void task_2 (void) {
	leon3_print_string(" Start T2\n");
	emu_execution_time(300); /* Execution time 300 ms */
	leon3_putchar('\n');
	leon3_print_string(" End T2\n");
}

/* Código de la Tarea T3 */
void task_3 (void) {
	leon3_print_string(" Start T3\n");
	emu_execution_time(500); /* Execution Time 500 ms */
	leon3_putchar('\n');
	leon3_print_string(" End T3\n");
}

/* Array bidimensional para definir la secuencia de tareas del ejecutivo cíclico */
void (*cyclic_executive [CYCLIC_EXECUTIVE_HYPER_PERIOD] [CYCLIC_EXECUTIVE_TASKS_NUMBER + 1]) (void) = {
		{task_1, task_2, task_3, NULL},
		{task_1, NULL, NULL, NULL},
		{task_2, task_1, task_3, NULL},
		{task_1, task_2, NULL, NULL},
		{task_1, task_3, NULL, NULL},
		{task_2, task_1, NULL, NULL}
};

/* Función Principal */
int main() {

	/* Instalar manejadores de traps para habilitar y dehsabilitar las interrupciones */
	leon3_set_trap_handler(0x83, leon3_trap_handler_enable_irqs);
	leon3_set_trap_handler(0x84, leon3_trap_handler_disable_irqs);

	/* Declaración de variables de control del ejecutivo cíclico */
	uint8_t current_period = 0; //variable que determina el periodo basico actual que ese esta ejecutando
	int task_index = 0;//determina la posicion de la tarea actual a ejecutar dentro de la secuencia de tareas de cada periodo
	uint64_t next_period_in_ticks_from_reset;

	/* Inicializar servicio de temporización con el tiempo universal actual */
	init_timing_service(date_time_to_Y2K(18, 3, 22, 0, 0, 0));

	/* Obtener la referencia absoluta desde el reset del sistema*/
	next_period_in_ticks_from_reset = get_tick_counter_from_reset();

	while (1) {

		task_index = 0; /* Poner a 0 al inicio de cada periodo básico */

		leon3_print_string("\nStart Period\n");
		print_date_time_from_Y2K(get_universal_time_Y2K());

		/* Control de la ejecución de las tareas de cada periodo básico */
		while (cyclic_executive[current_period][task_index]) {
			cyclic_executive[current_period][task_index]();
			task_index++;
		}

		/* Sincronización con el inicio del siguiente periodo básico */
		next_period_in_ticks_from_reset += CYCLIC_EXECUTIVE_PERIOD_INT_TICKS;
		wait_until(next_period_in_ticks_from_reset);

		/* Siguiente Perido Básico */
		current_period++;
		if (current_period == CYCLIC_EXECUTIVE_HYPER_PERIOD) {
			current_period = 0;
			leon3_print_string("\n****************\n");
			leon3_print_string("\nNext Hyperperiod\n");
			leon3_print_string("\n****************\n");
		}

	}

	return 0;
}
