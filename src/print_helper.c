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
#include <avr/pgmspace.h>
#include "../lib/andygock_avr-uart/uart.h"


void print_ascii_tbl ()
{
    uart0_puts_p(PSTR("\n\r"));
    char buffer[8];

    for (char c = ' '; c <= '~'; c++) {
        if (!sprintf_p(buffer, PSTR("%c "), c)) {
            return;
        }

        uart0_puts(buffer);
    }

    uart0_puts_p(PSTR("\n\r"));
}


void print_for_human (const unsigned char *array, const int len)
{
    char buffer[8];

    if (len > 1) {
        for (int i = 0; i < len; i++) {
            if (array[i] >= ' ' && array[i] <= '~') {
                if (!sprintf_P(buffer, PSTR("%c "), array[i])) {
                    return;
                }
            } else {
                if (!sprintf_P(buffer, PSTR("\"0x%02x\" "), array[i])) {
                    return;
                }
            }

            uart0_puts(buffer);
        }
    }

    return;
}