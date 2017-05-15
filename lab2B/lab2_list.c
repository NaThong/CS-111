// NAME: Jeffrey Chan
// EMAIL: jeffschan97@gmail.com
// ID: 004-611-638

#include<stdio.h>
#include<stdlib.h>
#include<getopt.h>
#include"SortedList.h"
#include<pthread.h>
#include<string.h>
#include<time.h>
#include<signal.h>

#define BILL 1000000000L

// global argument variables
int numThreads = 1;
int numIterations = 1;
int opt_yield = 0;
char syncOption;
int numLists = 1;

// global data structures
SortedList_t* listArray;
SortedListElement_t* elementList;
pthread_mutex_t mutex;
pthread_mutex_t* mutexArray;
int* spinArray;
long lockWaitTime = 0;
int* listNumber;

// strings
char* yieldString = "";
char returnString[50] = "";

// formats tags for test outpu
char* getTestTag() {
    // append proper yield tag
    if (strlen(yieldString) == 0) { strcat(returnString, "-none"); }
    else {
	strcat(returnString, "-");
	int k = 0;
	for (k = 0; *(yieldString + k) != '\0'; k++) {
	    if (*(yieldString + k) == 'i') { strcat(returnString, "i"); }
	    else if (*(yieldString + k) == 'd') { strcat(returnString, "d"); }
	    else if (*(yieldString + k) == 'l') { strcat(returnString, "l"); }
	}
    }

    // append proper sync tag
    if (!syncOption) { strcat(returnString, "-none"); }
    else if (syncOption == 'm') { strcat(returnString, "-m"); }
    else if (syncOption == 's') { strcat(returnString, "-s"); }

    return returnString;
}

// sementation fault handler
void segFaultHandler() {
    fprintf(stderr, "error: segmentation fault\n");
    exit(2);
}

// yield option handler
void setYieldOption(char* yieldOptions) {
    yieldString = yieldOptions;
    const char validYieldOptions[] = {'i', 'd', 'l'}; // initialize array of valid yield options

    // iterate through yield options and set opt_yield accordingly
    int k = 0;
    for (k = 0; *(yieldOptions + k) != '\0'; k++) {
        if (*(yieldOptions + k) == validYieldOptions[0]) opt_yield |= INSERT_YIELD;
        else if (*(yieldOptions + k) == validYieldOptions[1]) opt_yield |= DELETE_YIELD;
        else if (*(yieldOptions + k) == validYieldOptions[2]) opt_yield |= LOOKUP_YIELD;
        else {
            fprintf(stderr, "error: invalid yield option. valid options: [idl]\n");
            exit(1);
        }
    }
}

// hash function
int hash(const char* key) {
  return (key[0] % numLists);
}

// generates random keys and place them into elements
void generateRandomKeys(int totalElements) {
    srand(time(NULL)); // initialize random number generator

    // go through element list and insert random keys
    int k;
    for (k = 0; k < totalElements; k++) {
        int randNumber = rand() % 26; // bound rand number to an alphabet character
        char* randKey = malloc(2 * sizeof(char)); // 1 char key + null byte
        randKey[0] = 'a' + randNumber; // turn randNumber into character
        randKey[1] = '\0';

        elementList[k].key = randKey; // insert key into element
    }
}

