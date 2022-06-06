#include "leon3_types.h"

/* Punteros para acceder a los registros */
uint32_t * LEON3_ICLEAR =  (uint32_t *) (0x80000000 + 0x20C);
uint32_t * LEON3_IMASK  =  (uint32_t *) (0x80000000 + 0x240);
uint32_t * LEON3_IFORCE =  (uint32_t *) (0x80000000 + 0x208);

/* Pone a 1 el bit correspondiente al "IRQ_LEVEL" para hacer el clear de "PENDING_IRQ" */
uint8_t leon3_clear_irq (uint8_t irq_level) {
	uint8_t error = 0;

	if ((irq_level > 0) && (irq_level < 16)) {
		uint32_t bit = (1 << irq_level);	 	// Mask IRQ_LEVEL to clear
		*LEON3_ICLEAR = bit;					// Sets that bit to 1
	} else {
		error = 1;
	}

	return error;
}

/* Pone a 1 el bit del "IRQ_LEVEL" correspondiente al registro IFORCE forzando la interrupción del "IRQ_LEVEL" indicado */
uint8_t leon3_force_irq (uint8_t irq_level) {
	uint8_t error = 0;

	if((irq_level > 0) && (irq_level < 16)){
		uint32_t bit = (1 << irq_level);
		*LEON3_IFORCE = (*LEON3_IFORCE | bit);
	} else {
		error = 1;
	}

	return error;
}

/* Pone a 1 el bit del "IRQ_LEVEL" indicado en el registro de mascara, desenmascarando dicha interrupcion. */
uint8_t leon3_unmask_irq (uint8_t irq_level){
	uint8_t error = 0;

	if((irq_level > 0) && (irq_level < 16)) {
		leon3_clear_irq(irq_level); 			// Clear any pending IRQ of that level
		uint32_t bit = (1 << irq_level);
		*LEON3_IMASK = (*LEON3_IMASK | bit);
	} else {
		error = 1;
	}

	return error;
}

/* Pone a 0 el bit del "IRQ_LEVEL" indicado en el registro de mascara, enmascarando dicha interrupción. */
uint8_t leon3_mask_irq (uint8_t irq_level) {
	uint8_t error = 0;

	if(irq_level > 0 && irq_level < 16) {
		uint32_t bit =  ~(1 << irq_level);
		*LEON3_IMASK = (*LEON3_IMASK & bit);
	} else {
		error = 1;
	}

	return error;
}

/* Pone a 1 todos los bits del registro de mascara, desenmascarando todas las interrupciones externas */
void leon3_unmask_all_irqs(){
	*LEON3_IMASK=0xFFFE;
}

/* Pone a 0 todos los bits del registro de mascara, enmascarando todas las interrupciones externas */
void leon3_mask_all_irqs(){
	*LEON3_IMASK=0;
}
