#include<stdio.h>
#include<stdlib.h>
#include<getopt.h>

// global variables
int numThreads = 1;
int numIterations = 1;

int main(int argc, char **argv) {
    int option = 0; // used to hold option

    //arguments this program supports
    static struct option options[] = {
        {"threads", required_argument, 0, 't'},
        {"iterations", required_argument, 0, 'i'},
        {"yield", required_argument, 0, 'y'}
    }

    // iterate through options
    while ((option = getopt_long(argc, argv, "t:i:y", options, NULL)) != -1) {
        switch (option) {
            case 't':
                numThreads = atoi(optarg);
                break;
            case 'i':
                numIterations = atoi(optarg);
                break;
            default:
                fprintf(stderr, "error: unrecognized argument\nreocgnized arguments:\n--threads=#\n--iterations=#\n--yield=[idl]\n");
                exit(1);
        }
    }

    exit(0);
}
