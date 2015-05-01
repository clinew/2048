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

#ifndef arguments_H
#define arguments_H

#include <inttypes.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mode.h"


// Display the help message and exit.
#define ARGUMENTS_HELP		0x00000001
// Display the legal information and exit.
#define ARGUMENTS_LEGAL		0x00000002
// Whether the user specified a mode.
#define ARGUMENTS_MODE		0x00000004
// Whether to use a user-specified seed or not.
#define ARGUMENTS_SEED		0x00000008
// Whether the user requested program version or not.
#define ARGUMENTS_VERSION	0x00000010

/**
 * Structure to hold arguments parsed by the program.
 */
struct arguments {
	// Bitwise argument flags.
	uint32_t flags;
	// How to write output.
	enum mode mode;
	// Seed for the random-number generator.
	unsigned seed;
};


/**
 * Parse the arguments provided by the user.
 */
char* arguments_parse(struct arguments* arguments, int argc, char* argv[]);

/**
 * Parse a user-specified seed.
 */
char* arguments_parse_seed(struct arguments* arguments, char* arg);


#endif // arguments_H
