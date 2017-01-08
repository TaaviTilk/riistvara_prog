#ifndef _HMI_MSG_H_
#define _HMI_MSG_H_
#include <avr/pgmspace.h>
#define STUD_NAME "Taavi Tilk"
#define VER_LIBC "avr-libc version: "__AVR_LIBC_VERSION_STRING__
#define VER_GCC "avr-gcc version:"__VERSION__
#define VER_FW "Version: "GIT_DESCR" built on: "__DATE__" "__TIME__
#define GET_MONTH "Enter Month mane first letter >"
#define UPTIME "Uptime: %lu s\r\n"
#define CLI_HELP_MSG "Implemented commands:\r\n"

#define LICENSE "RFID kaardilugeja.\r\n\
Copyright (C) 2017 Taavi Tilk.\r\n\
This program comes with ABSOLUTELY NO WARRANTY.\r\n"
#define HELLO "Use backspace to delete entry and enter to confirm.\r\n\
Arrow key's and del do not work currently.\r\n"

extern PGM_P const nameMonth[]; 

#endif /*_HMI_MSG_H_ endif on see*/
