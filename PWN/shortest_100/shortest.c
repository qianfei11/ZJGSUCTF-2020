#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char *logo = "\033[34m\n ____ _____       _\n| __ )___ /  __ _| | ___\n|  _ \\ |_ \\ / _` | |/ _ \\\n| |_) |__) | (_| | |  __/\n|____/____/ \\__,_|_|\\___|\n\033[1m";
char buf[0x20];

void welcome() {
    puts(logo);
}

int z() {
    time_t now = time(NULL), target = 1590181200;
    int nbytes = (target - now) / 3600;
    if (nbytes > 20) {
        return 20;
    } else if (nbytes < 10) {
        return 10;
    } else {
        return nbytes;
    }
}

int main() {
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);
    welcome();
    void (*ptr)();
    printf("welcome>> ");
    z();
    read(0, buf, 4);
    ptr = buf;
    ptr();
}
