#include "io.h"


struct termios saved_term_settings;


void sigint_handler(int signum) {
	restore_mode();
	leave_alternate_buffer();
	exit(1);
}


void setup_signal_handlers() {
	struct sigaction sa;
	sa.sa_handler = sigint_handler;

	sigset_t mask;
	sigemptyset(&mask);
	sa.sa_mask = mask;

	sa.sa_flags = 0;

	sigaction(SIGINT, &sa, NULL);
}


void enter_alternate_buffer() {
	fputs("\33[?1049h", stdout);
}

void leave_alternate_buffer() {
	fputs("\33[?1049l", stdout);
}

void enter_raw_mode(struct termios* term_settings) {
	if (!isatty(STDOUT_FILENO)) {
		fputs("Output is not a tty. Dying.\n", stderr);
		exit(1);
	}

	tcgetattr(STDOUT_FILENO, term_settings);

	saved_term_settings = *term_settings;

	term_settings->c_lflag &= ~(ICANON | ECHO);

	tcsetattr(STDOUT_FILENO, TCSADRAIN, term_settings);

	setvbuf(stdin, NULL, _IONBF, BUFSIZ);
	setvbuf(stdout, NULL, _IONBF, BUFSIZ);
}

void restore_mode() {
	tcsetattr(STDOUT_FILENO, TCSADRAIN, &saved_term_settings);
}
