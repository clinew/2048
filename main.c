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

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#include "board.h"
#include "io.h"


#define print(x) fputs((x), stdout)


struct termios term_settings;


char* legal_shenanigans =
	"2048 (implemented in C)  Copyright (C) 2014  Wade T. Cline\n"
	"This program comes with ABSOLUTELY NO WARRANTY. This is\n"
	"free software, and you are welcome to redistribute it\n"
	"under certain conditions. See the file 'COPYING' in the\n"
	"source code for details.\n";

char* help_text =
	"Usage: ./2048 [OPTION]...\n"
	"Play 2048 from the command line.\n\n"
	"\t--raw\t\tIf supported, put terminal in raw mode and use the\n"
	"\t\t\talternate buffer\n"
	"\t--legal\t\tDisplay legal shenanigans\n"
	"\t--help\t\tDisplay this help text\n";


int handle_args(int argc, char* argv[]) {
	// Default to raw mode
	if (argc == 1)
		return isatty(STDIN_FILENO) && isatty(STDOUT_FILENO);

	if (strcmp(argv[1], "--no-raw") == 0)
		return 0;

	if (strcmp(argv[1], "--help") == 0) {
		print(help_text);
		exit(0);
	}

	if (strcmp(argv[1], "--legal") == 0) {
		print(legal_shenanigans);
		exit(0);
	}

	print("Option not recognized.\n");
	exit(1);
}


int main(int argc, char* argv[]) {
	int raw = handle_args(argc, argv);

	if (raw) {
		setup_signal_handlers();
		enter_alternate_buffer();
		enter_raw_mode(&term_settings);
	}

	struct board board;
	char c; // Command
	char input[1024]; // Input buffer
	int status; // Game status
	int valid;

	if (!raw) {
		// Print legal shenanigans.
		print(legal_shenanigans);
		print("\n\n");
	}

	// Set up random number generator.
	srand((int)time(NULL));

	// Set up board.
	board_init(&board);

	// Play the game.
	while (!(status = board_done(&board))) {
		if (raw) {
			// Clear display and put cursor at 0,0.
			print("\33[2J\33[H");

			// Print legal shenanigans.
			print(legal_shenanigans);
		} else {
			fputc('\n', stdout);
		}

		// Print the board.
		board_print(&board);

		if (!raw) {
			print("> ");
		}

		fflush(stdout);

		// Get the player's move.
		valid = 0;
		if (raw) {
			c = getchar();
		} else {
			fgets(input, 1024, stdin);
			c = input[0];
		}

		if (c == 'w' || c == 'k')
			valid = board_move_up(&board);
		else if (c == 's' || c == 'j')
			valid = board_move_down(&board);
		else if (c == 'a' || c == 'h')
			valid = board_move_left(&board);
		else if (c == 'd' || c == 'l')
			valid = board_move_right(&board);
		else
			valid = -1;

		if (!raw) {
			// Prepare for user's next move.
			if (valid == 0) {
				print("Invalid move.\n");
			} else if (valid == 1) {
				board_plop(&board);
			} else {
				print("Invalid command.\n");
			}
		} else {
			board_plop(&board);
		}
	}

	// Print the final board.
	printf("\nGame over, you %s!\n\n", (status < 0) ? "LOSE" : "WIN");
	if (raw) {
		print("Press space to exit.\n");
		while (getchar() != ' ') { /* wait */ }
		leave_alternate_buffer();
	}

	// Return success.
	return EXIT_SUCCESS;
}
