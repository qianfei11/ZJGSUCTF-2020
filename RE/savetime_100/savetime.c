#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

char logo[0x100] = "\033[34m\n ____ _____       _\n| __ )___ /  __ _| | ___\n|  _ \\ |_ \\ / _` | |/ _ \\\n| |_) |__) | (_| | |  __/\n|____/____/ \\__,_|_|\\___|\n\033[1m";
uint8_t flag[0x100] = "zjgsuctf{fake_flag}";
uint8_t failed[0x20] = "Time is not right.";
uint8_t x[0x100] = {0x5a, 0x4b, 0x45, 0x50, 0x51, 0x46, 0x52, 0x41, 0x53, 0x5d, 0x43, 0x46, 0x49, 0x72, 0x5e, 0x1b, 0x43, 0x42, 0x1, 0x57, 0x6b, 0x57, 0x6f, 0x49, 0x46, 0x47, 0x47}; // zjgsuctf{time_p4ss3d_bY~~~}
uint8_t *success = x;

void welcome() {
    puts(logo);
}

int z() {
	time_t now = time(NULL), target = 1590969600;
    if (target - now >= 0) {
        return 0;
    } else {
        return 1;
    }
}

void timeup() {
    int i;
    for (i = 0; i < 0x1b; i++) {
        x[i] ^= (0x20 + i);
    }
}

int main() {
    welcome();
    if (z() == 1) {
        timeup();
        puts((char *)success);
    } else {
        puts((char *)failed);
    }
	return 0;
}

