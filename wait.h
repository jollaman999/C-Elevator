#include <time.h>

// 밀리초 단위로 zzZ...
int wait(int mseconds)
{
	int time_end = 0;

	clock_t endwait;
	endwait = clock() + mseconds / 1000 * CLOCKS_PER_SEC;
	while (clock() < endwait);
	time_end = 1;

	return time_end;
}
