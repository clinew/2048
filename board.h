/*
 *  Copyright 2014-2015 by Wade T. Cline (Frostsnow)
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

#ifndef board_H
#define board_H

#include "include.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


#define BOARD_COLUMNS 4
#define BOARD_ROWS 4

// Likelihood of a '4' appearing on the board represented as a percentage
// multiplied by 100.
#define BOARD_4SPAWN_CHANCE 15


/**
 * Represents the in-game board.
 */
struct board {
	// Total score for the current game.
	unsigned score_current;
	// File handle for the top score.
	int score_file;
	// Top score.
	unsigned score_top;
	// Holds the value of each tile.
	unsigned tiles[BOARD_ROWS][BOARD_COLUMNS];
};


/**
 * Returns 0 if the game is not over; >0 if the game is over and the user
 * has won, or <0 if the user has lost.
 */
int board_done(struct board* board);

/**
 * Clean up any resources associated with the speicifed board.
 */
void board_free(struct board* board);

/**
 * Returns the number of empty tiles in the board.
 */
unsigned board_get_tiles_empty(struct board* board);

/**
 * Initializes the specified board.
 */
void board_init(struct board* board);

/**
 * Initialize the specified board's top score and score file.
 */
void board_init_score(struct board* board);

/**
 * Merge tiles in the board downwards.
 */
int board_merge_down(struct board* board);

/**
 * Merge tiles in the board leftwards.
 */
int board_merge_left(struct board* board);

/**
 * Merge tiles in the board rightwards.
 */
int board_merge_right(struct board* board);

/**
 * Merge tiles in the board upwards.
 */
int board_merge_up(struct board* board);

/**
 * Processes user move-down request.
 */
int board_move_down(struct board* board);

/**
 * Process user move-left request.
 */
int board_move_left(struct board* board);

/**
 * Process user move-right request.
 */
int board_move_right(struct board* board);

/**
 * Process user move-up request.
 */
int board_move_up(struct board* board);

/**
 * Spawn a new tile on the board.
 *
 * I didn't know what to call this, so "plopping" a value on the board sounds
 * legit enough.
 */
void board_plop(struct board* board);

/**
 * Print the board to 'stdout'.
 */
void board_print(struct board* board);

/**
 * Start a new game.
 */
void board_reset(struct board* board);

/**
 * Call whenever the current score is updated in order to propogate changes
 * to the top score and score file.
 */
void board_score_updated(struct board* board);

/**
 * Shift all the elements in the board down.
 */
int board_shift_down(struct board* board);

/**
 * Shift all the elements in the board left.
 */
int board_shift_left(struct board* board);

/**
 * Shift all the elements in the board right.
 */
int board_shift_right(struct board* board);

/**
 * Shift all the elements in the board up.
 */
int board_shift_up(struct board* board);

/**
 * Sets the values of all tiles on the specified board to '0'.
 */
void board_tiles_clear(struct board* board);

/**
 * Merges the specified tile 'b' into the specified tile 'a' and updates the
 * total score.
 */
void board_tiles_merge(struct board* board, unsigned* a, unsigned* b);


#endif // board_H
