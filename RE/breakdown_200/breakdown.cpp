#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/sha.h>
using namespace std;

static char logo[0x100] = "\033[34m\n ____ _____       _\n| __ )___ /  __ _| | ___\n|  _ \\ |_ \\ / _` | |/ _ \\\n| |_) |__) | (_| | |  __/\n|____/____/ \\__,_|_|\\___|\n\033[1m";
static std::string target = "d4f7fa60a88997834870611851d5d8a1797ba4794dbc4b91a140b4b09785a246";

void welcome() {
    cout << logo << endl;
}

int read_int() {
    char buf[8];
    int x;
    read(0, buf, 8);
    x = atoi(buf);
    return x;
}

void check(std::string s) {
    if (s == target) {
        cout << "[*] Success!" << endl;
    } else {
        cout << "[*] Failed..." << endl;
    }
}

// https://my.oschina.net/bobwei/blog/524147
// https://www.systutorials.com/convert-string-to-int-and-reverse/
int main() {
    std::stringstream s;
    std::string y, r;
    int x, i;
    unsigned char hash[SHA256_DIGEST_LENGTH];
    char buf[2];
    welcome();
    cout << "What's the number? " << endl;
    x = read_int();
    s << x;
    y = "zjgsuctf{" + s.str() + "}";
    //cout << y << endl;
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, y.c_str(), y.size());
    SHA256_Final(hash, &sha256);
    for (i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(buf, "%02x", hash[i]);
        r = r + buf;
    }
    //cout << r << endl;
    check(r);
    return 0;
}
