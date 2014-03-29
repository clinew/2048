#ifndef arguments_H
#define arguments_H

#define _POSIX_C_SOURCE 200112L

#include <inttypes.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Whether the user requested program version or not.
#define ARGUMENTS_VERSION 0x00000001
// Whether to use a user-specified seed or not.
#define ARGUMENTS_SEED 0x00000002

/**
 * Structure to hold arguments parsed by the program.
 */
struct arguments {
	// Bitwise argument flags.
	uint32_t flags;
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
