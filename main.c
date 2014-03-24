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

#include <string.h>
#include <unistd.h>

#include "board.h"


int main(int argc, char* argv[]) {
	struct board board;
	char input[1024];
	int status; // Game status.
	int valid;

	// Set up board.
	board_init(&board);
	
	// Play the game.
	while (!(status = board_done(&board))) {
		// Print the board.
		board_print(&board);

		// Get the player's move.
		valid = 0;
		memset((void*)input, 0, sizeof(input));
		write(STDOUT_FILENO, (void*)"> ", 3);
		if (read(STDIN_FILENO, (void*)input, sizeof(input) - 1)
		    == -1) {
			perror("Error reading input");
			break;
		}
		input[strlen(input) - 1] = 0;
		if (!strcmp(input, "u") || !strcmp(input, "up")) {
			valid = board_shift_up(&board);
		} else if (!strcmp(input, "d") || !strcmp(input, "down")) {
			valid = board_shift_down(&board);
		} else if (!strcmp(input, "l") || !strcmp(input, "left")) {
			valid = board_shift_left(&board);
		} else if (!strcmp(input, "r") || !strcmp(input, "right")) {
			valid = board_shift_right(&board);
		} else {
			printf("Don't understand input: %s.\n", input);
			continue;
		}

		// Prepare for user's next move.
		if (valid) {
			board_plop(&board);
		} else {
			printf("Invalid move.\n");
		}
	}
	
	// Print the final board.
	printf("Game over, you ");
	if (status < 0) {
		printf("LOSE");
	} else {
		printf("WIN");
	}
	printf("!\n");
	board_print(&board);
}
