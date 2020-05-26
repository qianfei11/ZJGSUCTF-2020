#!/usr/bin/env python
from rc2 import *

key = 'haliluya'
rc2_ctx = RC2(bytearray(key, 'ascii'))
ciphertext = [20, 180, 57, 50, 17, 164, 240, 38, 110, 45, 59, 122, 186, 229, 114, 109, 246, 93, 201, 140, 108, 36, 16, 150, 114, 86, 247, 126, 89, 69, 210, 56, 231, 106, 211, 66, 71, 128, 38, 26, 144, 190, 253, 20, 191, 28, 52, 221]
ciphertext = ''.join(chr(c) for c in ciphertext)
plaintext = rc2_ctx.decrypt(ciphertext, MODE_ECB)
print plaintext

