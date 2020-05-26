#include <time.h>
#include <stdio.h>

int main() {
	struct tm x = {
		.tm_year = 2020 - 1900,
		.tm_mon = 6 - 1,
		.tm_mday = 1,
		.tm_hour = 0,
		.tm_min = 0,
		.tm_sec = 0,
	};
	time_t y = mktime(&x);
	printf("%ld\n", y);
	struct tm *tm_now = localtime(&y);
	printf("%d-%d-%d %d:%d:%d\n", tm_now->tm_year+1900, tm_now->tm_mon+1, tm_now->tm_mday, tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec) ;
	time_t now = time(NULL);
	printf("NOW => %ld\n", now);
	tm_now = localtime(&now);
	printf("%d-%d-%d %d:%d:%d\n", tm_now->tm_year+1900, tm_now->tm_mon+1, tm_now->tm_mday, tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec) ;
	printf("result => %ld\n", (y - now) / 3600);
}
