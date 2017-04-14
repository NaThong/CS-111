// NAME: Jeffrey Chan
// EMAIL: jeffschan97@gmail.com
// ID: 004611638

#include<stdio.h>
#include<stdlib.h>
#include<getopt.h>
#include<signal.h>
#include<fcntl.h>

void catchHandler() {
    fprintf(stderr, "error: segmentation fault\n");
    exit(4);
}

void copy(int fd1, int fd2) {
    char buffer;
    while (read(fd1, &buffer, sizeof(char))) {
	write(fd2, &buffer, sizeof(char));
    }
}

int main(int argc, char **argv) {

    int option = 0; // used to hold option

    static struct option options[] = {
	{"input", required_argument, 0, 'I'},
	{"output", required_argument, 0, 'O'},
	{"segfault", no_argument, 0, 'S'},
	{"catch", no_argument, 0, 'C'},
	{0, 0, 0, 0} // null for end of array
    };

    char *inputFile = NULL; // string to hold input
    char *outputFile = NULL; // string to hold output
    char *nullptr = NULL; // to force segmentation fault
    int segFlag = 0; // flag for segfault option
    
    // parse through options
    while ((option = getopt_long(argc, argv, "I:O:SC", options, NULL)) != -1) {
	switch (option) {
	    case 'I': inputFile = optarg; break;
	    case 'O': outputFile = optarg; break;
	    case 'S': segFlag = 1; break;
	    case 'C': signal(SIGSEGV, catchHandler); break;
	    default:
		// unrecognized option. print usage statement
		fprintf(stderr, "error: unrecognized argument\nrecognized arguments:\n--input=filename\n--output=filename\n--segfault\n--catch\n"
		);
		exit(1);
	}
    }

    // if --segfault force segmentation fault
    if (segFlag) {
	char c = *nullptr;
    }

    // open input and put into file descriptor 0
    if (inputFile) {
	int inputFD = open(inputFile, O_RDONLY);
	if (inputFD >= 0) {
	    close(0);
	    dup(inputFD);
	    close(inputFD);
	}
	else {
	    fprintf(stderr, "error: unable to open input file\n");
	    exit(2);
	}
    }

    // creat output and put into file descriptor 1
    if (outputFile) {
	int outputFD = creat(outputFile, 0666);
	if (outputFD >= 0) {
	    close(1);
	    dup(outputFD);
	    close(outputFD);
	}
	else {
	    fprintf(stderr, "error: unable to open output file\n");
	    exit(3);
	}
    }

    // copy fd0 to fd1
    copy(0, 1);

    // strerror(0);

    exit(0);

}
