#include <avr/pgmspace.h>
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>

#include "uart.h"
#include "print_helper.h"
#include "hmi_msg.h"
#include "../lib/hd44780_111/hd44780.h"


#define BLINK_DELAY_MS 100

void main (void)
{
    /* Set pin 3 of PORTA for output */
    DDRA |= _BV(DDA3);
    /* Init error console as stderr in UART3 and print user code info */
    uart0_initialize();
    uart3_initialize();
    lcd_init();
    lcd_clrscr();
    stdin = stdout = &uart0_io;
    stderr = &uart3_out;
    fprintf_P(stderr, PSTR(VER_FW),
              PSTR(GIT_DESCR), PSTR(__DATE__), PSTR(__TIME__));
    fprintf_P(stderr, PSTR(VER_LIBC), PSTR(__AVR_LIBC_VERSION_STRING__));
    /* End UART3 init and info print */
    fprintf_P(stdout, PSTR(STUD_NAME "\n"));
    lcd_puts_P(PSTR(STUD_NAME));
    /* prindib acii tabeli */
    print_ascii_tbl(stdout);
    unsigned char ascii[128] = {0};

    for (unsigned char i = 0; i < 128; i++) {
        ascii[i] = i;
    }

    print_for_human(stdout, ascii, 128);

    while (1) {
        /* Set pin 3 high to turn LED on */
        PORTA |= _BV(PORTA3);
        _delay_ms(BLINK_DELAY_MS);
        char inBuf = ' ';
        fprintf_P(stdout, PSTR(GET_MONTH));
        fscanf(stdin, "%c", &inBuf);
        fprintf(stdout, "%c\n", inBuf);
        lcd_goto(0x40);

        for (int i = 0; i < 6; i++) {

            if (!strncmp_P(&inBuf, (PGM_P)pgm_read_word(&nameMonth[i]), 1)) {
                fprintf_P(stdout, (PGM_P)pgm_read_word(&nameMonth[i]));
                fputc('\n', stdout);
                lcd_puts_P((PGM_P)pgm_read_word(&nameMonth[i]));
                lcd_putc(' ');
            }

        }

        for (int i = 0; i < 16; i++) {
            lcd_putc(' ');
        }

        /* Set pin 3 low to turn LED off */
        PORTA &= ~_BV(PORTA3);
        _delay_ms(BLINK_DELAY_MS);
    }
}
