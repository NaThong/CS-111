// NAME: Jeffrey Chan
// EMAIL: jeffschan97@gmail.com
// ID: 004611638

#include<stdio.h>
#include<getopt.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc, char *argv[]) {
     int option = 0; // used to hold option

    // arguments that this program recognizes
    static struct option options[] = {
    	{"port", required_argument, 0, 'p'},
        {0, 0, 0, 0}
    };

    while ((option = getopt_long(argc, argv, "p", options, NULL)) != -1) {
        switch (option) {
            case 'p':
                printf("received port option\n");
                break;
            default:
                fprintf(stderr, "error: unrecognized argument\nrecognized arguments:\n--port\n");
                exit(1);
        }
    }
}
