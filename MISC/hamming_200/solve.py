#!/usr/bin/env python
from pwn import *

context.log_level = 'debug'

#r = process('./hamming')
r = remote('10.21.13.112', 11002)

while True:
    r.recvuntil('Infomation => ')
    ham = r.recvuntil('\n', drop=True)
    print ham
    hamming = []
    for c in ham:
        hamming.append(int(c))
    print hamming
    c1, c2, c4 = hamming[0], hamming[1], hamming[3]
    b4, b3, b2, b1 = hamming[2], hamming[4], hamming[5], hamming[6]
    p4 = c4 ^ b3 ^ b2 ^ b1
    p2 = c2 ^ b4 ^ b2 ^ b1
    p1 = c1 ^ b4 ^ b3 ^ b1
    print p4, p2, p1
    pos = p4 * (2 ** 2) + p2 * (2 ** 1) + p1 * (2 ** 0)
    print pos
    if pos == 0 or pos == 1 or pos == 2 or pos == 4:
        res = str(b4) + str(b3) + str(b2) + str(b1)
    else:
        if pos == 3:
            b4 ^= 1
        if pos == 5:
            b3 ^= 1
        if pos == 6:
            b2 ^= 1
        if pos == 7:
            b1 ^= 1
        res = str(b4) + str(b3) + str(b2) + str(b1)
    print res
    r.recvuntil('The original data => ')
    r.sendline(res)
r.interactive()
