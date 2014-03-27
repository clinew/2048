#  Copyright 2014 by Wade T. Cline
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.


compiler = gcc
flags = -Wall -Werror --pedantic-errors --std=c99
#libraries = -lm
files = arguments board main
output = 2048
headers = ${files:=.h}
objects = ${files:=.o}
sources = ${files:=.c}

# Compile the objects then the program.
all: ${files} ${output}

# Compile the programs.
#${executables}: ${sources}
${files}: %: %.c
	${compiler} -c $< ${libraries}

${output}: ${objects}
	${compiler} -o ${output} ${objects}

# Remove all the output files.
clean: tidy
	rm -f ${output}

# Remove all object files.
tidy:
	rm -f ${objects}
