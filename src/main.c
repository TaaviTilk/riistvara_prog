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

// see on  microrl initsaliseerimine
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
    TCCR5A = 0; // Clear control register A
    TCCR5B = 0; // Clear control register B
    TCCR5B |= _BV(WGM52) | _BV(CS52); // CTC and fCPU/256
    OCR5A = 62549; // 1 s
    TIMSK5 |= _BV(OCIE5A); // Output Compare A Match Interrupt Enable
}


// see on nime printimine
static inline void user(void)
{
    uart3_puts_p(PSTR(STUD_NAME"\n\r"));
    uart3_puts_p(PSTR(VER_FW "\n\r"));
    uart3_puts_p(PSTR(VER_LIBC " " VER_GCC "\n\r"));
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
                lcd_puts("Access denied");
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
    sei();
    init_hello();
    init_rl();
    init_rfid_reader();
    init_counter();
    user();
    lcd_print();

    while (1) {
        microrl_insert_char(prl, cli_get_char());
        read_rfid();
        heartbeat();
    }
}


// timer
ISR(TIMER5_COMPA_vect)
{
    time++;
}
