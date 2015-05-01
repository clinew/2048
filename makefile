#  Copyright 2014-2015 by Wade T. Cline (Frostsnow)
#
#  "2048" is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  "2048" is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with "2048".  If not, see <http://www.gnu.org/licenses/>.

compiler = gcc
flags = -Wall -Werror --pedantic-errors --std=c99
#libraries = -lm
files = arguments board main io
manpage = 2048.1
output = 2048
headers = ${files:=.h}
objects = ${files:=.o}
sources = ${files:=.c}

# Compile the objects then the program.
all: ${files} ${output}

# Compile the programs.
#${executables}: ${sources}
${files}: %: %.c
	${compiler} ${flags} -c $< ${libraries}

${output}: ${objects}
	${compiler} ${flags} -o ${output} ${objects}

# Install the program.
# Hmm, is it okay to install directly to /usr/share and not /usr/local/share?
install:
	cp ${output} /usr/local/bin/${output}
	cp ${manpage} /usr/share/man/man1/${manpage}

# Remove all the output files.
clean: tidy
	rm -f ${output}

# Remove all object files.
tidy:
	rm -f ${objects}
