/*
 *  Copyright 2014 by Wade T. Cline.
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

#include "board.h"


int board_done(struct board* board) {
	// TODO.
	return 0;
}

unsigned board_get_tiles_empty(struct board* board) {
	int i;
	int j;
	unsigned count;

	// Calculate the number of empty tiles on the board.
	count = 0;
	for (i = 0; i < BOARD_ROWS; i++) {
		for (j = 0; j < BOARD_COLUMNS; j++) {
			count += (board->tiles[i][j] == 0);
		}
	}

	// Return the calculated number.
	return count;
}

void board_init(struct board* board) {
	int i;
	int j;

	// Initialize each tile.
	for (i = 0; i < BOARD_ROWS; i++) {
		for (j = 0; j < BOARD_COLUMNS; j++) {
			board->tiles[i][j] = 0;
		}
	}

	// Add a tile to the board.
	board_plop(board);
}

void board_plop(struct board* board) {
	int i;
	int j;
	unsigned target;
	unsigned tiles_empty;

	// Count number of empty tiles.
	tiles_empty = board_get_tiles_empty(board);

	// Choose a random tile to palce the value into.
	target = random() % tiles_empty;

	// Place the value into the tile. Implemented poorly.
	tiles_empty = 0;
	for (i = 0; i < BOARD_ROWS; i++) {
		for (j = 0; j < BOARD_COLUMNS; j++) {
			if (target == tiles_empty && board->tiles[i][j] == 0) {
				board->tiles[i][j] = 2;
				tiles_empty++;
			} else {
				tiles_empty++;
			}
		}
	}
}

void board_print(struct board* board) {
	int i;
	int j;

	// Print the board to 'stdout'.
	for (i = 0; i < BOARD_ROWS; i++) {
		printf(" ");
		for (j = 0; j < BOARD_COLUMNS; j++) {
			printf("%4u ", board->tiles[i][j]);
		}
		printf("\n");
	}
}
