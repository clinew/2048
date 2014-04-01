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

char* legal_shenanigans =
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

int handle_args(int argc, char* argv[]) {
	// Default to raw mode
	if (argc == 1)
		return isatty(STDIN_FILENO) && isatty(STDOUT_FILENO);

	if (strcmp(argv[1], "--no-raw") == 0)
		return 0;

	if (strcmp(argv[1], "--help") == 0) {
		usage_print(NULL);
	}

	if (strcmp(argv[1], "--legal") == 0) {
		fputs(legal_shenanigans, stdout);
		exit(EXIT_SUCCESS);
	}

	if (strcmp(argv[1], "--raw") == 0)
		return 1;

	fputs("Option not recognized.\n", stderr);
	exit(1);
}


int main(int argc, char* argv[]) {
	struct arguments arguments;
	struct board board;
	char input;
	char* message;
	int raw;
	int status; // Game status.
	struct termios term_settings;
	int valid;

	raw = handle_args(argc, argv);
	if (raw) {
		setup_signal_handlers();
		enter_alternate_buffer();
		enter_raw_mode(&term_settings);
	}

	// Print legal shenanigains.
	printf("\t2048 (implemented in C)  Copyright (C) 2014  Wade T. Cline\n"
	       "\tThis program comes with ABSOLUTELY NO WARRANTY. This is\n"
	       "\tfree software, and you are welcome to redistribute it\n"
	       "\tunder certain conditions. See the file 'COPYING' in the\n"
	       "\tsource code for details.\n\n");
	
	// Parse arguments.
	message = arguments_parse(&arguments, argc, argv);
	if (message) {
		usage_print(message);
	}

	// Apply arguments.
	if (arguments.flags & ARGUMENTS_VERSION) {
		printf("%s\n", VERSION);
		exit(EXIT_SUCCESS);
	}
	if (arguments.flags & ARGUMENTS_SEED) {
		srand(arguments.seed);
	} else {
		srand(time(NULL));
	}

	if (!raw) {
		// Print legal shenanigans.
		fputs(legal_shenanigans, stdout);
		fputs("\n\n", stdout);
	}

	// Set up board.
	board_init(&board);

	// Play the game.
	while (!(status = board_done(&board))) {
		// Print legal shenanigains.
		fputs("\33[2J", stdout); // clear display
		fputs("\33[H", stdout); // put cursor at 0,0
		printf("\t2048 (implemented in C)  Copyright (C) 2014  Wade T. Cline\n"
		       "\tThis program comes with ABSOLUTELY NO WARRANTY. This is\n"
		       "\tfree software, and you are welcome to redistribute it\n"
		       "\tunder certain conditions. See the file 'COPYING' in the\n"
		       "\tsource code for details.\n\n");
		if (raw) {
			// Clear display and put cursor at 0,0.
			fputs("\33[2J\33[H", stdout);

			// Print legal shenanigans.
			fputs(legal_shenanigans, stdout);
		} else {
			fputc('\n', stdout);
		}

		// Print the board.
		board_print(&board);

		if (!raw) {
			fputs("> ", stdout);
		}

		fflush(stdout);

		// Get the player's move.
		valid = 0;
		if (raw) {
			input = getchar();
		} else {
			char raw_input[1024];
			fgets(raw_input, sizeof(raw_input), stdin);
			input = raw_input[0];
		}

		if (input == 'w' || input == 'k')
			valid = board_move_up(&board);
		else if (input == 's' || input == 'j')
			valid = board_move_down(&board);
		else if (input == 'a' || input == 'h')
			valid = board_move_left(&board);
		else if (input == 'd' || input == 'l')
			valid = board_move_right(&board);
		else
			valid = 0;

		if (valid) {
			board_plop(&board);
		} else {
			printf("Invalid move.\n");
		}
	}

	// Print the final board.
	printf("\nGame over, you %s!\n\n", (status < 0) ? "LOSE" : "WIN");
	if (raw) {
		fputs("Press space to exit.\n", stdout);
		while (getchar() != ' ') { /* wait */ }
		restore_mode();
		leave_alternate_buffer();
	}

	// Return success.
	return EXIT_SUCCESS;
}
