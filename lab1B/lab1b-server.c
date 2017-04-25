// NAME: Jeffrey Chan
// EMAIL: jeffschan97@gmail.com
// ID: 004611638

#include<stdio.h>
#include<getopt.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<poll.h>
#include<signal.h>
#include<fcntl.h>
#include<netdb.h>
#include<netinet/in.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<mcrypt.h>

// buffer to hold read characters
int BUFFER_SIZE = 1024;
char extraBuffer[1024];

// int arrays to hold pipes between processes
int pipeToChild[2];
int pipeToParent[2];

pid_t pid; // process id for when forking process
int socketFD, newSocketFD;
int encryptFlag; // flag for encrypt option
int keyLength; // length of key
MCRYPT cryptFD, decryptFD;

void deinitializeEncryption() {
    // DEINIT ENCRYPTIONG
    mcrypt_generic_deinit(cryptFD);
    mcrypt_module_close(cryptFD);
    
    // DEINIT DECRYPTIONG
    mcrypt_generic_deinit(decryptFD);
    mcrypt_module_close(decryptFD);
}

char* getKey(char *keyfile) {
    struct stat keyStat;
    int keyFD = open(keyfile, O_RDONLY);
    if (fstat(keyFD, &keyStat) < 0) {
        fprintf(stderr, "error: error with fstat\n");
        exit(EXIT_FAILURE);
    }
    char *key = (char*)malloc(keyStat.st_size*sizeof(char));
    if (read(keyFD, key, keyStat.st_size) < 0) {
        fprintf(stderr, "error: error reading from key file\n");
        exit(EXIT_FAILURE);
    }
    keyLength = keyStat.st_size;
    return key;
}

void initializeEncryption(char *key, int keyLength) {
    // INIT ENCRYPTION
    cryptFD = mcrypt_module_open("blowfish", NULL, "cfb", NULL);
    if (cryptFD == MCRYPT_FAILED) {
        fprintf(stderr, "error: error in opening module\n");
        exit(EXIT_FAILURE);
    }
    if (mcrypt_generic_init(cryptFD, key, keyLength, NULL) < 0) {
        fprintf(stderr, "error: error in initializing encryption\n");
        exit(EXIT_FAILURE);
    }

    // INIT DECRYPTION
    decryptFD = mcrypt_module_open("blowfish", NULL, "cfb", NULL);
    if (decryptFD == MCRYPT_FAILED) {
        fprintf(stderr, "error: error in opening module\n");
        exit(EXIT_FAILURE);
    }
    if (mcrypt_generic_init(decryptFD, key, keyLength, NULL) < 0) {
        fprintf(stderr, "error: error in initializing decryption\n");
        exit(EXIT_FAILURE);
    }
}

void encrypt(char *buffer, int cryptLength) {
	if(mcrypt_generic(cryptFD, buffer, cryptLength) != 0) {
        fprintf(stderr, "error: error in with encrypting buffer\n");
        exit(EXIT_FAILURE);
    }
}

void decrypt(char *buffer, int decryptLength) {
	if(mdecrypt_generic(decryptFD, buffer, decryptLength) != 0) {
        fprintf(stderr, "error: error in decrypting buffer\n");
        exit(EXIT_FAILURE);
    }
}

void signalHandler(int signal) {
    if (signal == SIGINT) {
	    kill(pid, SIGINT);
    }
}

void createPipe(int pipeHolder[2]) {
    if (pipe(pipeHolder) == -1) {
        fprintf(stderr, "error: error in creating pipe");
        exit(1);
    }
}

