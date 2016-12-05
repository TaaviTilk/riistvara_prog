#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <util/atomic.h>
#include "uart-wrapper.h"
#include "print_helper.h"
#include "hmi_msg.h"
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/andygock_avr-uart/uart.h"
#define BLINK_DELAY_MS 100
#define BAUD 9600

volatile uint32_t time;

static inline void init_system_clock (void)
{
    TCCR5A = 0; // Clear control register A
    TCCR5B = 0; // Clear control register B
    TCCR5B |= _BV(WGM52) | _BV(CS52); // CTC and fCPU/256
    OCR5A = 62549; // 1 s
    TIMSK5 |= _BV(OCIE5A); // Output Compare A Match Interrupt Enable
}

static inline void hw_init()
{
    DDRA |= _BV(DDA3); //PIN 25
    uart0_init(UART_BAUD_SELECT(BAUD, F_CPU));
    uart3_init(UART_BAUD_SELECT(BAUD, F_CPU));
    init_system_clock ();
    sei();
    stdin = stdout = &uart0_io;
    stderr = &uart3_out;
    lcd_init();
    lcd_clrscr();
}




static inline void print_version()
{
    fprintf_P(stderr, PSTR(VER_FW),
              PSTR(GIT_DESCR), PSTR(__DATE__), PSTR(__TIME__));
    fprintf_P(stderr, PSTR(VER_LIBC), PSTR(__AVR_LIBC_VERSION_STRING__));
}


static inline void print_algus()
{
    fprintf_P(stdout, PSTR(STUD_NAME "\n"));
    lcd_puts_P(PSTR(STUD_NAME));
    print_ascii_tbl(stdout);
    unsigned char ascii[128] = {0};

    for (unsigned char i = 0; i < 128; i++) {
        ascii[i] = i;
    }

    print_for_human(stdout, ascii, 128);
    fprintf_P(stdout, PSTR(GET_MONTH));
}

static inline void search_month()
{
    char inBuf = ' ';
    fscanf(stdin, "%c", &inBuf);
    fprintf(stdout, "%c\n", inBuf);
    lcd_goto(0x40);
    char print_spaces = 16;

    for (int i = 0; i < 6; i++) {
        if (!strncmp_P(&inBuf, (PGM_P)pgm_read_word(&nameMonth[i]), 1)) {
            char print_c;
            print_c = fprintf_P(stdout, (PGM_P)pgm_read_word(&nameMonth[i]));
            fputc('\n', stdout);
            lcd_puts_P((PGM_P)pgm_read_word(&nameMonth[i]));
            lcd_putc(' ');
            print_spaces -= (print_c + 1);
        }
    }

    while (print_spaces > -1) {
        lcd_putc(' ');
        print_spaces--;
    }

    fprintf_P(stdout, PSTR(GET_MONTH));
}


static inline void heartbeat()
{
    static uint32_t last_time;
    uint32_t cur_time;
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        cur_time = time;
    }

    if ((last_time - cur_time) > 0) {
        // Toggle led on arduino pin 25
        PORTA ^= _BV(PORTA3);
        fprintf_P(stderr, PSTR(UPTIME "\n"), cur_time);
    }

    last_time = cur_time;
}


void main (void)
{
    hw_init();
    print_version();
    print_algus();

    while (1) {
        heartbeat();

        if (uart0_available()) {
            search_month();
        }
    }
}

// System clock
ISR(TIMER5_COMPA_vect)
{
    time++;
}
