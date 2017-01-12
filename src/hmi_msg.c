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
#include "hmi_msg.h"

const char string_1[] PROGMEM = "January";
const char string_2[] PROGMEM = "February";
const char string_3[] PROGMEM = "March";
const char string_4[] PROGMEM = "April";
const char string_5[] PROGMEM = "May";
const char string_6[] PROGMEM = "June";


PGM_P const nameMonth[] PROGMEM = {
    string_1,
    string_2,
    string_3,
    string_4,
    string_5,
    string_6
};

const char help_cmd[] PROGMEM = HELP_CMD;
const char help_help[] PROGMEM = HELP_HELP;
const char ver_cmd[] PROGMEM = VER_CMD;
const char ver_help[] PROGMEM = VER_HELP;
const char ascii_cmd[] PROGMEM = ASCII_CMD;
const char ascii_help[] PROGMEM = ASCII_HELP;
const char month_cmd[] PROGMEM = MONTH_CMD;
const char month_help[] PROGMEM = MONTH_HELP;
const char rfid_read_cmd[] PROGMEM = RIF_READ_CMD;
const char rfid_read_help[] PROGMEM = RIF_READ_HELP;
const char rfid_add_cmd[] PROGMEM = RIF_ADD_CMD;
const char rfid_add_help[] PROGMEM = RIF_ADD_HELP;
const char rfid_remove_cmd[] PROGMEM = RIF_REMOVE_CMD;
const char rfid_remove_help[] PROGMEM = RIF_REMOVE_HELP;
const char rfid_print_cmd[] PROGMEM = RIF_PRINT_CMD;
const char rfid_print_help[] PROGMEM = RIF_PRINT_HELP;
const char rfid_mem_cmd[] PROGMEM = RIF_MEM_CMD;
const char rfid_mem_help[] PROGMEM = RIF_MEM_HELP;

