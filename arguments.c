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
				return "Seed declared but not defined";
			}
			message = arguments_parse_seed(arguments, argv[i]);
			if (message) {
				return message;
			}
		} else if (!strcmp(argv[i], "--mode") ||
			   !strcmp(argv[i], "-m")) {
			arguments->flags |= ARGUMENTS_MODE;
			if (++i >= argc) {
				return "Mode declared but not defined";
			}
			if (!strcmp(argv[i], "plain")) {
				arguments->mode = mode_plain;
			} else if (!strcmp(argv[i], "format")) {
				arguments->mode = mode_format;
			} else {
				return "Mode not recognized";
			}
		} else if (!strcmp(argv[i], "--help")) {
			arguments->flags |= ARGUMENTS_HELP;
		} else if (!strcmp(argv[i], "--legal")) {
			arguments->flags |= ARGUMENTS_LEGAL;
		} else {
			return "Unrecognized argument";
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
