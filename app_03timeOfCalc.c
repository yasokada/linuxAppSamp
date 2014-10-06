#include <stdio.h>
#include <time.h>
#include <unistd.h>

// -lrt

static int disp_precision(void)
{
	struct timespec res;

	if(clock_getres(CLOCK_REALTIME, &res) < 0) { /***/
		perror("clock_getres failed");
		return 1;
	}

	printf("Precision (sec) = %ld.%09ld\n", res.tv_sec, res.tv_nsec);
	return 0;
}

static void funcToBeTimed_floatAdd(void)
{
	int idx;
	float fval = 0.0;

	for(idx=0; idx<100; idx++) {
		fval = fval + 3.14;
	}
}

static void funcToBeTimed_floatMulti(void)
{
	int idx;
	float fval = 3.14;

	for(idx=0; idx<100; idx++) {
		fval = fval * 1.001;
	}
}

static void funcToBeTimed_floatDiv(void)
{
	int idx;
	float fval = 314159000.00;

	for(idx=0; idx<100; idx++) {
		fval = fval / 1.001;
	}
}

static void disp_elapsed(int procNo)
{
	struct timespec tp1, tp2;
	long dsec, dnsec;

	/***/
	switch(procNo) {
	case 0:
		printf("add\n");
		clock_gettime(CLOCK_REALTIME, &tp1);
		funcToBeTimed_floatAdd();
		clock_gettime(CLOCK_REALTIME, &tp2);
		break;
	case 1:
		printf("multi\n");
		clock_gettime(CLOCK_REALTIME, &tp1);
		funcToBeTimed_floatMulti();
		clock_gettime(CLOCK_REALTIME, &tp2);
		break;
	default:
		printf("div\n");
		clock_gettime(CLOCK_REALTIME, &tp1);
		funcToBeTimed_floatDiv();
		clock_gettime(CLOCK_REALTIME, &tp2);
		break;
	}

	dsec = tp2.tv_sec - tp1.tv_sec;
	dnsec = tp2.tv_nsec - tp1.tv_nsec;
	if (dnsec < 0) {
		dsec--;
		dnsec += 1000000000L;
	}

	printf("Epalsed (nsec) = %ld\n", dnsec);
	printf("Epalsed (usec) = %ld\n", (long)dnsec / 1000L);
	printf("Epalsed (msec) = %ld\n", (long)dnsec / 1000L / 1000L);
}

int main(int argc, char *argv[])
{
	int procNo;

	if (argc <= 1) {
		printf("[cmd] [No]\n");
		return;
	}

	sscanf(argv[1], "%d", &procNo);

	if (disp_precision() != 0) {
		return 1;
	}

	disp_elapsed(procNo);

	return 0;
}