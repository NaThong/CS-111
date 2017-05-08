// NAME: Jeffrey Chan
// EMAIL: jeffschan97@gmail.com
// ID: 004-611-638

#include<stdio.h>
#include<stdlib.h>
#include<getopt.h>
#include<time.h>
#include<pthread.h>

#define BILL 1000000000L

// gloabl variables
int numThreads = 1;
int numIterations = 1;
long long counter = 0;

// provided add function
void add (long long *pointer, long long value) {
    long long sum = *pointer + value;
    *pointer = sum;
}

void* add_one (void *pointer) {
    pointer = (long long*) pointer;

    int i = 0;
    for (i = 0; i < numIterations; i++) {
	add(pointer, 1);
    }
    for (i = 0; i < numIterations; i++) {
	add(pointer, -1);
    }
}

int main(int argc, char **argv) {
    int option = 0; // used to hold option

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
	if (pthread_create(threads + k, NULL, add_one, &counter)) {
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

    long totalTime = BILL * (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec); // calculate total time elapsed
    int numOperations = numThreads * numIterations * 2; // calculate number of operations
    int timePerOperation = totalTime / numOperations; // calculate the total time cost per operations
    
    printf("totalTime: %d counter: %d\n", totalTime, counter);

    exit(0);
}