// routine that all threads need to perform
void* listOperations(void* threadIndex) {
    int k;
    int totalElements = numThreads * numIterations;
    struct timespec lockWaitStart;
    struct timespec lockWaitEnd;

    // iterate through elements and insert them into sublist based on listNumber
    for (k= *(int*)threadIndex; k < totalElements; k += numThreads) {
    	switch (syncOption) {
    	    case 'm':
          		if (clock_gettime(CLOCK_MONOTONIC, &lockWaitStart) == -1) { fprintf(stderr, "error: error in getting start time\n"); exit(1); }
    		      pthread_mutex_lock(&mutexArray[listNumber[k]]);
        		  if (clock_gettime(CLOCK_MONOTONIC, &lockWaitEnd) == -1) { fprintf(stderr, "error: error in getting stop time\n"); exit(1); }
        		  lockWaitTime += BILL * (lockWaitEnd.tv_sec - lockWaitStart.tv_sec) + (lockWaitEnd.tv_nsec - lockWaitEnd.tv_nsec);
          		SortedList_insert(&listArray[listNumber[k]], &elementList[k]);
          		pthread_mutex_unlock(&mutexArray[listNumber[k]]);
          		break;
    	    case 's':
          		while(__sync_lock_test_and_set(&spinArray[listNumber[k]], 1));
          		SortedList_insert(&listArray[listNumber[k]], &elementList[k]);
          		__sync_lock_release(&spinArray[listNumber[k]]);
          		break;
    	    default:
          		SortedList_insert(&listArray[listNumber[k]], &elementList[k]);
          		break;
    	}
    }

    // get length of list by adding up length of sublists
    int length = 0;
    switch (syncOption) {
    	case 'm':
          for(k = 0; k < numLists; k++) {
      	    if (clock_gettime(CLOCK_MONOTONIC, &lockWaitStart) == -1) { fprintf(stderr, "error: error in getting start time\n"); exit(1); }
      	    pthread_mutex_lock(&mutexArray[k]);
      	    if (clock_gettime(CLOCK_MONOTONIC, &lockWaitEnd) == -1) { fprintf(stderr, "error: error in getting stop time\n"); exit(1); }
            lockWaitTime += BILL * (lockWaitEnd.tv_sec - lockWaitStart.tv_sec) + (lockWaitEnd.tv_nsec - lockWaitEnd.tv_nsec);
            int tempLength = SortedList_length(&listArray[k]);
      	    if (tempLength == -1) {
              	fprintf(stderr, "error: failed to get length of list\n");
              	exit(2);
      	    }
            else
              length += tempLength;
      	    pthread_mutex_unlock(&mutexArray[k]);
          }
    	    break;
    	case 's':
          for (k = 0; k < numLists; k++) {
      	    while (__sync_lock_test_and_set(&spinArray[k], 1));
            int tempLength = SortedList_length(&listArray[k]);
      	    if (tempLength == -1) {
          		fprintf(stderr, "error: failed to get length of list\n");
          		exit(2);
      	    }
            else
              length += tempLength;
      	    __sync_lock_release(&spinArray[k]);
          }
    	    break;
    	default:
          for (k = 0; k < numLists; k++) {
            int tempLength = SortedList_length(&listArray[k]);
      	    if (tempLength == -1) {
          		fprintf(stderr, "error: failed to get length of list\n");
          		exit(2);
      	    }
            else
              length += tempLength;
          }
    	    break;
    }

    // lookup and delete previously inserted elements within sublists
    SortedListElement_t *temp = NULL;
    for (k = *(int*)threadIndex; k < totalElements; k += numThreads) {
      switch (syncOption) {
        case 'm':
          if (clock_gettime(CLOCK_MONOTONIC, &lockWaitStart) == -1) { fprintf(stderr, "error: error in getting start time\n"); exit(1); }
          pthread_mutex_lock(&mutexArray[listNumber[k]]);
          if (clock_gettime(CLOCK_MONOTONIC, &lockWaitEnd) == -1) { fprintf(stderr, "error: error in getting stop time\n"); exit(1); }
          lockWaitTime += BILL * (lockWaitEnd.tv_sec - lockWaitStart.tv_sec) + (lockWaitEnd.tv_nsec - lockWaitEnd.tv_nsec);
          temp = SortedList_lookup(&listArray[listNumber[k]], elementList[k].key);
          if (temp == NULL) {
              fprintf(stderr, "error: failed to find element we already inserted\n");
              exit(2);
          }
          if (SortedList_delete(temp)) {
              fprintf(stderr, "error: failed to delete an element we already inserted\n");
              exit(2);
          }
          pthread_mutex_unlock(&mutexArray[listNumber[k]]);
          break;
        case 's':
          while (__sync_lock_test_and_set(&spinArray[listNumber[k]], 1));
          temp = SortedList_lookup(&listArray[listNumber[k]], elementList[k].key);
          if (temp == NULL) {
              fprintf(stderr, "error: failed to find element we already inserted\n");
              exit(2);
          }
          if (SortedList_delete(temp)) {
              fprintf(stderr, "error: failed to delete an element we already inserted\n");
              exit(2);
          }
          __sync_lock_release(&spinArray[listNumber[k]]);
          break;
        default:
          temp = SortedList_lookup(&listArray[listNumber[k]], elementList[k].key);
          if (temp == NULL) {
              fprintf(stderr, "error: failed to find element we already inserted\n");
              exit(2);
          }
          if (SortedList_delete(temp)) {
              fprintf(stderr, "error: failed to delete an element we already inserted\n");
              exit(2);
          }
	        break;
      }
    }

    return NULL;
}

