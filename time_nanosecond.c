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

static void funcToBeTimed(void)
{
	usleep(100000);
}

static void disp_elapsed(void)
{
	struct timespec tp1, tp2;
	long dsec, dnsec;

	/***/
	clock_gettime(CLOCK_REALTIME, &tp1);
	funcToBeTimed();
	clock_gettime(CLOCK_REALTIME, &tp2);

	dsec = tp2.tv_sec - tp1.tv_sec;
	dnsec = tp2.tv_nsec - tp1.tv_nsec;
	if (dnsec < 0) {
		dsec--;
		dnsec += 1e9L;
	}

	printf("Epalsed (sec) = %ld.%09ld\n", dsec, dnsec);
}

int main(void)
{
	if (disp_precision() != 0) {
		return 1;
	}

	disp_elapsed();

	return 0;
}