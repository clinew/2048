/*
 *  Copyright 2014-2015 by Wade T. Cline (Frostsnow)
 *
 *  "2048" is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  "2048" is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with "2048".  If not, see <http://www.gnu.org/licenses/>.
 */

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
