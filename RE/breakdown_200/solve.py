#!/usr/bin/env python
import hashlib

i = 0
while True:
    flag = 'zjgsuctf{' + str(i) + '}'
    if hashlib.sha256(flag).hexdigest() == 'd4f7fa60a88997834870611851d5d8a1797ba4794dbc4b91a140b4b09785a246':
        print flag
        break
    i += 1

