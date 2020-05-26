#!/usr/bin/env python
import base64
import re

b64char = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/'

with open('base64.txt', 'r') as f:
    content = f.readlines()
#print content

res = ''
plaintext = ''
for l in content:
    line = l.replace('\n', '')
    if line[-2] == '=':
        msg = bin(b64char.index(line[-3]))[2:].zfill(4)
        plaintext += msg[-4:]
    elif line[-1] == '=':
        msg = bin(b64char.index(line[-2]))[2:].zfill(2)
        plaintext += msg[-2:]
    t = base64.b64decode(line)
    res += t
print res
print plaintext
plaintext = re.findall('.{8}', plaintext)
plaintext = ''.join([chr(int(i, 2)) for i in plaintext])
print plaintext
