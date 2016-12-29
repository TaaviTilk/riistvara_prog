#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <string.h>
#include <util/delay.h>
#include <util/atomic.h>
#include "print_helper.h"
#include "hmi_msg.h"
#include "cli_microrl.h"
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/andygock_avr-uart/uart.h"
#include "../lib/helius_microrl/microrl.h"
#define BAUD 9600

volatile uint32_t time;

// Create  microrl object  and pointer on  it
microrl_t rl;
microrl_t *prl = &rl;


// see on uardi initsaliseerimine
static inline void init_uart(void)
{
    DDRA |= _BV(DDA3);
    uart0_init(UART_BAUD_SELECT(BAUD, F_CPU));
    uart3_init(UART_BAUD_SELECT(BAUD, F_CPU));
}


// see on LCD initsaliseerimine
static inline void init_lcd(void)
{
    lcd_init();
    lcd_clrscr();
}


// see on  microrl initsaliseerimine
static inline void init_rl(void)
{
    microrl_init (prl, uart0_puts);
    microrl_set_execute_callback (prl, cli_execute);
}


// see on  counteri initsaliseerimine
static inline void init_counter(void)
{
    TCCR5A = 0; // Clear control register A
    TCCR5B = 0; // Clear control register B
    TCCR5B |= _BV(WGM52) | _BV(CS52); // CTC and fCPU/256
    OCR5A = 62549; // 1 s
    TIMSK5 |= _BV(OCIE5A); // Output Compare A Match Interrupt Enable
}


// see on  nime printimine
static inline void user(void)
{
    uart3_puts_p(PSTR(STUD_NAME"\n\r"));
    uart3_puts_p(PSTR(VER_FW "\n\r"));
    uart3_puts_p(PSTR(VER_LIBC " " VER_GCC "\n\r"));
}


// see on  LCD ekraanile printimine
static inline void lcd_print(void)
{
    lcd_puts_P(PSTR(STUD_NAME));
    lcd_goto(0x40);
}


// see on  LED ON-OFF
static inline void led_on_off(void)
{
    /* set pin 3 high to turn led on/off */
    PORTA ^= _BV(PORTA3);
}


// see on hearbeat - naitab, et probramm ei ole blokeerunud
static inline void heartbeat()
{
    static uint32_t last_time;
    uint32_t cur_time;
    char buffer[15];
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        cur_time = time;
    }

    if ((last_time - cur_time) > 0) {
        led_on_off();
        sprintf_P(buffer, PSTR(UPTIME) , cur_time);
        uart3_puts(buffer);
    }

    last_time = cur_time;
}


void main (void)
{
    init_uart();
    init_lcd();
    sei();
    init_rl();
    init_counter();
    user();
    lcd_print();

    while (1) {
        microrl_insert_char(prl, cli_get_char());
        heartbeat();
    }
}

// timer
ISR(TIMER5_COMPA_vect)
{
    time++;
}
