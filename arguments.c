#include "arguments.h"


char* arguments_parse(struct arguments* arguments, int argc, char* argv[]) {
	int i;
	char* message;

	// Clear the arguments structure.
	memset((void*)arguments, 0, sizeof(struct arguments));

	// Parse arguments.
	i = 0;
	while (++i < argc) {
		if (!strcmp(argv[i], "--version") | !strcmp(argv[i], "-v")) {
			arguments->flags |= ARGUMENTS_VERSION;
		} else if (!strcmp(argv[i], "--seed") |
			   !strcmp(argv[i], "-s")) {
			arguments->flags |= ARGUMENTS_SEED;
			if (++i >= argc) {
				return "Seed declared but not defined.";
			}
			message = arguments_parse_seed(arguments, argv[i]);
			if (message) {
				return message;
			}
		} else {
			return "Unrecognized argument.";
		}
	}

	// Return success.
	return NULL;
}

char* arguments_parse_seed(struct arguments* arguments, char* arg) {
	long long seed;
	char* end;

	// Parse the seed argument.
	seed = strtoll(arg, &end, 0);
	if (arg == end) {
		return "No valid digits in seed";
	} else if (errno == EINVAL || errno == ERANGE) {
		return strerror(errno);
	} else if (*end != '\0') {
		return "Nonsense suffix in seed";
	}

	// Check for overflow.
	if (seed < 0) {
		return "Refusing to coerce negative seed (it's probably not "
		       "what you want).";
	} else if (seed > ULONG_MAX) {
		return "Refusing to coerce seed overflow (it's probably not "
		       "what you want).";
	}

	// Assign seed.
	arguments->seed = seed;

	// Return success.
	return NULL;
}
