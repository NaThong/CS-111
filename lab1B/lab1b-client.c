// NAME: Jeffrey Chan
// EMAIL: jeffschan97@gmail.com
// ID: 004611638

#include<stdio.h>
#include<getopt.h>
#include<stdlib.h>
#include<unistd.h>
#include<termios.h>
#include<poll.h>
#include<sys/types.h>
#include<sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>

struct termios savedAttributes; // struct to hold saved terminal attributes
int portFlag = 0; // flag for port flag
int socketFD; // fd for socket

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
    terminalAttributes.c_iflag=ISTRIP;
    terminalAttributes.c_oflag=0;
    terminalAttributes.c_lflag=0;
    tcsetattr(STDIN_FILENO, TCSANOW, &terminalAttributes);
}

int main(int argc, char *argv[]) {
    int option = 0; // used to hold option
    struct hostent *server;
    struct sockaddr_in serverAddress;
    int portNumber;

    // arguments that this program recognizes
    static struct option options[] = {
    	{"port", required_argument, 0, 'p'},
        {"log", required_argument, 0, 'l'},
        {"encrypt", required_argument, 0, 'e'},
        {0, 0, 0, 0}
    };

    // parse through arguments
    while ((option = getopt_long(argc, argv, "p:l:e", options, NULL)) != -1) {
        switch (option) {
            case 'p':
		// set portFlag and portNumber
                portFlag = 1;
		portNumber = atoi(optarg);
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

    // set input mode to non-canonical no echo mode
    setInputMode();

    // setup socket file descriptor
    socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFD < 0) { fprintf(stderr, "error: error in opening socket\n"); exit(0); }
    server = gethostbyname("127.0.0.1");
    if (server == NULL) { fprintf(stderr, "error: cannot find host\n"); exit(0); }

    // establish server address and assign it
    memset((char*) &serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    memcpy((char*) &serverAddress.sin_addr.s_addr, (char*) server->h_addr, server->h_length);
    serverAddress.sin_port = htons(portNumber);

    if (connect(socketFD, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
	fprintf(stderr, "error: error in connecting\n");
	exit(0);
    }

    exit(0);
}

