#ifndef io_H
#define io_H


#define _POSIX_C_SOURCE 1


#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>


/**
 * Puts the TTY into some kind of raw mode; switches to the alternate buffer;
 * and sets up a SIGINT handler to switch back to the regular buffer.
 */
void initialize_tty(struct termios* term_settings);


#endif // io_H
