/*   This is Estionian ITC Course I237, where we use RFID on Arduino Mega 2560
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
#include <string.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/andygock_avr-uart/uart.h"
#include "../lib/matejx_avr_lib/mfrc522.h"
#include "../lib/andy_brown_memdebug/memdebug.h"
#include "hmi_msg.h"
#include "print_helper.h"
#include "cli_microrl.h"

#define NUM_ELEMS(x)        (sizeof(x) / sizeof((x)[0]))
#define UART_STATUS_MASK    0x00FF

void cli_print_help(const char *const *argv);
bool check_card_id(byte size, byte *uidByte);
bool check_user_name(const char *argv);
void cli_rfid_remove(const char *const *argv);
void cli_rfid_add(const char *const *argv);
void cli_rfid_print(void);
void cli_rfid_read(const char *const *argv);
void cli_print_ver(const char *const *argv);
void cli_print_ascii_tbls(const char *const *argv);
void cli_handle_month(const char *const *argv);
void cli_print_cmd_error(void);
void cli_print_cmd_arg_error(void);
void cli_mem_stat(const char *const *argv);


const cli_cmd_t cli_cmds[] = {
    {help_cmd, help_help, cli_print_help, 0},
    {ver_cmd, ver_help, cli_print_ver, 0},
    {ascii_cmd, ascii_help, cli_print_ascii_tbls, 0},
    {month_cmd, month_help, cli_handle_month, 1},
    {rfid_read_cmd, rfid_read_help, cli_rfid_read, 0},
    {rfid_add_cmd, rfid_add_help, cli_rfid_add, 1},
    {rfid_print_cmd, rfid_print_help, cli_rfid_print, 0},
    {rfid_remove_cmd, rfid_remove_help, cli_rfid_remove, 1},
    {rfid_mem_cmd, rfid_mem_help, cli_mem_stat, 0}
};


card_t *head_ptr = NULL;


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


//looks for a card and returns the card username
const char *get_username(byte *uidByte)
{
    card_t *current = head_ptr;

    while (current != NULL) {
        if (memcmp(uidByte, current->u_id, current->id_size) == 0) {
            return current->user;
        }

        current = current->next;
    }

    return NULL;
}


//check if the card ID is used
bool check_card_id(byte size, byte *uidByte)
{
    uart0_puts_p(PSTR("\n\r"));
    card_t *current = head_ptr;
    char buffer[30] = "";

    while (current != NULL) {
        if (memcmp(uidByte, current->u_id, size) == 0) {
            uart0_puts_p(PSTR(CARD_ID_MSG));

            for (byte i = 0; i < current->id_size; i++) {
                sprintf_P(buffer, PSTR("%02X"), current->u_id[i]);
                uart0_puts(buffer);
            }

            sprintf_P(buffer, PSTR(IS_USED_USER_MSG), current->user);
            uart0_puts(buffer);
            uart0_puts_p(PSTR(NEW_CARD_MSG));
            return 1;
        }

        current = current->next;
    }

    uart0_puts_p(PSTR(CARD_OK_MSG));
    return 0;
}


//check if a username is used
bool check_user_name(const char *argv)
{
    uart0_puts_p(PSTR("\n\r"));
    card_t *current = head_ptr;
    char buffer[30] = "";

    if (strlen(argv) > 15) {
        uart0_puts_p(PSTR(USER_TO_BIG_MSG));
        return 0;
    }

    while (current != NULL) {
        if (!strcmp(current->user, argv)) {
            sprintf_P(buffer, PSTR(USER_AND_ID_MSG), current->user);
            uart0_puts(buffer);

            for (byte i = 0; i < current->id_size; i++) {
                sprintf_P(buffer, PSTR("%02X"), current->u_id[i]);
                uart0_puts(buffer);
            }

            uart0_puts_p(PSTR(SAME_NAME_MSG));
            return 0;
        }

        current = current->next;
    }

    uart0_puts_p(PSTR(USER_OK_MSG));
    return 1;
}


//remove card and user from linked-list
void cli_rfid_remove(const char *const *argv)
{
    uart0_puts_p(PSTR("\n\r"));
    card_t *current = head_ptr;
    card_t *previous_user = NULL;
    char buffer[30] = "";
    int n = 0;

    while (current != NULL) {
        if (!strcmp(current->user, argv[1])) {
            sprintf_P(buffer, PSTR(REMOV_USER_AND_ID_MSG), current->user);
            uart0_puts(buffer);

            for (byte i = 0; i < current->id_size; i++) {
                sprintf_P(buffer, PSTR("%02X"), current->u_id[i]);
                uart0_puts(buffer);
            }

            sprintf_P(buffer, PSTR(REMOV_LIKEDL_MSG));
            uart0_puts(buffer);
            n = 1;

            if (previous_user == NULL) {
                head_ptr = current->next;
            } else {
                previous_user->next = current->next;
            }

            free(current->user);
            free(current);
        }

        previous_user = current;
        current = current->next;
    }

    if (n == 0) {
        sprintf_P(buffer, PSTR(NO_USER_MSG));
        uart0_puts(buffer);
    }
}


//add RFID to linkde-list
void cli_rfid_add(const char *const *argv)
{
    Uid uid;
    Uid *uid_ptr = &uid;

    if (PICC_IsNewCardPresent()) {
        card_t *new_head;
        new_head = malloc(sizeof(card_t));

        if (new_head == NULL) {
            uart0_puts_p(PSTR(LIST_ERROR_MSG));
        } else {
            PICC_ReadCardSerial(uid_ptr);
            //check if username or card ID is used
            bool b = check_card_id(uid.size, uid.uidByte);
            bool a = check_user_name(argv[1]);

            if (a && !b) {
                uart0_puts_p(PSTR(ADDING_CARD_MSG));
                new_head->id_size = uid.size;

                for (byte i = 0; i < uid.size; i++) {
                    new_head->u_id[i] = uid.uidByte[i];
                }

                new_head->user = malloc(sizeof(argv[1]) + 1);

                if (new_head->user == NULL) {
                    uart0_puts_p(PSTR(NAME_ERROR_MSG));
                } else {
                    new_head->user = strdup(argv[1]);
                    new_head->next = head_ptr;
                    head_ptr = new_head;
                }
            } else {
                uart0_puts_p(PSTR("\n\r"));
            }
        }
    } else {
        uart0_puts_p((PSTR(UNABLE_CARD_MSG)));
    }
}


//print linked-list
void cli_rfid_print(void)
{
    char buffer[30] = "";
    int n = 0;
    card_t *current = head_ptr;
    uart0_puts_p(PSTR("\n\r"));

    while (current != NULL) {
        n++;
        sprintf_P(buffer, PSTR("%i. "), n);
        uart0_puts(buffer);

        for (byte i = 0; i < current->id_size; i++) {
            sprintf_P(buffer, PSTR("%02X"), current->u_id[i]);
            uart0_puts(buffer);
        }

        uart0_puts(" ");
        uart0_puts(current->user);
        current = current->next;
        uart0_puts_p(PSTR("\n\r"));
    }
}


//RFID read.
void cli_rfid_read(const char *const *argv)
{
    (void)argv;
    char buffer[20] = "";
    Uid uid;
    Uid *uid_ptr = &uid;
    uart0_puts_p(PSTR("\n\r"));

    if (PICC_IsNewCardPresent()) {
        uart0_puts_p(PSTR(CARD_SEL_MSG));
        PICC_ReadCardSerial(uid_ptr);
        sprintf_P(buffer, PSTR(UID_SIZE_MSG), uid.size);
        uart0_puts(buffer);
        sprintf_P(buffer, PSTR(UID_SAK_MSG), uid.sak);
        uart0_puts(buffer);
        uart0_puts_p(PSTR(CARD_ID_MSG));

        for (byte i = 0; i < uid.size; i++) {
            sprintf_P(buffer, PSTR("%02X"), uid.uidByte[i]);
            uart0_puts(buffer);
        }

        const char *user = get_username(uid.uidByte);

        if (user != NULL) {
            uart0_puts_p(PSTR(USER_MSG));
            uart0_puts_p(user);
        }

        uart0_puts_p(PSTR("\n\r"));
    } else {
        uart0_puts_p((PSTR(UNABLE_CARD_MSG)));
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
    print_ascii_tbl(stdout);
    unsigned char ascii[128];

    for (unsigned char i = 0; i < sizeof(ascii); i++) {
        ascii[i] = i;
    }

    print_ascii_tbl();
    print_for_human(ascii, 128);
    uart0_puts_p(PSTR("\n\r"));
}


void cli_handle_month(const char *const *argv)
{
    (void) argv;
    uart0_puts_p(PSTR("\n\r"));
    lcd_clr(0x40, 16);
    lcd_goto(0x40);

    for (int i = 0; i < 6; i++) {
        if (!strncmp_P(argv[1], (PGM_P)pgm_read_word(&nameMonth[i]),
                       strlen(argv[1]))) {
            uart0_puts_p((PGM_P)pgm_read_word(&nameMonth[i]));
            uart0_puts_p(PSTR("\n\r"));
            lcd_puts_P((PGM_P)pgm_read_word(&nameMonth[i]));
            lcd_goto(0x48);
        }
    }
}


void cli_print_cmd_error(void)
{
    uart0_puts_p(PSTR("\n\r"));
    uart0_puts_p(PSTR(CMD_NOT_MSG));
}


void cli_print_cmd_arg_error(void)
{
    uart0_puts_p(PSTR("\n\r"));
    uart0_puts_p(
        PSTR(FEW_OR_MANY_MSG));
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


void cli_mem_stat(const char *const *argv)
{
    (void) argv;
    char buffer[128];
    extern int __heap_start, *__brkval;
    int v;
    int space;
    static int prev_space;
    space = (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
    uart0_puts("\n\r");
    uart0_puts_p(PSTR(HEAD_MSG));
    sprintf_P(buffer, PSTR(USED_MSG), getMemoryUsed());
    uart0_puts(buffer);
    sprintf_P(buffer, PSTR(FREE_MSG), getFreeMemory());
    uart0_puts(buffer);
    uart0_puts_p(PSTR(HEAD2_MSG));
    sprintf_P(buffer, PSTR(CURRENT_MSG), space);
    uart0_puts(buffer);
    sprintf_P(buffer, PSTR(PREV_MSG), prev_space);
    uart0_puts(buffer);
    sprintf_P(buffer, PSTR(CHANGE_MSG), space - prev_space);
    uart0_puts(buffer);
    uart0_puts_p(PSTR(HEAD3_MSG));
    sprintf_P(buffer, PSTR(F_SIZE_MSG), getFreeListSize());
    uart0_puts(buffer);
    sprintf_P(buffer, PSTR(F_BLOCK_MSG), getNumberOfBlocksInFreeList());
    uart0_puts(buffer);
    sprintf_P(buffer, PSTR(F_LARG_MSG), getLargestBlockInFreeList());
    uart0_puts(buffer);
    sprintf_P(buffer, PSTR(F_LARG_BLOCK_MSG), getLargestAvailableMemoryBlock());
    uart0_puts(buffer);
    sprintf_P(buffer, PSTR(L_BLOCK_MSG), getLargestNonFreeListBlock());
    uart0_puts(buffer);
    uart0_puts("\n\r");
    prev_space = space;
}
