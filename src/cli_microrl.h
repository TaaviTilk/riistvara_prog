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
#include "../lib/matejx_avr_lib/mfrc522.h"
#ifndef _CLI_MICRORL_H_
#define _CLI_MICRORL_H_

char cli_get_char(void);
int cli_execute(int argc, const char *const *argv);
const char *get_username(byte *uidByte);


typedef struct cli_cmd {
    PGM_P cmd;
    PGM_P help;
    void (*func_p)();
    const uint8_t func_argc;
} cli_cmd_t;


//card link-list
typedef struct card {
    uint8_t u_id[10];
    uint8_t id_size;
    char *user;
    struct card *next;
} card_t;

#endif

