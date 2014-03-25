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
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#include "arguments.h"
#include "board.h"
#include "io.h"


#define VERSION "1.0.0"


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
	fprintf(stderr, "  -s, --seed     Use specified seed for pseudo-random "
			"number generation.\n");
	fprintf(stderr, "      --version  Output version information and "
			"exit.\n");
	
	// Exit failure.
	exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
	struct arguments arguments;
	struct board board;
	char input;
	char* message;
	int status; // Game status.
	struct termios term_settings;
	int valid;

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

	initialize_tty(&term_settings);

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

		// Print the board.
		board_print(&board);

		// Get the player's move.
		valid = 0;
		input = getchar();
		if (input == 'w' || input == 'k')
			valid = board_move_up(&board);
		else if (input == 's' || input == 'j')
			valid = board_move_down(&board);
		else if (input == 'a' || input == 'h')
			valid = board_move_left(&board);
		else if (input == 'd' || input == 'l')
			valid = board_move_right(&board);
		else
			continue;

		// Prepare for user's next move.
		if (valid) {
			board_plop(&board);
		} else {
			printf("Invalid move.\n");
		}
	}

	// Print the final board.
	printf("\nGame over, you %s!\n\n", (status < 0) ? "LOSE" : "WIN");
	fputs("Press space to exit.\n", stdout);
	while (getchar() != ' ') { /* wait */ }
	fputs("\33[?1049l", stdout);

	// Return success.
	return EXIT_SUCCESS;
}
