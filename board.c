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
			} else if (board->tiles[i][j] == 0) {
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

void board_shift_up(struct board* board) {
	int i;
	int j;
	unsigned k;

	// Shift tiles up the columns.
	for (i = 0; i < BOARD_COLUMNS; i++) {
		// Find first free tile in the column.
		k = 0;
		while (k < BOARD_ROWS && board->tiles[k][i]) {
			k++;
		}

		// Shift tiles up the column.
		for (j = k + 1; j < BOARD_ROWS; j++) {
			if (board->tiles[j][i]) {
				board->tiles[k++][i] = board->tiles[j][i];
				board->tiles[j][i] = 0;
			}
		}
	}
}

void board_shift_down(struct board* board) {
	int i;
	int j;
	unsigned k;

	// Shift tiles down the columns.
	for (i = 0; i < BOARD_COLUMNS; i++) {
		// Find the last free tile in the column.
		k = BOARD_ROWS - 1;
		while (k >= 0 && board->tiles[k][i]) {
			k--;
		}
		
		// Shift tiles down the column.
		for (j = k - 1; j >= 0; j--) {
			if (board->tiles[j][i]) {
				board->tiles[k--][i] = board->tiles[j][i];
				board->tiles[j][i] = 0;
			}
		}
	}
}

void board_shift_left(struct board* board) {
	int i;
	int j;
	unsigned k;

	// Shift tiles left across the rows.
	for (i = 0; i < BOARD_ROWS; i++) {
		// Find the first free tile in the row.
		k = 0;
		while (k < BOARD_COLUMNS && board->tiles[i][k]) {
			k++;
		}

		// Shift tiles left across the row.
		for (j = k + 1; j < BOARD_COLUMNS; j++) {
			if (board->tiles[i][j]) {
				board->tiles[i][k++] = board->tiles[i][j];
				board->tiles[i][j] = 0;
			}
		}
	}
}

void board_shift_right(struct board* board) {
	int i;
	int j;
	unsigned k;

	// Shift tiles right across the rows.
	for (i = 0; i < BOARD_ROWS; i++) {
		// Find the last free tile in the row.
		k = BOARD_COLUMNS - 1;
		while (k >= 0 && board->tiles[i][k]) {
			k--;
		}

		// Shift tiles right across the row.
		for (j = k - 1; j >= 0; j--) {
			if (board->tiles[i][j]) {
				board->tiles[i][k--] = board->tiles[i][j];
				board->tiles[i][j] = 0;
			}
		}
	}
}
