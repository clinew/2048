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
	int i;
	int j;
	int k;

	// Check for zeroes or winning number.
	for (i = 0; i < BOARD_ROWS; i++) {
		for (j = 0; j < BOARD_COLUMNS; j++) {
			if (!board->tiles[i][j]) {
				return 0;
			} else if (board->tiles[i][j] == 2048) {
				return 1;
			}
		}
	}

	// Check for possible horizontal merge.
	for (i = 0; i < BOARD_ROWS; i++) {
		j = -1;
		while ((k = ++j + 1) < BOARD_COLUMNS) {
			if (board->tiles[i][j] == board->tiles[i][k]) {
				return 0;
			}
		}
	}

	// Check for possible verical merge.
	for (i = 0; i < BOARD_COLUMNS; i++) {
		j = -1;
		while ((k = ++j + 1) < BOARD_ROWS) {
			if (board->tiles[j][i] == board->tiles[k][i]) {
				return 0;
			}
		}
	}

	// No possible merges.
	return -1;
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

	// Add two tiles to the board.
	board_plop(board);
	board_plop(board);
}

int board_merge_down(struct board* board) {
	int i;
	int j;
	int k;
	int merge;

	// Merge elements downwards.
	merge = 0;
	for (i = 0; i < BOARD_COLUMNS; i++) {
		j = BOARD_ROWS - 1;
		while (1) {
			// Find two potential elements to merge.
			while (j >= 0 && !board->tiles[j][i]) {
				j--;
			}
			k = j - 1;
			while (k >= 0 && !board->tiles[k][i]) {
				k--;
			}
			if (k < 0) {
				break;
			}

			// Try to merge the tiles.
			if (board->tiles[j][i] == board->tiles[k][i]) {
				board->tiles[k][i] += board->tiles[k][i];
				board->tiles[j][i] = 0;
				j = k - 1;
				merge = 1;
			} else {
				j = k;
			}
		}
		
	}

	// Return whether an item was merged or not.
	return merge;
}

int board_merge_left(struct board* board) {
	int i;
	int j;
	int k;
	int merge;

	// Merge items leftwards.
	merge = 0;
	for (i = 0; i < BOARD_ROWS; i++) {
		j = 0;
		while (1) {
			// Two two potential tiles to merge.
			while (j < BOARD_COLUMNS && !board->tiles[i][j]) {
				j++;
			}
			k = j + 1;
			while (k < BOARD_COLUMNS && !board->tiles[i][k]) {
				k++;
			}
			if (k >= BOARD_COLUMNS) {
				break;
			}

			// Try to merge the tiles.
			if (board->tiles[i][j] == board->tiles[i][k]) {
				board->tiles[i][j] += board->tiles[i][j];
				board->tiles[i][k] = 0;
				j = k + 1;
				merge = 1;
			} else {
				j = k;
			}
		}
	}

	// Return whether a tile was merged or not.
	return merge;
}

int board_merge_right(struct board* board) {
	int i;
	int j;
	int k;
	int merge;

	// Merge items rightward.
	merge = 0;
	for (i = 0; i < BOARD_ROWS; i++) {
		j = BOARD_ROWS - 1;
		while (1) {
			// Find potential tiles to merge.
			while (j >= 0 && !board->tiles[i][j]) {
				j--;
			}
			k = j - 1;
			while (k >= 0 && !board->tiles[i][k]) {
				k--;
			}
			if (k < 0) {
				break;
			}

			// Try to merge the tiles.
			if (board->tiles[i][j] == board->tiles[i][k]) {
				board->tiles[i][k] += board->tiles[i][k];
				board->tiles[i][j] = 0;
				j = k - 1;
				merge = 1;
			} else {
				j = k;
			}
		}
	}

	// Return whether a tile was merged or not.
	return merge;
}

int board_merge_up(struct board* board) {
	int i;
	int j;
	int k;
	int merge;

	// Merge elements upwards.
	merge = 0;
	for (i = 0; i < BOARD_COLUMNS; i++) {
		j = 0;
		while (1) {
			// Find two potential tiles to merge.
			while (j < BOARD_ROWS && !board->tiles[j][i]) {
				j++;
			}
			k = j + 1;
			while (k < BOARD_ROWS && !board->tiles[k][i]) {
				k++;
			}
			if (k >= BOARD_ROWS) {
				break;
			}

			// Try to merge the tiles.
			if (board->tiles[j][i] == board->tiles[k][i]) {
				board->tiles[j][i] += board->tiles[j][i];
				board->tiles[k][i] = 0;
				j = k + 1;
				merge = 1;
			} else {
				j = k;
			}
		}
	}

	// Return whether an item was merged or not.
	return merge;
}

int board_move_down(struct board* board) {
	return board_merge_down(board) | board_shift_down(board);
}

int board_move_left(struct board* board) {
	return board_merge_left(board) | board_shift_left(board);
}

int board_move_right(struct board* board) {
	return board_merge_right(board) | board_shift_right(board);
}

int board_move_up(struct board* board) {
	return board_merge_up(board) | board_shift_up(board);
}

void board_plop(struct board* board) {
	int i;
	int j;
	unsigned target;
	unsigned tiles_empty;

	// Count number of empty tiles.
	tiles_empty = board_get_tiles_empty(board);

	// Choose a random tile to palce the value into.
	target = rand() % tiles_empty;

	// Place the value into the tile. Implemented poorly.
	tiles_empty = 0;
	for (i = 0; i < BOARD_ROWS; i++) {
		for (j = 0; j < BOARD_COLUMNS; j++) {
			if (target == tiles_empty && board->tiles[i][j] == 0) {
				board->tiles[i][j] = (rand() % 100 <=
						BOARD_4SPAWN_CHANCE) ? 4 : 2;
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
			if (board->tiles[i][j]) {
				printf("%4u ", board->tiles[i][j]);
			} else {
				printf("   . ");
			}
		}
		printf("\n");
	}
}

int board_shift_up(struct board* board) {
	int i;
	int j;
	unsigned k;
	int valid;

	// Shift tiles up the columns.
	valid = 0;
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
				valid = 1;
			}
		}
	}

	// Return move status.
	return valid;
}

int board_shift_down(struct board* board) {
	int i;
	int j;
	unsigned k;
	int valid;

	// Shift tiles down the columns.
	valid = 0;
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
				valid = 1;
			}
		}
	}

	// Return move status.
	return valid;
}

int board_shift_left(struct board* board) {
	int i;
	int j;
	unsigned k;
	int valid;

	// Shift tiles left across the rows.
	valid = 0;
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
				valid = 1;
			}
		}
	}

	// Return move status.
	return valid;
}

int board_shift_right(struct board* board) {
	int i;
	int j;
	unsigned k;
	int valid;

	// Shift tiles right across the rows.
	valid = 0;
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
				valid = 1;
			}
		}
	}

	// Return move status.
	return valid;
}
