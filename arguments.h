#ifndef arguments_H
#define arguments_H


#include <inttypes.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
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
int arguments_parse(struct arguments* arguments, int argc, char* argv[]);


#endif // arguments_H
