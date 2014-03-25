#include "io.h"


void sigint_handler(int signum)
{
	fputs("\33[?1049l", stdout);
	exit(0);
}


void setup_signal_handlers()
{
	struct sigaction sa;
	sa.sa_handler = sigint_handler;

	sigset_t mask;
	sigemptyset(&mask);
	sa.sa_mask = mask;

	sa.sa_flags = 0;

	sigaction(SIGINT, &sa, NULL);
}


void initialize_tty(struct termios* term_settings) {
	setup_signal_handlers();
	fputs("\33[?1049h", stdout);

	if (!isatty(STDOUT_FILENO)) {
		fputs("Output is not a tty. Dying.\n", stderr);
		exit(1);
	}

	tcgetattr(STDOUT_FILENO, term_settings);

	term_settings->c_lflag &= ~(ICANON | ECHO);

	tcsetattr(STDOUT_FILENO, TCSADRAIN, term_settings);

	setvbuf(stdin, NULL, _IONBF, BUFSIZ);
	setvbuf(stdout, NULL, _IONBF, BUFSIZ);
}
