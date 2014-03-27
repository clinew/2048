#include "arguments.h"


int arguments_parse(struct arguments* arguments, int argc, char* argv[]) {
	int i;

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
				fprintf(stderr, "Not enough arguments for "
					"the specified options.\n");
				return -1;
			}
			if (arguments_parse_seed(arguments, argv[i]) == -1) {
				return -1;
			}
		} else {
			fprintf(stderr, "Unrecognized argument: %s.\n",
				argv[i]);
			return -1;
		}
	}
}

int arguments_parse_seed(struct arguments* arguments, char* arg) {
	long long seed;
	char* end;

	// Parse the seed argument.
	seed = strtoll(arg, &end, 0);
	if (arg == end) {
		fprintf(stderr, "No valid digits in seed.\n");
		return -1;
	} else if (errno == EINVAL || errno == ERANGE) {
		perror("Converting seed");
		return -1;
	} else if (*end != '\0') {
		fprintf(stderr, "Nonsense suffix in seed.\n");
		return -1;
	}

	// Check for overflow.
	if (seed < 0) {
		fprintf(stderr, "Refusing to coerce negative seed (it's "
			"probably not what you want).\n");
		return -1;
	} else if (seed > ULONG_MAX) {
		fprintf(stderr, "Refusing to coerce seed overflow (it's "
			"probably not what you want.)\n");
		return -1;
	}

	// Assign seed.
	arguments->seed = seed;

	// Return success.
	return 0;
}
