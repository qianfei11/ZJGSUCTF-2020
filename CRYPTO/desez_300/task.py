#!/usr/bin/env python
import os
import pyDes
from secret import flag

key = os.urandom(8)

d = pyDes.des(key)
ciphertext = d.encrypt(flag.encode())

with open('ciphertext.txt', 'wb') as f:
    f.write(cipher)

# print d.Kn[11]
# [1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0]
