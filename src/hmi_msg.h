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
#ifndef _HMI_MSG_H_
#define _HMI_MSG_H_

#include <avr/pgmspace.h>

#define LICENSE "RFID kaardilugeja.\r\n\
Copyright (C) 2017 Taavi Tilk.\r\n\
This program comes with ABSOLUTELY NO WARRANTY.\r\n"
#define HELLO "Use backspace to delete entry and enter to confirm.\r\n\
Arrow key's and del do not work currently.\r\n"

#define STUD_NAME "Taavi Tilk"
#define VER_LIBC "avr-libc version: "__AVR_LIBC_VERSION_STRING__
#define VER_GCC "avr-gcc version:"__VERSION__
#define VER_FW "Version: "GIT_DESCR" built on: "__DATE__" "__TIME__

#define GET_MONTH "Enter Month mane first letter >"
#define UPTIME "Uptime: %lu s\r\n"

#define CLI_HELP_MSG "Implemented commands:\r\n"

#define CARD_ID_MSG "The card ID: \""
#define IS_USED_USER_MSG "\" is used, username: \"%s\"\n\r"
#define NEW_CARD_MSG "Please use an other card"
#define CARD_OK_MSG "Card ID: OK"
#define USER_TO_BIG_MSG "username has to be less than 15 char"
#define USER_AND_ID_MSG "The username: \"%s\" is used, card ID: \""
#define SAME_NAME_MSG "\"\n\rPlease pick an other name."
#define USER_OK_MSG "Username: OK"
#define REMOV_USER_AND_ID_MSG "Removing user: \"%s\" with card ID: \""
#define REMOV_LIKEDL_MSG "\" from the linked-list\n\r"
#define NO_USER_MSG "No user with such name was found\n\r"
#define LIST_ERROR_MSG "\n\rLIST ERROR\n\r"
#define ADDING_CARD_MSG "\n\rAdding card to list\n\r"
#define NAME_ERROR_MSG "\n\rUSER NAME ERROR\n\r"
#define UNABLE_CARD_MSG "Unable to select card.\n\r"

#define CARD_SEL_MSG "Card selected!\n\r"
#define UID_SIZE_MSG "UID size: 0x%02X\n\r"
#define UID_SAK_MSG "UID sak: 0x%02X\n\r"

#define USER_MSG "\n\rUser:"

#define CMD_NOT_MSG "Command not implemented.\n\r Use <help> to get help.\n\r"
#define FEW_OR_MANY_MSG "To few or to many arguments for this command\n\rUse <help>\n\r"

#define HEAD_MSG "Heap statistics\r\n"
#define USED_MSG "Used: %d\r\n"
#define FREE_MSG "Free: %d\r\n"
#define HEAD2_MSG "\nSpace between stack and heap:\r\n"
#define CURRENT_MSG "Current  %d\r\n"
#define PREV_MSG "Previous %d\r\n"
#define CHANGE_MSG "Change   %d\r\n"
#define HEAD3_MSG "\r\nFreelist\r\n"
#define F_SIZE_MSG "Freelist size:              %d\r\n"
#define F_BLOCK_MSG "Blocks in freelist:         %d\r\n"
#define F_LARG_MSG "Largest block in freelist:  %d\r\n"
#define F_LARG_BLOCK_MSG "Largest freelist block:     %d\r\n"
#define L_BLOCK_MSG "Largest allocable block:    %d\r\n"

#define HELP_CMD "help"
#define HELP_HELP "Get help"
#define VER_CMD "version"
#define VER_HELP "Print FW version"
#define ASCII_CMD "ascii"
#define ASCII_HELP "print ASCII tables"
#define MONTH_CMD "month"
#define MONTH_HELP "Find matching month from lookup list. Usage: month <string>"
#define RIF_READ_CMD "read"
#define RIF_READ_HELP "Print RFID data and user(if it has one)"
#define RIF_ADD_CMD "add"
#define RIF_ADD_HELP "Add card to LL and give it a user. Usage: add username <string>(MAX length 15 char)"
#define RIF_REMOVE_CMD "remove"
#define RIF_REMOVE_HELP "Remove user/card from LL. Usage: remove username <string>"
#define RIF_PRINT_CMD "print"
#define RIF_PRINT_HELP "Print all used cards"
#define RIF_MEM_CMD "mem"
#define RIF_MEM_HELP "Memory statistics"

extern PGM_P const nameMonth[];

extern const char help_cmd[] PROGMEM;
extern const char help_help[] PROGMEM;
extern const char ver_cmd[] PROGMEM;
extern const char ver_help[] PROGMEM;
extern const char ascii_cmd[] PROGMEM;
extern const char ascii_help[] PROGMEM;
extern const char month_cmd[] PROGMEM;
extern const char month_help[] PROGMEM;
extern const char rfid_read_cmd[] PROGMEM;
extern const char rfid_read_help[] PROGMEM;
extern const char rfid_add_cmd[] PROGMEM;
extern const char rfid_add_help[] PROGMEM;
extern const char rfid_remove_cmd[] PROGMEM;
extern const char rfid_remove_help[] PROGMEM;
extern const char rfid_print_cmd[] PROGMEM;
extern const char rfid_print_help[] PROGMEM;
extern const char rfid_mem_cmd[] PROGMEM;
extern const char rfid_mem_help[] PROGMEM;

#endif /*_HMI_MSG_H_ endif on see*/
