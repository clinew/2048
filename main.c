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

#include "board.h"


int main(int argc, char* argv[]) {
	struct board board;
	int status; // Game status.

	// Set up board.
	board_init(&board);
	
	// Get player moves.
	//while (!(status = board_done(&board))) {
	//}
	
	// Print the final board.
	board_print(&board);
	board_shift_up(&board);
	board_print(&board);
	board_shift_down(&board);
	board_print(&board);
	board_shift_left(&board);
	board_print(&board);
	board_shift_right(&board);
	board_print(&board);
}
