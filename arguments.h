#ifndef arguments_H
#define arguments_H

#define _POSIX_C_SOURCE 200112L

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
