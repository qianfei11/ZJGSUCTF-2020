#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <assert.h>

int count = 2;
void *tps_ptr;

char logo[0x100] = "\033[34m\n ____ _____       _\n| __ )___ /  __ _| | ___\n|  _ \\ |_ \\ / _` | |/ _ \\\n| |_) |__) | (_| | |  __/\n|____/____/ \\__,_|_|\\___|\n\033[1m";
char receipt[0x100] = "                   _       _\n _ __ ___  ___ ___(_)_ __ | |_\n| '__/ _ \\/ __/ _ \\ | '_ \\| __|\n| | |  __/ (_|  __/ | |_) | |_\n|_|  \\___|\\___\\___|_| .__/ \\__|\n                    |_|\n";

void *name;
void (*func)(void);

void welcome() {
    puts(logo);
}

struct books {
    int size;
    char *content;
};

struct books bs[3];

void read_buf(char *buf, int len) {
    if (read(0, buf, len) <= 0) {
        puts("Something wrong with the store system...");
        exit(-1);
    }
}

void write_buf(char *buf, int len) {
    write(1, buf, len);
}

void intro() {
    printf("What's your name? ");
    name = malloc(0x20);
    read_buf(name, sizeof(name));
}

int read_int() {
    char buf[0x10];
    read(0, buf, sizeof(buf));
    return atoi(buf);
}

int read_idx() {
    char buf[0x10];
    int idx;
    read(0, buf, sizeof(buf));
    idx = atoi(buf);
    if (idx < 1 || idx > 4) {
        puts("No way");
        exit(-1);
    }
    return idx - 1;
}

int read_bool() {
    char buf[0x20];
    while (1) {
        read(0, buf, sizeof(buf));
        if (buf[0] == 'y') {
            return 1;
        } else if (buf[0] == 'n') {
            return 0;
        } else {
            puts("This is a wrong choice...");
        }
    }
}

void menu() {
    puts("📖   Book Store   📖");
    puts("📖                📖");
    puts("📖  1. Buy        📖");
    puts("📖  2. Sell       📖");
    puts("📖  3. Write      📖");
    puts("📖  4. Read       📖");
    puts("📖  5. Exit       📖");
    puts("📖                📖");
    printf("Your choice: ");
}

void buy() {
    int idx, size, c;
    void *buf;
    printf("This is your 1st/2nd/3rd book? ");
    idx = read_idx();
    printf("How long is your content? ");
    size = read_int();
    if (size <= 0x7F) {
        puts("How can a book be that short???");
        exit(-1);
    }
    // enable largebins attack
    //if (size > 0x400) {
    //    puts("I know what you are thinking...");
    //    exit(-1);
    //}
    bs[idx].size = size;
    buf = calloc(1, size);
    assert(buf);
    printf("Please input your content: ");
    read_buf(buf, size);
    bs[idx].content = buf;
    printf("Do you want a receipt? [y/n] ");
    c = read_bool();
    if (c == 1) {
        puts("Here is your receipt.");
        puts(receipt);
    } else {
        puts("OK. Welcome next time!");
    }
}

void sell() {
    int idx;
    printf("This is your 1st/2nd/3rd book? ");
    idx = read_idx();
    if (!bs[idx].content) {
        puts("No way...");
        return;
    }
    free(bs[idx].content);
    // give an uaf
    //bs[idx].size = 0;
    //bs[idx].content = 0;
}

void write_book() {
    int idx;
    printf("This is your 1st/2nd/3rd book? ");
    idx = read_idx();
    if (!bs[idx].content) {
        puts("No way...");
        return;
    }
    printf("Please input your content: ");
    read_buf(bs[idx].content, bs[idx].size);
}

void read_book() {
    int idx;
    printf("This is your 1st/2nd/3rd book? ");
    idx = read_idx();
    if (!bs[idx].content) {
        puts("No way...");
        return;
    }
    printf("Hi, %s! Here is your book's content: ", (char *)name);
    puts(bs[idx].content);
}

void secret() {
    void *buf;
    char hint[0x50];
    if (*(uint16_t *)tps_ptr < 6) {
        puts("That's no no...");
        exit(-1);
    }
    if (count <= 0 || count > 2) {
        puts("No more chances.");
        exit(-1);
    } 
    puts("Here is a magic place.");
    buf = malloc(0x233);
    assert(buf);
    read_buf(buf, 0x233);
    sprintf(hint, "⌚️ %d times left...", count--);
    puts(hint);
    puts(buf);
}

void get_ptr() {
    void *ptr = malloc(0x1000);
    assert(ptr);
    free(ptr);
    tps_ptr = (void *)((uint64_t)ptr & 0xFFFFFFFFFFFFF000) + 0x32 + 0x22; // glibc-2.31
//    printf("%p\n", tps_ptr);
//    printf("%d\n", *(uint16_t *)tps_ptr);
//    ptr = malloc(0x233);
//    free(ptr);
//    printf("%d\n", *(uint16_t *)tps_ptr);
}

int main() {
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);
    int c;
    welcome();
    intro();
    get_ptr();
    while (1) {
        menu();
        c = read_int();
        switch (c) {
            case 1:
                buy();
                break;
            case 2:
                sell();
                break;
            case 3:
                write_book();
                break;
            case 4:
                read_book();
                break;
            case 5:
                puts("👄 Bye~");
                exit(-1);
            case 0xdeadbeef:
                secret();
                break;
            default:
                break;
        }
    }
    return 0;
}
