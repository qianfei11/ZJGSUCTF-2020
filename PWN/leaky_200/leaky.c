#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

char *logo = "\033[34m\n ____ _____       _\n| __ )___ /  __ _| | ___\n|  _ \\ |_ \\ / _` | |/ _ \\\n| |_) |__) | (_| | |  __/\n|____/____/ \\__,_|_|\\___|\n\033[1m";
char buf[0x20];

void welcome() {
	puts(logo);
}

int main() {
	setvbuf(stdin, 0, 2, 0);
	setvbuf(stdout, 0, 2, 0);
	setvbuf(stderr, 0, 2, 0);
    char buf[0x20], num[0x20];
	welcome();
    srand(time(NULL));
    sprintf(num, "%d", rand());
    printf("Give you a secret number: %s\n", num);
    printf("Show me what you got: ");
    read(0, buf, 0x200);
}
