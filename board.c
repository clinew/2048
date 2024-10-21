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

#include "board.h"


int board_done(struct board* board) {
	int i;
	int j;
	int k;

	// Check for winning number.
	for (i = 0; i < BOARD_ROWS; i++) {
		for (j = 0; j < BOARD_COLUMNS; j++) {
			if (board->tiles[i][j] == 2048) {
				return 1;
			}
		}
	}

	// Check for zeroes
	for (i = 0; i < BOARD_ROWS; i++) {
		for (j = 0; j < BOARD_COLUMNS; j++) {
			if (!board->tiles[i][j]) {
				return 0;
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

void board_free(struct board* board) {
	// Close the score file.
	close(board->score_file);
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
	// Initialize the scores.
	board->score_current = 0;
	board_init_score(board);

	// Initialize the tiles.
	board_tiles_clear(board);

	// Add two tiles to the board.
	board_plop(board);
	board_plop(board);
}

void board_init_score(struct board* board) {
	char directory_path[128];
	char file_path[128];
	char* home;

	// Get filepaths.
	if (!(home = getenv("HOME"))) {
		fprintf(stderr, "Unable to get home directory.");
		goto out;
	}
	strncpy(directory_path, home, sizeof(directory_path));
	if (strlen(directory_path) + strlen("/.2048") + 1 > 128) {
		fprintf(stderr, "Directory path too long.");
		goto out;
	}
	strncat(directory_path, "/.2048", sizeof(directory_path) - 1);
	strncpy(file_path, home, sizeof(file_path));
	if (strlen(file_path) + strlen("/.2048/score_top") + 1 > 128) {
		fprintf(stderr, "File path too long.");
		goto out;
	}
	strncat(file_path, "/.2048/score_top", sizeof(file_path) - 1);

	// Open the top scores file.
	if ((board->score_file = open(file_path, O_RDWR)) == -1) {
		int directory;

		// Create program directory.
		if ((directory = open(directory_path, O_DIRECTORY | O_RDONLY))
								== -1) {
			if (mkdir(directory_path, 0755) == -1) {
				perror("Unable to create save path.");
				goto out;
			}
		} else {
			close(directory);
		}

		// Create top scores file.
		if ((board->score_file = open(file_path, O_CREAT | O_RDWR,
							0644)) == -1) {
			perror("Unable to create save file.");
			goto out;
		}

		// Set initial score.
		board->score_top = 0;
	} else {
		// Read in top score.
		if (read(board->score_file, &board->score_top,
				sizeof(board->score_top)) == -1 ) {
			perror("Unable to read in top score.");
			goto out;
		}
	}

	// Return safely.
	return;

out:
	// Score file neither found nor loaded.
	board->score_top = 0;
	board->score_file = 0;
	return;
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
				board_tiles_merge(board,
						  &board->tiles[k][i],
						  &board->tiles[j][i]);
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
				board_tiles_merge(board,
						  &board->tiles[i][j],
						  &board->tiles[i][k]);
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
				board_tiles_merge(board,
						  &board->tiles[i][k],
						  &board->tiles[i][j]);
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
				board_tiles_merge(board,
						  &board->tiles[j][i],
						  &board->tiles[k][i]);
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
	char a = board_merge_down(board);
	return a | board_shift_down(board);
}

int board_move_left(struct board* board) {
	char a = board_merge_left(board);
	return a | board_shift_left(board);
}

int board_move_right(struct board* board) {
	char a = board_merge_right(board);
	return a | board_shift_right(board);
}

int board_move_up(struct board* board) {
	char a = board_merge_up(board);
	return a | board_shift_up(board);
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

	// Print the board's score.
	printf("          Top: %5u\n      Current: %5u\n\n", board->score_top,
	       board->score_current);

	// Print the board's tiles.
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

void board_reset(struct board* board) {
	// Clear the current score.
	board->score_current = 0;

	// Clear the tiles.
	board_tiles_clear(board);

	// Place new starting tiles.
	board_plop(board);
	board_plop(board);
}

void board_score_updated(struct board* board) {
	// Check for new top score.
	if (board->score_current <= board->score_top) {
		// Nothing to do.
		return;
	}

	// Update in-game score.
	board->score_top = board->score_current;

	// Update score file.
	if (!board->score_file) {
		// Nothing to do.
		return;
	}
	if (lseek(board->score_file, 0, SEEK_SET) == -1) {
		fprintf(stderr, "Error updating score file: %i", errno);
		return;
	}
	if (write(board->score_file, &board->score_top,
			sizeof(board->score_top)) == -1) {
		fprintf(stderr, "Error writing to score file: %i", errno);
		return;
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
	int k;
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
	int k;
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

void board_tiles_clear(struct board* board) {
	int i;
	int j;

	// Clear the tiles.
	for (i = 0; i < BOARD_ROWS; i++) {
		for (j = 0; j < BOARD_COLUMNS; j++) {
			board->tiles[i][j] = 0;
		}
	}
}

void board_tiles_merge(struct board* board, unsigned* a, unsigned* b) {
	// Merge the two specified tiles.
	board->score_current += (*a) += (*a);
	(*b) = 0;

	// Propogate updated score.
	board_score_updated(board);
}
