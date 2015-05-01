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

#include "io.h"

struct termios saved_term_settings;


void sigint_handler(int signum) {
	restore_mode();
	leave_alternate_buffer();
	exit(1);
}


void setup_signal_handlers() {
	struct sigaction sa;
	sa.sa_handler = sigint_handler;

	sigset_t mask;
	sigemptyset(&mask);
	sa.sa_mask = mask;

	sa.sa_flags = 0;

	sigaction(SIGINT, &sa, NULL);
}


void enter_alternate_buffer() {
	fputs("\33[?1049h", stdout);
}

void leave_alternate_buffer() {
	fputs("\33[?1049l", stdout);
}

void enter_format_mode(struct termios* term_settings) {
	if (!isatty(STDOUT_FILENO)) {
		fputs("Output is not a tty. Dying.\n", stderr);
		exit(1);
	}

	tcgetattr(STDOUT_FILENO, term_settings);

	saved_term_settings = *term_settings;

	term_settings->c_lflag &= ~(ICANON | ECHO);

	tcsetattr(STDOUT_FILENO, TCSADRAIN, term_settings);

	setvbuf(stdin, NULL, _IONBF, BUFSIZ);
	setvbuf(stdout, NULL, _IONBF, BUFSIZ);
}

void restore_mode() {
	tcsetattr(STDOUT_FILENO, TCSADRAIN, &saved_term_settings);
}
