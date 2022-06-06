#include "leon3_uart.h"
#include "leon3_bprint.h"
int main() {
	char * pchar = "cadena\n";
	leon3_print_string(pchar);
	leon3_print_uint8(3);
	leon3_putchar('\n');
	leon3_print_uint8(43);
	leon3_putchar('\n');
	leon3_print_uint8(108);
	leon3_putchar('\n');
	return 0;
}
