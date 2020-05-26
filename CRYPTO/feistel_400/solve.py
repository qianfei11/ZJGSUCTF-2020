#!/usr/bin/env python

def xor(a, b):
    assert len(a) == len(b)
    c = ""
    for i in range(len(a)):
        c += chr(ord(a[i]) ^ ord(b[i]))
    return c

f = open('task.log', 'rb')

test = f.readline().strip('\n')
enc1 = f.readline().strip('\n')
enc2 = f.readline().strip('\n')

t_l = test[:54].decode('hex')
t_r = test[54:].decode('hex')

k2356 = xor(enc1[:54].decode('hex'), t_r)
k13467 = xor(xor(enc1[54:].decode('hex'), t_r), t_l)

m_r = xor(enc2[:54].decode('hex'), k2356)
m_l = xor(xor(enc2[54:].decode('hex'), m_r), k13467)

print m_l + m_r
