#!/usr/bin/env python

ciphertext = [0x8E, 0x5B, 0x67, 0x73, 0x3C, 0x77, 0x65, 0x66, 0x7B, 0x3F, 0x61, 0x57, 0x55, 0x6E, 0x68, 0x91]

def decode(arr):
    result = ''
    for i in range(16):
        if i % 5 == 0:
            c = arr[i] - 20
            result += chr(c)
        elif i % 5 == 1:
            c = arr[i] + 15
            result += chr(c)
        elif i % 5 == 2:
            c = arr[i] / 1
            result += chr(c)
        elif i % 5 == 3:
            c = arr[i] * 1
            result += chr(c)
        elif i % 5 == 4:
            c = arr[i] ^ 73
            result += chr(c)
    return result

print decode(ciphertext)

