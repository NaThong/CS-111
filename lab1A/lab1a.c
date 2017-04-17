// NAME: Jeffrey Chan
// EMAIL: jeffschan97@gmail.com
// ID: 004-611-638

#include<stdio.h>
#include<getopt.h>
#include<stdlib.h>
#include<unistd.h>
#include<termios.h>
#include<poll.h>

// struct to hold saved terminal attributes
struct termios savedAttributes;

// buffer to hold read characters
char buffer;

// int arrays to hold pipes between processes
int pipeToChild[2];
int pipeToParent[2];

// process id for when forking process
pid_t pid;

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

void createPipe(int pipeHolder[2]) {
    if (pipe(pipeHolder) == -1) {
	fprintf(stderr, "error: error in creating pipe");
	exit(1);
    }
}

void closeAllPipes() {
    close(pipeToChild[0]);
    close(pipeToChild[1]);
    close(pipeToParent[0]);
    close(pipeToParent[1]);
}

void readWriteEcho(int fd1, int fd2) {
    struct pollfd pollfdArray[2]; // array of pollfd strctures
    pollfdArray[0].fd = fd1; // first pollfd describes keyboard
    pollfdArray[1].fd = pipeToParent[0]; // second pollfd describes pipe outputi
    pollfdArray[0].events = POLLIN | POLLHUP | POLLERR;
    pollfdArray[1].events = POLLIN | POLLHUP | POLLERR;

    int returnValue;

    while (1) {
	returnValue = poll(pollfdArray, 2, 0);
	if (returnValue < 0) {
	    fprintf(stderr, "error: error while polling\n");
	    exit(1);
	}
		
    }

/*
    while (read(0, &buffer, sizeof(char))) {
    	// if receive ^D, exit
	if (buffer == '\004') {
	    exit(0);
	}
	// receiving \r or \n
	else if (buffer == '\r' || buffer == '\n') {
	    char tempBuffer[2] = {'\r', '\n'};
	    char tempShellBuffer[1] = {'\n'};
	    // should go to shell as <lf>
	    write(pipeToChild[1], &tempShellBuffer, sizeof(char));
	    // should echo as <cr><lf>
	    write(1, &tempBuffer, 2*sizeof(char));
	    continue;
	}
	else {
	    write(pipeToChild[1], &buffer, sizeof(char));
	    write(1, &buffer, sizeof(char));
	}
    }*/
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
    
    // create pipes
    createPipe(pipeToChild);
    createPipe(pipeToParent);

    // if shell flag, fork process
    if (shellFlag) {
	pid = fork();
	// fork failed
	if (pid == -1) {
	    fprintf(stderr, "error: error in forking process");
	    exit(1);
	}
	// child process
	else if (pid == 0) {
	    // pipe reconnection for child process
	    close(pipeToChild[1]);
	    close(pipeToParent[0]);
	    dup2(pipeToChild[0], STDIN_FILENO);
	    dup2(pipeToParent[1], STDOUT_FILENO);
	    close(pipeToChild[0]);
	    close(pipeToParent[1]);
	    
	    // format arguments for shell
	    char *execvp_argv[2];
	    char execvp_filename[] = "/bin/bash";
	    execvp_argv[0] = execvp_filename;
	    execvp_argv[1] = NULL;

	    // execute shell with it's filename as an argument
	    if (execvp(execvp_filename, execvp_argv) == -1) {
		fprintf(stderr, "error: error in executing shell");
		exit(1);
	    }
	}
	// parent process
	else {
	    // pipe reconnection for parent process
	    close(pipeToChild[0]);
	    close(pipeToParent[1]);

	    readWriteEcho(0, 1);

	    /*
	    char buffer[2048];
	    int count = 0;
	    count = read(STDIN_FILENO, buffer, 2048);
	    write(pipeToChild[1], buffer, count);
	    count = read(pipeToParent[0], buffer, 2048);
	    write(STDOUT_FILENO, buffer, count);
	    */
	}
    }

    // readAndWrite(0, 1);

    exit(0);

}
