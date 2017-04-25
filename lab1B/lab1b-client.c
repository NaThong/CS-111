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
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<netinet/in.h>
#include<netdb.h>
#include<mcrypt.h>

// FLAGS
int portFlag = 0;
int logFlag = 0;
int encryptFlag = 0;

struct termios savedAttributes; // struct to hold saved terminal attributes
char *logFile = NULL;           // string to hold log file
int socketFD;                   // fd for socket
int keyLength;                  // length of key from my.key
MCRYPT cryptFD, decryptFD;

void deinitializeEncryption() {
    // DEINIT ENCRYPTIONG
    mcrypt_generic_deinit(cryptFD);
    mcrypt_module_close(cryptFD);
    
    // DEINIT DECRYPTIONG
    mcrypt_generic_deinit(decryptFD);
    mcrypt_module_close(decryptFD);
}

void resetInputMode() {
    tcsetattr(STDIN_FILENO, TCSANOW, &savedAttributes);
    if (encryptFlag) {
	    deinitializeEncryption();
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
    terminalAttributes.c_iflag=ISTRIP;
    terminalAttributes.c_oflag=0;
    terminalAttributes.c_lflag=0;
    tcsetattr(STDIN_FILENO, TCSANOW, &terminalAttributes);
}

char* getKey(char *keyfile) {
    // Gets key from keyfile
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
    // encrypts a buffer with size cryptLength
	if(mcrypt_generic(cryptFD, buffer, cryptLength) != 0) {
        fprintf(stderr, "error: error in with encrypting buffer\n");
        exit(EXIT_FAILURE);
    }
}

void decrypt(char *buffer, int decryptLength) {
    // decrypts a buffer with size decryptLenght
	if(mdecrypt_generic(decryptFD, buffer, decryptLength) != 0) {
        fprintf(stderr, "error: error in decrypting buffer\n");
        exit(EXIT_FAILURE);
    }
}

void readWrite(socketFD) {
    struct pollfd pollfdArray[2];       // array of pollfd strctures
    pollfdArray[0].fd = 0;              // first pollfd describes keyboard
    pollfdArray[1].fd = socketFD;       // second pollfd describes socketFD
    pollfdArray[0].events = POLLIN | POLLHUP | POLLERR;
    pollfdArray[1].events = POLLIN | POLLHUP | POLLERR;

    int returnValue;
    char buffer;
    int logFD;
    if (logFlag) {
        logFD = creat(logFile, 0666);
    }

    while (1) {
        // do a poll and check for errors
        returnValue = poll(pollfdArray, 2, 0);
        if (returnValue < 0) {
            fprintf(stderr, "error: error while polling\n");
            exit(1);
        }
        
        // if keyboard pollfd revents has POLLIN (has input to read)
        if ((pollfdArray[0].revents & POLLIN)) {
            int bytesRead = read(0, &buffer, sizeof(char)); // read from keyboard
            /*if (buffer == '\003') {
            // TODO: something here that ends program and closes socket
            // TODO: client should not check for ^C or ^D but should send it to server instead
            if (buffer == '\003') {
                exit(0);
            }*/
            if (buffer == '\r' || buffer == '\n') {
                char tempBuffer[2] = {'\r','\n'};	
                write(1, &tempBuffer, 2*sizeof(char));
                if (encryptFlag) { encrypt(&buffer, 1); }
                if (logFlag) {
                    write(logFD, "SENT 1 bytes: ", 14);
                    write(logFD, &buffer, sizeof(char));
                    write(logFD, "\n", sizeof(char));
                }
                write(socketFD, &buffer, sizeof(char));
                continue;
            }
            write(1, &buffer, sizeof(char));            // write to screen
            if (encryptFlag) { encrypt(&buffer, 1); }   // encrypt keyboard data
            if (logFlag) {                              // write to log
                write(logFD, "SENT 1 bytes: ", 14);
                write(logFD, &buffer, sizeof(char));
                write(logFD, "\n", 1);
            }
            write(socketFD, &buffer, sizeof(char));     // write to socket
        }

        // if socket pollFD has POLLIN (has output to read)
        if ((pollfdArray[1].revents & POLLIN)) {
            int bytesRead = read(socketFD, &buffer, sizeof(char));  // read from socket
            if (logFlag) {                                          // write to log
                write(logFD, "RECEIVED 1 bytes: ", 17);
                write(logFD, &buffer, sizeof(char));
                write(logFD, "\n", 1);
            }
            if (encryptFlag) { decrypt(&buffer, 1); }               // decrypt socket data
            if (buffer == '\r' || buffer == '\n') {
                char tempBuffer[2] = {'\r','\n'};
                write(1, &tempBuffer, 2*sizeof(char));
                continue;
            }
            write(1, &buffer, sizeof(char));                        // write to screen
        }

        // error checking from socket pollFD
        if ((pollfdArray[1].revents & (POLLHUP | POLLERR))) {
	        fprintf(stderr, "error: received POLLHUP | POLLERR");
            exit(0);    
        }
    }
}

int main(int argc, char *argv[]) {
    int option = 0;
    struct hostent *server;
    struct sockaddr_in serverAddress;
    int portNumber;

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
                portFlag = 1;
		        portNumber = atoi(optarg);
                break;
            case 'l':
               	logFlag = 1;
		        logFile = optarg;
                break;
            case 'e':
                encryptFlag = 1;
                char *key = getKey("my.key");
		        initializeEncryption(key, keyLength);
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

    readWrite(socketFD);

    exit(0);
}

