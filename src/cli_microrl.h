#include "../lib/matejx_avr_lib/mfrc522.h"
#ifndef _CLI_MICRORL_H_
#define _CLI_MICRORL_H_


char cli_get_char(void);
void cli_print_help(const char *const *argv);
void cli_print_ver(const char *const *argv);
void cli_print_ascii_tbls(const char *const *argv);
void cli_handle_month(const char *const *argv);
void cli_rfid_read(const char *const *argv);
void cli_rfid_add(const char *const *argv);
void cli_rfid_remove(const char *const *argv);
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

