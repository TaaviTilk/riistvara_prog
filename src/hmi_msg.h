#ifndef _HMI_MSG_H_
#define _HMI_MSG_H_
#include <avr/pgmspace.h>
#define STUD_NAME "Taavi Tilk"
#define VER_LIBC "avr-libc version: "__AVR_LIBC_VERSION_STRING__
#define VER_GCC "avr-gcc version:"__VERSION__
#define VER_FW "Version: " GIT_DESCR "-built on: " __DATE__" "__TIME__
#define GET_MONTH "Enter Month mane first letter >"
#define UPTIME "Uptime: %lu s\n\r"
#define CLI_HELP_MSG "Implemented commands:\n\r"

extern PGM_P const nameMonth[]; 

#endif /*_HMI_MSG_H_ endif on see*/
