/*   This file is part of Estionian ITC Course I237, where we use RFID on Arduino Mega 2560
 *
 *   Copyright (C) 2017 Taavi Tilk
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
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
//
uint8_t old_card[10];


// Create  microrl object  and pointer on  it
microrl_t rl;
microrl_t *prl = &rl;


// see on uardi initsaliseerimine
static inline void init_uart(void)
{
    DDRA |= _BV(DDA1);
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


// prindi Stud name ja teavitus tekst
static inline void init_hello(void)
{
    uart0_puts_p(PSTR(LICENSE));
    uart0_puts_p(PSTR(HELLO));
}

// see on microrl initsaliseerimine
static inline void init_rl(void)
{
    microrl_init (prl, uart0_puts);
    microrl_set_execute_callback (prl, cli_execute);
}


//RFID lugeja init
static inline void init_rfid_reader(void)
{
    /*Init RFID-RC522*/
    MFRC522_init();
    PCD_Init();
}


// see on counteri initsaliseerimine
static inline void init_counter(void)
{
    time = 0;
    TCCR1A = 0; // Clear control register A
    TCCR1B = 0; // Clear control register B
    TCCR1B |= _BV(WGM12); 
    TCCR1B |= _BV(CS12); // CTC and fCPU/256
    OCR1A = 62549; // 1 s
    TIMSK1 |= _BV(OCIE1A); // Output Compare A Match Interrupt Enable
}


// see on nime printimine
static inline void user(void)
{
    uart3_puts_p(PSTR(STUD_NAME"\r\n"));
    uart3_puts_p(PSTR(VER_FW"\r\n"));
    uart3_puts_p(PSTR(VER_LIBC" "VER_GCC"\r\n"));
}


// see on LCD ekraanile printimine
static inline void lcd_print(void)
{
    lcd_puts_P(PSTR(STUD_NAME));
    lcd_goto(0x40);
}


// see on LED ON-OFF
static inline void led_on_off(void)
{
    /* set pin 3 high to turn led on/off */
    PORTA ^= _BV(PORTA3);
}


// see on hearbeat - naitab, et probramm ei ole blokeerunud
static inline void heartbeat()
{
    static uint32_t last_time;
    char buffer[15];
    uint32_t cur_time;
    
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        cur_time = time;
    }

    if ((last_time - cur_time) > 0) {
        led_on_off();
        sprintf_P(buffer, PSTR(UPTIME), cur_time);
        uart3_puts(buffer);
    }

    last_time = cur_time;
}


//RFID lugemine
static inline void read_rfid(void)
{
    static uint32_t start;
    uint32_t cur_time;
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        cur_time = time;
    }

    //read RFID
    if (PICC_IsNewCardPresent()) {
        start = cur_time;
        Uid uid;
        Uid *uid_ptr = &uid;
        PICC_ReadCardSerial(uid_ptr);

        if (memcmp(uid.uidByte, old_card, uid.size) != 0) {
            lcd_clr(0x40, 16);
            lcd_goto(0x40);
            const char *user = get_username(uid.uidByte);

            if (user != NULL) {
                lcd_puts(user);
                //turn led in pin 23 ON
                PORTA |= _BV(PORTA1);
            } else {
                lcd_puts_P(PSTR("Access denied"));
                PORTA &= ~_BV(PORTA1);
            }

            for (byte i = 0; i < uid.size; i++) {
                old_card[i] = uid.uidByte[i];
            }
        }
    }

    //clear old_card arrey every 2 sec (prevents LCD from constantly printing 1 and the same line)
    if (cur_time - start == 2) {
        old_card[0] = '\0';
    }

    //turn led in pin 23 OFF after 3 sec
    if (cur_time - start == 3) {
        PORTA &= ~_BV(PORTA1);
    }

    //clear screen after 5 sec
    if (cur_time - start == 5) {
        lcd_clr(0x40, 16);
    }
}


void main (void)
{
    init_uart();
    init_lcd();
    init_rl();
    init_rfid_reader();
    init_counter();
    sei();
    init_hello();
    user();
    lcd_print();

    while (1) {
        microrl_insert_char(prl, cli_get_char());
        heartbeat();
        read_rfid();
    }
}


// timer
ISR(TIMER1_COMPA_vect)
{
    time++;
}
