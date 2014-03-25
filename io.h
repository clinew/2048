#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

void initialize_tty(struct termios* term_settings);

void setup_signal_handlers();
