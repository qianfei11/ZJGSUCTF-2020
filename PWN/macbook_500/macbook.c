#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUY_CEILING 3
#define EXCHANGE_CEILING 2

char *logo = "\033[34m\n ____ _____       _\n| __ )___ /  __ _| | ___\n|  _ \\ |_ \\ / _` | |/ _ \\\n| |_) |__) | (_| | |  __/\n|____/____/ \\__,_|_|\\___|\n\033[1m";

int sum_of_macbook = 0;
int times_of_exchange = 0;

struct touchbar {
    int have_touchbar;
    int tb_version;
};

struct macbook {
    struct touchbar *tb;
    char *configuration;
};

struct macbook *mbs;

void welcome() {
    puts(logo);
}

void read_buf(char *buf, int len) {
    if (read(0, buf, len) <= 0) {
        puts("Something wrong with the store system...");
        exit(-1);
    }
}

int read_int() {
    char buf[0x10];
    read(0, buf, sizeof(buf));
    return atoi(buf);
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
    puts("  Welcome to Apple Store!");
    puts("+-------------------------+");
    puts("|                         |");
    puts("|  1. Buy a MacBook       |");
    puts("|  2. Exchange a MacBook  |");
    puts("|  3. Sell a MacBook      |");
    puts("|  4. Open your Macbook   |");
    puts("|  5. Leave the store     |");
    puts("|                         |");
    puts("+-------------------------+");
    printf("What's your choice? ");
}

char *tb_versions[] = {"A1706, A1989, A2159", "A1707, A1990", "A2141"};

void touchbar_menu() {
    puts("===========================");
    printf("   1. %s  \n", tb_versions[0]);
    printf("   2. %s         \n", tb_versions[1]);
    printf("   3. %s                \n", tb_versions[2]);
    puts("===========================");
    printf("What's your choice ?");
}

void buy() {
    struct macbook *mb;
    struct touchbar *tb;
    int sz, c;
    if (sum_of_macbook > BUY_CEILING) {
        puts("You do not have that much money..");
        exit(-1);
    }
    mb = malloc(0x10);
    printf("Length of your configuration: ");
    sz = read_int();
    if (sz > 0x1000) {
        puts("No such perfect MacBook in the world!");
        exit(-1);
    }
    mb->configuration = malloc(sz);
    if (!mb->configuration) {
        puts("Something wrong with the store system...");
        exit(-1);
    }
    printf("Give me your configuration: ");
    read_buf(mb->configuration, sz);
    tb = calloc(1, 8);
    printf("Do you want a touchbar? [y/n] ");
    tb->have_touchbar = read_bool();
    if (tb->have_touchbar) {
        touchbar_menu();
        while (1) {
            c = read_int();
            if (c < 1 || c > 3) {
                puts("There is no such version...");
                continue;
            } else {
                puts("Good choice!");
                tb->tb_version = c;
                break;
            }
        }
        puts("This is your macbook => 💻.");
    } else {
        tb->tb_version = 0;
        puts("This is your macbook => 🖥️.");
    }
    mb->tb = tb;
    mbs = mb;
    sum_of_macbook++;
    puts("Welcome next time!");
}

void exchange() {
    struct touchbar *tb;
    int sz, c;
    if (times_of_exchange > EXCHANGE_CEILING) {
        puts("Customer service do not want to see you again!");
    }
    if (!mbs) {
        puts("You did not buy one???");
    }
    printf("Length of your configuration :");
    sz = read_int();
    if (sz > 0x1000) {
        puts("No such perfect MacBook in the world!");
        exit(-1);
    }
    printf("Give me your configuration :");
    read_buf(mbs->configuration, sz);
    tb = mbs->tb;
    printf("Do you want a touchbar [y/n]? ");
    tb->have_touchbar = read_bool();
    if (tb->have_touchbar) {
        touchbar_menu();
        while (1) {
            c = read_int();
            if (c < 1 || c > 3) {
                puts("There is no such version...");
                continue;
            } else {
                puts("Good choice!");
                tb->tb_version = c;
                break;
            }
        }
        puts("This is your new macbook => 💻.");
    } else {
        tb->tb_version = 0;
        puts("This is your new macbook => 🖥️.");
    }
    times_of_exchange++;
}

void sell() {
    puts("Once you buy it, you can not sell it!");
}

void open() {
    if (!mbs) {
        puts("You did not buy one???");
    }
    printf("This is your MacBook's configuration: %s\n", mbs->configuration);
    if (mbs->tb->have_touchbar) {
        printf("And you have a touchbar: %s\n", tb_versions[mbs->tb->tb_version]);
    }
}

int main() {
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);
    int c;
    welcome();
    while (1) {
        menu();
        c = read_int();
        switch (c) {
            case 1:
                buy();
                break;
            case 2:
                exchange();
                break;
            case 3:
                sell();
                break;
            case 4:
                open();
                break;
            case 5:
                puts("Bye~");
                exit(0);
            default:
                puts("Wrong choice...");
                break;
        }
    }
    return 0;
}