// function that initializes the global array of sublists
void initializeLists() {
    listArray = malloc(numLists * sizeof(SortedList_t));
    int k;
    for (k = 0; k < numLists; k++) {
        listArray[k].key = NULL;
        listArray[k].next = &listArray[k];
        listArray[k].prev = &listArray[k];
    }
}

// function that initializes locks for the sublists
void initializeLocks() {
    if (syncOption == 'm') {
      mutexArray = malloc(numLists * sizeof(pthread_mutex_t));
      int k;
      for (k = 0; k < numLists; k++)
        pthread_mutex_init(&mutexArray[k], NULL);
    }
    else if (syncOption == 's') {
      spinArray = malloc(numLists * sizeof(int));
      int k;
      for (k = 0; k < numLists; k++)
        spinArray[k] = 0;
    }
}

int main(int argc, char **argv) {
    int option = 0; // used to hold option

    // arguments this program supports
    static struct option options[] = {
        {"threads", required_argument, 0, 't'},
        {"iterations", required_argument, 0, 'i'},
        {"yield", required_argument, 0, 'y'},
        {"sync", required_argument, 0, 's'},
        {"lists", required_argument, 0, 'l'}
    };

    // iterate through options
    while ((option = getopt_long(argc, argv, "t:i:y:s", options, NULL)) != -1) {
        switch (option) {
            case 't':
                numThreads = atoi(optarg);
                break;
            case 'i':
                numIterations = atoi(optarg);
                break;
            case 'y':
                setYieldOption(optarg);
                break;
            case 'l':
                numLists = atoi(optarg);
                break;
            case 's':
                if ((optarg[0] == 'm' || optarg[0] == 's') && strlen(optarg) == 1)
                    syncOption = optarg[0];
                else {
                    fprintf(stderr, "error: unrecognized sync argument. recognized arguments: [ms]\n");
                    exit(1);
                }
		            break;
            default:
                fprintf(stderr, "error: unrecognized argument\nreocgnized arguments:\n--threads=#\n--iterations=#\n--yield=[idl]\n--sync=[ms]\n");
                exit(1);
        }
    }

    // handler for segmentation faults
    signal(SIGSEGV, segFaultHandler);

    // initialize sublists and their locks
    initializeLists();
    initializeLocks();

    // create required number of list elements with random keys
    int totalElements = numThreads * numIterations;
    elementList = malloc(totalElements * sizeof(SortedListElement_t));
    generateRandomKeys(totalElements);

    // calculate which lists each element should go into using hash function
    listNumber = malloc(numLists * sizeof(int));
    int m;
    for (m = 0; m < totalElements; m++)
      listNumber[m] = hash(elementList[m].key);

    // creates the number of specified threads
    pthread_t *threads = malloc(numThreads * sizeof(pthread_t));
    int* threadIds = malloc(numThreads * sizeof(int));

    // start timing
    struct timespec start;
    if (clock_gettime(CLOCK_MONOTONIC, &start) == -1) { fprintf(stderr, "error: error in getting start time\n"); exit(1); }

    // runs the threads
    int k;
    for (k = 0; k < numThreads; k++) {
        threadIds[k] = k;
        if (pthread_create(threads + k, NULL, listOperations, &threadIds[k])) {
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

    free(listArray);
    free(mutexArray);
    free(elementList);
    free(threads);

    // format and print test output
    long totalTime = BILL * (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec); // calculate total time elapsed
    int numOperations = numThreads * numIterations * 3; // total operatinos performed
    long costPerOperation = totalTime / numOperations; // average cost per operation
    long averageLockWaitTime = lockWaitTime / numOperations; // average wait-for-lock
    char* testTag = getTestTag();
    printf("list%s,%d,%d,%d,%d,%d,%d,%d\n", testTag, numThreads, numIterations, numLists, numOperations, totalTime, costPerOperation,averageLockWaitTime);

    // check length of all sublists
    int listLength;
    int i;
    for (i = 0; i < numLists; i++) { listLength += SortedList_length(&listArray[i]); }
    if (listLength != 0) { exit(2); } // exit 2 if list length is not 0

    exit(0);
}
