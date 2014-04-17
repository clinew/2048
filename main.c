/*
 *  Copyright 2014 by Wade T. Cline
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define _POSIX_C_SOURCE 200112L

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#include "arguments.h"
#include "board.h"
#include "io.h"


#define VERSION "1.0.0"

char* legal =
	"2048 (implemented in C)  Copyright (C) 2014  Wade T. Cline\n"
	"This program comes with ABSOLUTELY NO WARRANTY. This is\n"
	"free software, and you are welcome to redistribute it\n"
	"under certain conditions. See the file 'COPYING' in the\n"
	"source code for details.\n";

/**
 * Print the usage message and exit failure.
 */
void usage_print(char* message) {
	// Print a specific error message.
	if (message) {
		fprintf(stderr, "ERROR: %s.\n", message);
	}
	
	// Print the generic usage message.
	fprintf(stderr, "\nUSAGE: 2048 [OPTIONS...]\n");
	fprintf(stderr, "      --help     Display this help text.\n");
	fprintf(stderr, "      --legal    Display legal information.\n");
	fprintf(stderr, "  -m, --mode     Select output mode.\n");
	fprintf(stderr, "  -s, --seed     Use specified seed for pseudo-random "
			"number generation.\n");
	fprintf(stderr, "      --version  Output version information and "
			"exit.\n");
	
	// Exit failure.
	if (message) {
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

/**
 * Get input from the user.
 */
char yoink(int format) {
	if (format) {
		return getchar();
	} else {
		char message[256];
		fgets(message, sizeof(message), stdin);
		return message[0];
	}
}

int main(int argc, char* argv[]) {
	struct arguments arguments;
	struct board board;
	int format;
	char input;
	char* message;
	int status; // Game status.
	struct termios term_settings;
	int valid;

	// Parse arguments.
	message = arguments_parse(&arguments, argc, argv);
	if (message) {
		usage_print(message);
	}

	// Apply arguments.
	valid = 1; // Hack; overload to determine whether to play or quit.
	if (arguments.flags & ARGUMENTS_VERSION) {
		printf("%s\n", VERSION);
		valid = 0;
	}
	if (arguments.flags & ARGUMENTS_LEGAL) {
		printf("%s\n", legal);
		valid = 0;
	}
	if (arguments.flags & ARGUMENTS_HELP) {
		usage_print(NULL);
	}
	if (!valid) {
		exit(EXIT_SUCCESS);
	}
	if (arguments.flags & ARGUMENTS_MODE) {
		if (arguments.mode == mode_format) {
			setup_signal_handlers();
			enter_alternate_buffer();
			enter_format_mode(&term_settings);
			format = 1;
		} else if (arguments.mode == mode_plain) {
			format = 0;
		}
	} else if (isatty(STDOUT_FILENO) && isatty(STDIN_FILENO)) {
		setup_signal_handlers();
		enter_alternate_buffer();
		enter_format_mode(&term_settings);
		format = 1;
	}
	if (arguments.flags & ARGUMENTS_SEED) {
		srand(arguments.seed);
	} else {
		srand(time(NULL));
	}

	// Set up the board.
	board_init(&board);
	if (!format) {
		fputs(legal, stdout);
	}

	// Play the game.
play:
	valid = 1;
	while (!(status = board_done(&board))) {
		// Set up screen for next move.
		if (format) {
			// Clear display and put cursor at 0,0.
			printf("\33[2J\33[H");

			// Print legal shenanigans.
			fputs(legal, stdout);
		}
		printf("\n");
		board_print(&board);
		printf("%s", valid ? (format ? "\n\n" : "") : "\nInvalid move.\n");
		printf("> ");
		fflush(stdout);

		// Get the player's move.
		input = yoink(format);

		// Process player's move.
		if (input == 'w' || input == 'k') {
			valid = board_move_up(&board);
		} else if (input == 's' || input == 'j') {
			valid = board_move_down(&board);
		} else if (input == 'a' || input == 'h') {
			valid = board_move_left(&board);
		} else if (input == 'd' || input == 'l') {
			valid = board_move_right(&board);
		} else if (input == 'n') {
			// Start a new game (or not) based on user input.
			printf("Start a new game? [y/N] ");
			input = yoink(format);
			if (input == 'y' || input == 'Y') {
				board_reset(&board);
			}
			continue;
		} else {
			valid = 0;
		}

		// End player's move.
		if (valid) {
			board_plop(&board);
		}
	}

	// Print the final board.
	printf("\nGame over, you %s!\n", (status < 0) ? "LOSE" : "WIN");

	// Check for new game.
	do {
		printf("\nPlay again? [y/n]");
	} while ((input = yoink(format)) != 'y' && input != 'Y' &&
		 input != 'n' && input != 'N');
	if (input == 'y' || input == 'Y') {
		board_reset(&board);
		goto play;
	}

	// Restore the terminal.
	if (format) {
		restore_mode();
		leave_alternate_buffer();
	}

	// Return success.
	return EXIT_SUCCESS;
}
