#include <stdio.h>
#include <avr/pgmspace.h>
#include "../lib/andygock_avr-uart/uart.h"


void print_ascii_tbl ()
{
    uart0_puts("\n\r");
    char buffer[8];
    
    for (char c = ' '; c <= '~'; c++) {
        if (!sprintf(buffer, "%c ", c)) {
            return;
        }
        uart0_puts(buffer);
    }
    uart0_puts("\n\r");
}


void print_for_human (const unsigned char *array, const int len)
{
    char buffer[8];
    if (len > 1) {
        for (int i = 0; i < len; i++) {
            if (array[i] >= ' ' && array[i] <= '~') {
                if (!sprintf(buffer, "%c ", array[i])) {
                    return;
                }
            } else {
                if (!sprintf(buffer, "\"0x%02x\" ", array[i])) {
                    return;
                }
            }
            uart0_puts(buffer);
        }
    }
    return;
}


