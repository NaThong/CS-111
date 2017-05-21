// NAME: Jeffrey Chan
// EMAIL: jeffschan97@gmail.com
// ID: 004-611-638

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<getopt.h>
#include<string.h>
#include<mraa/aio.h>

// GLOBAL VARIABLES
int period = 0;
char scale = 'F';
char *logFile = NULL;

int main(int argc, char **argv) {
	int option = 0; // used to hold option

	// arguments this program supports
	static struct option options[] = {
		{"period", required_argument, 0, 'p'},
		{"scale", required_argument, 0, 's'},
		{"log", required_argument, 0, 'l'}
	};

	// iterate through options
	while ((option = getopt_long(argc, argv, "p:s:l", options, NULL)) != -1) {
		switch (option) {
			case 'p':
				period = atoi(optarg); break;
			case 's':
				if (strlen(optarg) == 1 && (optarg[0] == 'C' || optarg[0] == 'F'))
					scale = optarg[0];
				else {
					fprintf(stderr, "error: unrecognized scale argument\n");
					exit(1);
				}
				break;
			case 'l':
				logFile = optarg; break;
			default:
				fprintf(stderr, "error: unrecognized argument\n");
				exit(1);
		}
	}

	// initialize temperature sensor at A0
	mraa_aio_context temperatureSensor;
	temperatureSensor = mraa_aio_init(0);

	// to hold raw temperature read by temperature sensor
	int rawTemperature = 0;

	exit(0);
}
