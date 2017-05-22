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
#include<poll.h>

// GLOBAL VARIABLES
const int B = 4275; // value of thermistor
int period = 1;
char scale = 'F';
FILE *logFile = NULL;
int start = 1;

double getTemperature(int rawTemperature, char scale) {
	double temp = 1023.0 / ((double)rawTemperature) - 1.0;
	temp = 100000.0 * temp;
	double celsius = 1.0 / (log(temp/100000.0) / B + 1/298.15) - 273.15;

	if (scale == 'C')
		return celsius;
	return celsius * 9/5 + 32; // farenheit
}

void printCommand(const char* command) {
	if (logFile) fprintf(logFile, "%s\n", command);
	fflush(logFile);
}

void handleShutdown(FILE *logFile) {
	time_t localTimer;
	char timeString[10];
	struct tm* localTimeInfo;

	// get current time
	time(&localTimer);
	localTimeInfo = localtime(&localTimer);
	strftime(timeString, 10, "%H:%M:%S", localTimeInfo);

	// print log and exit
	if (logFile)
		fprintf(logFile, "%s SHUTDOWN\n", timeString);
	exit(0);
}

void handleStartStop(const int newValue, const char* command) {
	start = newValue;
	fprintf(stdout, "start is now: %d", start)
	printCommand(command);
}

void handleScale(char newScale, const char* command) {
	scale = newScale;
	printCommand(command);
}

void handlePeriod(int newPeriod, const char* command) {
	period = newPeriod;
}

void handleInvalidCommand() {
	printCommand("error: invalid command");
	exit(1);
}

void handleCommand(const char* command) {
	if (strcmp(command, "OFF") == 0) {
		printCommand("OFF");
		handleShutdown(logFile);
	}
	else if (strcmp(command, "STOP") == 0) handleStartStop(0, command);
	else if (strcmp(command, "START") == 0) handleStartStop(1, command);
	else if (strcmp(command, "SCALE=F") == 0) handleScale('F', command);
	else if (strcmp(command, "SCALE=C") == 0) handleScale('C', command);
	else handleInvalidCommand();
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

	// initialize button at D3
	mraa_gpio_context button;
	button = mraa_gpio_init(3);
	mraa_gpio_dir(button, MRAA_GPIO_IN);

	// to hold raw temperature read by temperature sensor
	int rawTemperature = 0;

	// initialize time variables
	time_t timer, start, end;
	char timeString[10];
	struct tm* timeInfo;

	// initialize poll structures
	struct pollfd pollfdArray[1];
	pollfdArray[0].fd = STDIN_FILENO; // polls from stdin
	pollfdArray[0].events = POLLIN | POLLHUP | POLLERR;

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
		if (logFile) {
			fprintf(logFile, "%s %.1f\n", timeString, processedTemperature);
		}
		fflush(logFile); // flush out buffer to make sure log file is written

		time(&start); // start the timer
		time(&end);	// sample ending time
		while (difftime(end, start) < period) {
			// read from button and shutdown if needed
			if (mraa_gpio_read(button))
				handleShutdown(logFile);

			// poll for input
			int returnValue = poll(pollfdArray, 1, 0);
			if (returnValue < 0) {
				fprintf(stderr, "error: error while polling\n");
				exit(1);
			}

			if ((pollfdArray[0].revents & POLLIN)) {
				char command[50];
				scanf("%s", command);
				handleCommand(command);
			}
			if (start) {
				time(&end); // sample new ending time
			}
		}
	}

	exit(0);
}
