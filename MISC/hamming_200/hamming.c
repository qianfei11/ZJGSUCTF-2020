#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

char *logo = "\033[34m\n ____ _____       _\n| __ )___ /  __ _| | ___\n|  _ \\ |_ \\ / _` | |/ _ \\\n| |_) |__) | (_| | |  __/\n|____/____/ \\__,_|_|\\___|\n\033[5m";
char flag[0x100] = "zjgsuctf{b4sic_c0mput3r_kn0l3dge~}";

void welcome() {
    puts(logo);
}

char itoa(int a) {
    char x = (char)(a + 0x30);
    //printf("%c\n", x);
    return x;
}

int atoi_(char a) {
    int x = (int)a - 0x30;
    //printf("%d\n", x);
    return x;
}

int main() {
    int i, check, pos, count = 0;
    char c1, c2, c4;
    char *p, *q, *buf, *s;
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);
    welcome();
    srand(time(NULL));
    while (1) {
        printf("========== ROUND %d ==========\n", count);
        p = malloc(4);
        q = malloc(4);
        s = malloc(7);
        buf = malloc(4);
        assert(p && q && s && buf);
        for (i = 0; i < 4; i++) {
            q[3 - i] = itoa(rand() % 2);
            p[i] = q[3 - i];
        }
        q[i] = '\x00';
        //printf("q => %s\n", q);
        c1 = itoa(atoi_(p[3]) ^ atoi_(p[2]) ^ atoi_(p[0]));
        c2 = itoa(atoi_(p[3]) ^ atoi_(p[1]) ^ atoi_(p[0]));
        c4 = itoa(atoi_(p[2]) ^ atoi_(p[1]) ^ atoi_(p[0]));
        //printf("%c%c%c\n", c1, c2, c4);
        s[0] = c1;
        s[1] = c2;
        s[2] = p[3];
        s[3] = c4;
        s[4] = p[2];
        s[5] = p[1];
        s[6] = p[0];
        s[7] = '\x00';
        //printf("s => %s\n", s);
        pos = rand() % 7;
        s[pos] = itoa(atoi_(s[pos]) ^ 1);
        //printf("s => %s\n", s);
        printf("Infomation => %s\n", s);
        printf("The original data => ");
        scanf("%s", buf);
        check = strcmp(buf, q);
        free(p);
        free(q);
        free(buf);
        free(s);
        if (check == 0) {
            if (count == 100) {
                break;
            } else {
                count += 1;
            }
        } else {
            puts("[!] Wrong...");
            exit(-1);
        }
    }
    puts(flag);
    return 0;
}
