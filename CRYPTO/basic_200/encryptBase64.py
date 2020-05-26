#!/usr/bin/env python
import base64

with open('src.txt', 'r') as f:
    data = [i.strip() for i in f.readlines()]
base64Data = [base64.b64encode(i) for i in data]
#print base64Data
b64char = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/'

size = 0
for b in base64Data:
    if b.find('=='):
        size += 4
    elif b.find('='):
        size += 2
print size
msg = 'zjgsuctf{base64_c4n_6e_H4rd}'
msg_bit = ''.join([bin(ord(i))[2:].zfill(8) for i in msg])
print len(msg_bit)
offset = 0
res = []
for i in base64Data:
    if msg_bit == '':
        break
    if i[-2] == '=':
        offset = int(msg_bit[:4], 2)
        i = i.replace(i[-3], b64char[b64char.index(i[-3]) + offset])
        msg_bit = msg_bit[4:]
    elif i[-1] == '=':
        offset = int(msg_bit[:2], 2)
        i = i.replace(i[-2], b64char[b64char.index(i[-2]) + offset])
        msg_bit = msg_bit[2:]
    res.append(i + '\n')
with open('base64.txt', 'w') as f:
    f.writelines(res)

