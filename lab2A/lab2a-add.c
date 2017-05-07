// NAME: Jeffrey Chan
// EMAIL: jeffschan97@gmail.com
// ID: 004-611-638

#include<stdio.h>
#include<stdlib.h>
#include<getopt.h>
#include<time.h>
#include<pthread.h>

#define BILL 1000000000L

// provided add function
void add (long long *pointer, long long value) {
    long long sum = *pointer + value;
    *pointer = sum;
}

int main(int argc, char **argv) {
    int option = 0; // used to hold option
    int numThreads = 1; // # threads defaulted to 1
    int numIterations = 1; // # iterations defaulted to 1
    long long counter = 0; // long long counter initialized to 0

    // arguments this program supports
    static struct option options[] = {
	{"threads", required_argument, 0, 't'},
	{"iterations", required_argument, 0, 'i'}
    };

    // iterate through options
    while ((option = getopt_long(argc, argv, "t:i", options, NULL)) != -1) {
	switch (option) {
	    case 't':
		// numThreads = optarg;
		numThreads = atoi(optarg);
		break;
	    case 'i':
		numIterations = atoi(optarg);
		break;
	    default:
		fprintf(stderr, "error: unrecognized argument\nrecognized arguments:\n--threads=#\n--iterations=#\n");
		exit(1);
	}
    }
    
    // start timing
    struct timespec start;
    if (clock_gettime(CLOCK_MONOTONIC, &start) == -1) { fprintf(stderr, "error: error in getting start time\n"); exit(1); }

    // creates the number of specified threads
    pthread_t *threads = malloc(numThreads * sizeof(pthread_t));
    int k;
    for (k = 0; k < numThreads; k++) {
	if (pthread_create(threads + k, NULL, &add, &counter)) {
	    fprintf(stderr, "error: error in thread creation\n");
	    exit(1);
	}
    }

    // wait for all the threads to complete
    for (k = 0; k < numThreads; k++) {
	if (pthread_join(*(threads + k), NULL)) {
	    fprintf(stderr, "error: error in joining threads\n");
	    exit(1);
	}
    }

    // stop timing
    struct timespec end;
    if (clock_gettime(CLOCK_MONOTONIC, &end) == -1) { fprintf(stderr, "error: error in getting stop time\n"); exit(1); }

    long totalTime = end.tv_sec - start.tv_sec + (end.tv_nsec - start.tv_nsec); // calculate total time elapsed
    printf("totalTime: %d\n", totalTime);

    exit(0);
}
