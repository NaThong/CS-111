// NAME: Jeffrey Chan
// EMAIL: jeffschan97@gmail.com
// ID: 004-611-638

#include<stdio.h>
#include<stdlib.h>
#include<getopt.h>

// GLOBAL VARIABLES
char *logFile = NULL;
char scale = 'F';

int main(int argc, char **argv) {
	int option = 0; // used to hold option

	// arguments this program supports
	static struct options options[] = {
		{"period", required_argument, 0, 'p'},
		{"scale", required_argument, 0, 's'},
		{"log", required_argument, 0, 'l'}
	};

	// iterate through options
	while ((option = getopt_long(argc, argv, "p:s:l", options, NULL)) != -1) {
		switch (option) {
			case 'p':
				printf("received period option\n");
				break;
			case 's':
				scale = optarg; break;
			case 'l':
				logFile = optarg; break;
			default:
				fprintf(stderr, "error: unrecognized argument\n")
				exit(1);
		}
	}

	exit(0);
}
