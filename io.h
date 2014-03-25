#define _POSIX_C_SOURCE 1

#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

void initialize_tty(struct termios* term_settings);
int get_token_utf8(uint8_t queue[]);
