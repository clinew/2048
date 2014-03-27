#ifndef io_H
#define io_H


#define _POSIX_C_SOURCE 1


#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>


/**
 * Puts the TTY into some kind of raw mode
 */
void enter_raw_mode(struct termios* term_settings);


/**
 * Sets up a SIGINT handler to switch back to the regular buffer.
 */
void setup_signal_handlers();

/**
 * Enters the alternate screen buffer.
 */
void enter_alternate_buffer();

/**
 * Returns to the regular screen buffer.
 */
void leave_alternate_buffer();


#endif // io_H
