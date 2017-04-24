// NAME: Jeffrey Chan
// EMAIL: jeffschan97@gmail.com
// ID: 004611638

#include<stdio.h>
#include<getopt.h>
#include<stdlib.h>
#include<unistd.h>
#include<termios.h>
#include<poll.h>

// struct to hold saved terminal attributes
struct termios savedAttributes;

int portFlag = 0; // flag for port option


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

int main(int argc, char *argv[]) {
    int option = 0; // used to hold option

    // arguments that this program recognizes
    static struct option options[] = {
    	{"port", required_argument, 0, 'p'},
        {"log", required_argument, 0, 'l'},
        {"encrypt", required_argument, 0, 'e'},
        {0, 0, 0, 0}
    };

    // parse through arguments
    while ((option = getopt_long(argc, argv, "p", options, NULL)) != -1) {
        switch (option) {
            case 'p':
                printf("received port option\n");
                portFlag = 1;
                break;
            case 'l':
                printf("received log option\n");
                break;
            case 'e':
                printf("received encrypt option\n");
                break;
            default:
                fprintf(stderr, "error: unrecognized argument\nrecognized arguments:\n--port\n--log\n--encrypt\n");
                exit(1);
        }
    }

    // if no port specified, exit
    if (!portFlag) {
        fprintf(stderr, "error: no port specified\n");
        exit(1);
    }

    setInputMode();

    char buffer;
    while(read(0, &buffer, sizeof(char))) {
        write(1, &buffer, sizeof(char));
    }
}
