#ifndef io_H
#define io_H

#include "include.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>


/**
 * Puts the terminal into some kind of formatting mode.
 */
void enter_format_mode(struct termios* term_settings);

/**
 * Restores the original terminal mode.
 */
void restore_mode();

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
