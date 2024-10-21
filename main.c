/*
 *  Copyright 2014-2015, 2024 by Wade T. Cline (Frostsnow)
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

#include "include.h"

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


#define VERSION "2.0.1"

char* legal =
	"2048 (implemented in C)  Copyright (C) 2014-2015, 2024 Wade T. Cline\n"
	"This program comes with ABSOLUTELY NO WARRANTY. This is\n"
	"free software, and you are welcome to redistribute it\n"
	"under certain conditions. See the file 'COPYING' in the\n"
	"source code for details.\n";

/**
 * Print help message.
 */
void help_print() {
	printf("\nMovement Keys:\n");
	printf("  [wk]    Move tiles up.\n");
	printf("  [sj]    Move tiles down.\n");
	printf("  [ah]    Move tiles left.\n");
	printf("  [dl]    Move tiles right.\n");
	printf("\nGame Control:\n");
	printf("  Ctrl+C  Quit game (SIGNINT).\n");
	printf("  n       Start new game.\n");
	printf("  ?       Display this help text.\n");
}

/**
 * Print the game screen to 'stdout'.
 */
void screen_print(struct board* board, char* message, int format) {
	if (format) {
		// Clear display and put cursor at 0,0.
		printf("\33[2J\33[H");

		// Print legal shenanigans.
		fputs(legal, stdout);
	}
	printf("\n");
	board_print(board);
	if (message) {
		printf("%s", message);
	}
	printf("> ");
	fflush(stdout);
}

/**
 * Print the usage message and exit failure.
 */
void usage_print(char* message) {
	FILE* out;
	// Print a specific error message.
	if (message) {
		fprintf(stderr, "ERROR: %s.\n", message);
	}
	
	// Print the generic usage message.
	out = message ? stderr : stdout;
	fprintf(out, "\nUSAGE: 2048 [OPTIONS...]\n");
	fprintf(out, "      --help             Display this help text.\n");
	fprintf(out, "      --legal            Display legal information.\n");
	fprintf(out, "  -m, --mode <mode>      Select output mode.\n");
	fprintf(out, "  -s, --seed <unsigned>  Use specified seed for pseudo-random "
			"number generation.\n");
	fprintf(out, "      --version          Output version information and "
			"exit.\n\n");
	fprintf(out, "<mode> is one of:\n");
	fprintf(out, "\tformat - Dynamic input and pretty output.\n");
	fprintf(out, "\tplain - Line-by-line input and just dump output.\n");
	
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
	char buffer[256];
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
		// Sorry about this ugly call.
		screen_print(&board, valid ? (format ? "\n\n" : "") :
			"\nInvalid move.\n", format);

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
		} else if (input == '?') {
			help_print();
			if (format) {
				printf("\nPress any key to continue.");
				input = yoink(format);
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
	snprintf(buffer, sizeof(buffer),
		"\nGame over, you %s!\n\nPlay again? [y/n]\n",
		(status < 0) ? "LOSE" : "WIN");
	screen_print(&board, buffer, format);

	// Check for new game.
	while ((input = yoink(format)) != 'y' && input != 'Y' &&
		 input != 'n' && input != 'N') {
		 screen_print(&board, buffer, format);
	}
	if (input == 'y' || input == 'Y') {
		board_reset(&board);
		goto play;
	}

	// Restore the terminal.
	if (format) {
		restore_mode();
		leave_alternate_buffer();
	}

	// Free the board.
	board_free(&board);

	// Return success.
	return EXIT_SUCCESS;
}
