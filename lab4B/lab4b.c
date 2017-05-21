// NAME: Jeffrey Chan
// EMAIL: jeffschan97@gmail.com
// ID: 004-611-638

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<getopt.h>
#include<string.h>
#include<mraa.h>
#include<mraa/aio.h>
#include<sys/time.h>
#include<time.h>
#include<math.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

// GLOBAL VARIABLES
const int B = 4275; // value of thermistor
int period = 1;
char scale = 'F';
FILE *logFile = NULL;
int logFD = 0;

double getTemperature(int rawTemperature, char scale) {
	double temp = 1023.0 / ((double)rawTemperature) - 1.0;
	temp = 100000.0 * temp;
	double celsius = 1.0 / (log(temp/100000.0) / B + 1/298.15) - 273.15;

	if (scale == 'C')
		return celsius;
	return celsius * 9/5 + 32; // farenheit
}

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
				logFile = fopen(optarg, "w"); break;
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

	// initialize time variables
	time_t timer;
	char timeString[10];
	struct tm* timeInfo;

	while (1) {
		// get temperature
		rawTemperature = mraa_aio_read(temperatureSensor);
		double processedTemperature = getTemperature(rawTemperature, scale);

		// get current time
		time(&timer);
		timeInfo = localtime(&timer);
		strftime(timeString, 10, "%H:%M:%S", timeInfo);

		// print to stdout and log file
		fprintf(stdout, "%s %.1f\n", timeString, processedTemperature);
		if (logFile != NULL) {
			fprintf(logFile, "%s %.1f\n", timeString, processedTemperature);
		}

		sleep(period);
	}

	exit(0);
}