void execShell() {
    // pipe reconnection
    close(pipeToChild[1]);
    close(pipeToParent[0]);
    dup2(pipeToChild[0], STDIN_FILENO);
    dup2(pipeToParent[1], STDOUT_FILENO);
    dup2(pipeToParent[1], STDERR_FILENO);
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

void closeAllFD() {
    close(pipeToChild[1]);
    close(pipeToParent[0]);
    close(newSocketFD);
    close(socketFD);
}

void shutdownServer(int eof) {
    closeAllFD();
    int status;
    if (waitpid(pid, &status, 0) == -1) {
        fprintf(stderr, "error: waitpid failed");
        exit(EXIT_FAILURE);
    }
    if (WIFEXITED(status)) {
        const int es = WEXITSTATUS(status);
        const int ss = WTERMSIG(status);
	if (eof) {
            fprintf(stderr, "SHELL EXIT SIGNAL=%d STATUS=%d\n", ss, es);
	}
        exit(0);
    }
}

void readWrite(int socketFD) {
    struct pollfd pollfdArray[2]; // array of pollfd strctures
    pollfdArray[0].fd = socketFD; // first pollfd describes socket
    pollfdArray[1].fd = pipeToParent[0]; // second pollfd describes pipe output
    pollfdArray[0].events = POLLIN | POLLHUP | POLLERR;
    pollfdArray[1].events = POLLIN | POLLHUP | POLLERR;

    int returnValue;
    char buffer;

    while (1) {
        // do a poll and check for errors
        returnValue = poll(pollfdArray, 2, 0);
        if (returnValue < 0) {
            fprintf(stderr, "error: error while polling\n");
            exit(1);
        }
        
        // if socketFD pollfd revents has POLLIN (has input to read)
        if ((pollfdArray[0].revents & POLLIN)) {
            int bytesRead = read(socketFD, &buffer, sizeof(char)); // read from socketFD
            if (encryptFlag) { decrypt(&buffer, 1); } // if encryptFlag, decrypt socket data
            if (buffer == '\003') {
		shutdownServer(0);
            }
            if (buffer == '\004') {
                shutdownServer(1);
            }
            if (buffer == '\r' || buffer == '\n') {
                char shellBuffer[1] = {'\n'};
                write(pipeToChild[1], &shellBuffer, sizeof(char));
                continue;
            }
            write(pipeToChild[1], &buffer, sizeof(char));
        }

        // if shell pollfd has POLLIN (has output to read)
        if ((pollfdArray[1].revents & POLLIN)) {
            int bytesRead = read(pipeToParent[0], &buffer, sizeof(char)); // read from shell pipe
            if (encryptFlag) { encrypt(&buffer, 1); } // encrypt data before sending over socket
            write(socketFD, &buffer, sizeof(char));
        }

        if ((pollfdArray[1].revents & (POLLHUP | POLLERR))) {
            fprintf(stderr, "error: received POLLHUP|POLLERR\n");
            exit(0);    
        }
    }
}

int main(int argc, char *argv[]) {
     int option = 0; // used to hold option
     int portNumber, clientLength;
     struct sockaddr_in serverAddress, clientAddress;

    // arguments that this program recognizes
    static struct option options[] = {
    	{"port", required_argument, 0, 'p'},
        {"encrypt", required_argument, 0, 'e'},
        {0, 0, 0, 0}
    };

    // parse through arguments
    while ((option = getopt_long(argc, argv, "p:e", options, NULL)) != -1) {
        switch (option) {
            case 'p':
                portNumber = atoi(optarg);
                break;
            case 'e':
                encryptFlag = 1;
                char *key = getKey("my.key");
                initializeEncryption(key, keyLength);
                break;
            default:
                fprintf(stderr, "error: unrecognized argument\nrecognized arguments:\n--port\n--encryp\n");
                exit(1);
        }
    }

    // process SIGINTS on the server side
    signal(SIGINT, signalHandler);

    // setup socket settings
    socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFD < 0) { fprintf(stderr, "error: error in opening socket\n"); exit(1); }
    memset((char*) &serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(portNumber);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // bind host address
    if (bind(socketFD, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        fprintf(stderr, "error: error in binding socket");
        exit(1);
    }

    // listen for incoming clients
    listen(socketFD, 5);
    clientLength = sizeof(clientAddress);

    // accept connection from client
    newSocketFD = accept(socketFD, (struct sockaddr *) &clientAddress, &clientLength);
    if (newSocketFD < 0) {
	fprintf(stderr, "error: error accepting client");
	exit(1);
    }

    // create pipes
    createPipe(pipeToChild);
    createPipe(pipeToParent);

    // fork process and exec shell
    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "error: error in forking\n");
        exit(1);
    }
    else if (pid == 0) {
        // CHILD PROCESS
        execShell();
    }
    else {
        // PARENT PROCESS
        readWrite(newSocketFD);
    }

    deinitializeEncryption(); //deinitialize encryption before exiting
    exit(0);
}
