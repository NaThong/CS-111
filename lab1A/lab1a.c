// NAME: Jeffrey Chan
// EMAIL: jeffschan97@gmail.com
// ID: 004-611-638

#include<stdio.h>
#include<getopt.h>
#include<stdlib.h>
#include<unistd.h>
#include<termios.h>

struct termios savedAttributes;
char *buffer; // buffer to hold read characters

void resetInputMode() {
    tcsetattr(STDIN_FILENO, TCSANOW, &savedAttributes);
}

void setInputMode() {
    struct termios terminalAttributes;

    // check if stdin is a terminal
    if (!isatty(STDIN_FILENO)) {
	fprintf(stderr, "stdin not a terminal.\n");
	exit(EXIT_FAILURE);
    }

    // save terminal attributes
    tcgetattr(STDIN_FILENO, &savedAttributes);
    atexit(resetInputMode);

    // set terminal mode
    tcgetattr(STDIN_FILENO, &terminalAttributes);
    terminalAttributes.c_lflag &= ~(ICANON|ECHO); // clear ICANON and ECHO
    terminalAttributes.c_cc[VMIN] = 1;
    terminalAttributes.c_cc[VTIME] = 0;
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &terminalAttributes) < 0) {
	fprintf(stderr, "error: error in setting new termianl attributes");
	exit(EXIT_FAILURE);
    }
}

void readAndWrite(int fd1, int fd2) {
    int byteOffset = 0;
    ssize_t num_bytes = read(fd1, buffer, 1);
    while (num_bytes) {
	// if ^D, restore terminal modes and exit
	if (*(buffer + byteOffset) == '\004') {
	    exit(0); // shutdown on ^D exit code
	}
	// if receive \r or \n, write both \r\n instead
	if (*(buffer + byteOffset) == '\r' || *(buffer + byteOffset) == '\n') {
	    char temporaryBuffer[2] = {'\r', '\n'};
	    write(fd2, temporaryBuffer, 2);
	    byteOffset += 1;
	    continue;
	}
	write(fd2, buffer + byteOffset, 1);
	byteOffset += 1;
	num_bytes = read(0, buffer + byteOffset, 1);
	if (!num_bytes) { exit(1); }
    }
}

int main(int argc, char **argv) {

    int option = 0; // used to hold option
    int shellFlag = 0; // flag for shell option

    // arguments that this program recognizes
    static struct option options[] = {
    	{"shell", no_argument, 0, 's'},
	{0, 0, 0, 0}
    };

    while ((option = getopt_long(argc, argv, "s", options, NULL)) != -1) {
	switch (option) {
	    case 's': shellFlag = 1; break;
	    default:
		fprintf(stderr, "error: unrecognized argument\nrecognized arguments:\n--shell\n");
		exit(1);
	}
    }

    // set input mode to non-canonical no echo
    setInputMode();

    //char buffer[10];
    //while(read(0, &buffer, sizeof(char))) {
	//write(1, &buffer, sizeof(char));
    //}
    readAndWrite(0, 1);

    exit(0);

}
