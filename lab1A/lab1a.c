// NAME: Jeffrey Chan
// EMAIL: jeffschan97@gmail.com
// ID: 004-611-638

#include<stdio.h>
#include<getopt.h>
#include<stdlib.h>
#include<unistd.h>
#include<termios.h>
#include<poll.h>
#include<signal.h>
#include<sys/wait.h>
#include<sys/types.h>

// struct to hold saved terminal attributes
struct termios savedAttributes;

// buffer to hold read characters
int BUFFER_SIZE = 1024;
char extraBuffer[1024];

// int arrays to hold pipes between processes
int pipeToChild[2];
int pipeToParent[2];

// process id for when forking process
pid_t pid;

int shellFlag; // flag for shell option

void resetInputMode() {
    tcsetattr(STDIN_FILENO, TCSANOW, &savedAttributes);
    if (shellFlag) {
	int status;
        if (waitpid(pid, &status, 0) == -1) {
            fprintf(stderr, "error: waitpid failed");
            exit(EXIT_FAILURE);
        }
        if (WIFEXITED(status)) {
            const int es = WEXITSTATUS(status);
            const int ss = WTERMSIG(status);
            fprintf(stderr, "SHELL EXIT SIGNAL=%d STATUS=%d\n", ss, es);
            exit(0);
        }
    }
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

void signalHandler(int signalNumber) {
    if (shellFlag && signalNumber == SIGINT) {
        kill(pid, SIGINT);
    }
    if (signalNumber == SIGPIPE) {
        exit(0);
    }
}

void createPipe(int pipeHolder[2]) {
    if (pipe(pipeHolder) == -1) {
        fprintf(stderr, "error: error in creating pipe");
        exit(1);
    }
}

void readWriteShell(int fd1, int fd2) {
    struct pollfd pollfdArray[2]; // array of pollfd strctures
    pollfdArray[0].fd = fd1; // first pollfd describes keyboard
    pollfdArray[1].fd = pipeToParent[0]; // second pollfd describes pipe output
    pollfdArray[0].events = POLLIN | POLLHUP | POLLERR;
    pollfdArray[1].events = POLLIN | POLLHUP | POLLERR;

    int returnValue;
    char buffer;

    while (1) {
        // do a polli and check for errors
        returnValue = poll(pollfdArray, 2, 0);
        if (returnValue < 0) {
            fprintf(stderr, "error: error while polling\n");
            exit(1);
        }

        // if keyboard pollfd revents has POLLIN (has input to read)
        if ((pollfdArray[0].revents & POLLIN)) {
            int bytesRead = read(fd1, &buffer, sizeof(char)); // read from keyboard

            if (buffer == '\r' || buffer == '\n') {
        		char tempBuffer[2] = {'\r', '\n'};
        		char shellBuffer[1] = {'\n'};
        		write(fd2, &tempBuffer, 2*sizeof(char));
        		write(pipeToChild[1], &shellBuffer, sizeof(char));
        		continue;
            }
            else if (buffer == 0x04) {
                close(pipeToChild[1]);
            }
            else if (buffer == 0x03) {
                kill(pid, SIGINT);
            }
            else {
                write(fd2, &buffer, sizeof(char));
                write(pipeToChild[1], &buffer, sizeof(char));
            }
        }

        // if shell pollfd has POLLIN (has output to read)
        if ((pollfdArray[1].revents & POLLIN)) {
            int bytesRead = read(pipeToParent[0], &buffer, sizeof(char)); // read from shell pipe

            if (buffer == '\n') {
                char tempBuffer[2] = {'\r', '\n'};
                write(1, &tempBuffer, 2*sizeof(char));
                continue;
            }
            else {
                write(1, &buffer, sizeof(char));
            }
        }

    	if ((pollfdArray[1].revents & (POLLHUP | POLLERR))) {
    	    exit(0);
    	}
    }
}

void readWrite(fd1, fd2) {
    char buffer2;
    while (read(0, &buffer2, sizeof(char))) {
    	// if receive ^D, exit
        if (buffer2 == '\004') {
            exit(0);
        }
        // receiving \r or \n
        else if (buffer2 == '\r' || buffer2 == '\n') {
            char tempBuffer[2] = {'\r', '\n'};
            // should echo as <cr><lf>
            write(1, &tempBuffer, 2*sizeof(char));
            continue;
        }
        else {
            write(1, &buffer2, sizeof(char));
        }
    }
}

int main(int argc, char **argv) {

    int option = 0; // used to hold option

    // arguments that this program recognizes
    static struct option options[] = {
    	{"shell", no_argument, 0, 's'}
    };

    while ((option = getopt_long(argc, argv, "s", options, NULL)) != -1) {
        switch (option) {
            case 's':
            	signal(SIGINT, signalHandler);
            	signal(SIGPIPE, signalHandler);
            	shellFlag = 1;
            	break;
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

    	    readWriteShell(0, 1);
    	}
    }

    // if no shell flag, just do usual readWrite from stdin to stdout
    readWrite(0, 1);

    exit(0);
}
