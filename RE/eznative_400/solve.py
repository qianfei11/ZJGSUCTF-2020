#!/usr/bin/env python
from Crypto.Cipher import Blowfish

#flag = 'zjgsuctf{bl0wf15h_bl0ws_y0U_4way~~~}'
key = '\x00' * 8
ciphertext = 'iogHhlbYf5Iu87/J3DSiXjXZhCYeYwDVSahRn4hcB8l2xbmbkuEAUg=='
cipher = Blowfish.new(key, Blowfish.MODE_ECB)
plaintext = cipher.decrypt(ciphertext.decode('base64'))
print plaintext

