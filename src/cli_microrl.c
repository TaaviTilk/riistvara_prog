#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/andygock_avr-uart/uart.h"
#include "hmi_msg.h"
#include "print_helper.h"

#define NUM_ELEMS(x)        (sizeof(x) / sizeof((x)[0]))
#define UART_STATUS_MASK    0x00FF

void cli_print(const char * str);
char cli_get_char(void);
void cli_print_help(const char *const *argv);
void cli_print_ver(const char *const *argv);
void cli_print_ascii_tbls(const char *const *argv);
void cli_handle_month(const char *const *argv);
void cli_print_cmd_error(void);
void cli_print_cmd_arg_error(void);
int cli_execute(int argc, const char *const *argv);


typedef struct cli_cmd {
    PGM_P cmd;
    PGM_P help;
    void (*func_p)();
    const uint8_t func_argc;
} cli_cmd_t;


const char help_cmd[] PROGMEM = "help";
const char help_help[] PROGMEM = "Get help";
const char ver_cmd[] PROGMEM = "version";
const char ver_help[] PROGMEM = "Print FW version";
const char ascii_cmd[] PROGMEM = "ascii";
const char ascii_help[] PROGMEM = "print ASCII tables";
const char month_cmd[] PROGMEM = "month";
const char month_help[] PROGMEM =
    "Find matching month from lookup list. Usage: month <string>";


const cli_cmd_t cli_cmds[] = {
    {help_cmd, help_help, cli_print_help, 0},
    {ver_cmd, ver_help, cli_print_ver, 0},
    {ascii_cmd, ascii_help, cli_print_ascii_tbls, 0},
    {month_cmd, month_help, cli_handle_month, 1}
};


char cli_get_char(void)
{
    if (uart0_peek() != UART_NO_DATA) {
        return uart0_getc() & UART_STATUS_MASK;
    } else {
        return 0x00;
    }
}


void cli_print_help(const char *const *argv)
{
    (void) argv;
    uart0_puts_p(PSTR("\n\r"));
    uart0_puts_p(PSTR(CLI_HELP_MSG));

    for (uint8_t i = 0; i < NUM_ELEMS(cli_cmds); i++) {
        uart0_puts_p(cli_cmds[i].cmd);
        uart0_puts_p(PSTR(" : "));
        uart0_puts_p(cli_cmds[i].help);
        uart0_puts_p(PSTR("\n\r"));
    }
}


void cli_print_ver(const char *const *argv)
{
    (void) argv;
    uart0_puts_p(PSTR("\n\r"));
    uart0_puts_p(PSTR(VER_FW "\n\r"));
    uart0_puts_p(PSTR(VER_LIBC " " VER_GCC "\n\r"));
}


void cli_print_ascii_tbls(const char *const *argv)
{
    (void) argv;
    print_ascii_tbl (stdout);
    unsigned char ascii[128];

    for (unsigned char i = 0; i < sizeof(ascii); i++) {
        ascii[i] = i;
    }

    print_ascii_tbl();
    print_for_human (ascii, 128);
    uart0_puts("\n\r");
}


void cli_handle_month(const char *const *argv)
{
    (void) argv;
    uart0_puts("\n\r");
    lcd_clr(0x40, 16);
    lcd_goto(0x40);

    for (int i = 0; i < 6; i++) {
        if (!strncmp_P(argv[1], (PGM_P)pgm_read_word(&nameMonth[i]),
                       strlen(argv[1]))) {
            uart0_puts_p((PGM_P)pgm_read_word(&nameMonth[i]));
            uart0_puts("\n\r");
            lcd_puts_P((PGM_P)pgm_read_word(&nameMonth[i]));
            lcd_goto(0x48);
        }
    }
}


void cli_print_cmd_error(void)
{
    uart0_puts_p(PSTR("\n\r"));
    uart0_puts_p(PSTR("Command not implemented.\n\r"
                      "Use <help> to get help.\n\r"));
}


void cli_print_cmd_arg_error(void)
{
    uart0_puts_p(PSTR("\n\r"));
    uart0_puts_p(
        PSTR("To few or to many arguments for this command\n\rUse <help>\n\r"));
}


int cli_execute(int argc, const char *const *argv)
{
    for (uint8_t i = 0; i < NUM_ELEMS(cli_cmds); i++) {
        if (!strcmp_P(argv[0], cli_cmds[i].cmd)) {
            // Test do we have correct arguments to run command
            // Function arguments count shall be defined in struct
            if ((argc - 1) != cli_cmds[i].func_argc) {
                cli_print_cmd_arg_error();
                return 0;
            }

            // Hand argv over to function pointer,
            // cross fingers and hope that funcion handles it properly
            cli_cmds[i].func_p (argv);
            return 0;
        }
    }

    cli_print_cmd_error();
    return 0;
}
