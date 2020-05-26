# desez_300

> 参考：NCTF 2019

泄漏了 DES 的某个子密钥（48 bits），通过爆破 8 bits 来还原主密钥：

```python
#!/usr/bin/env python
from des import *
from itertools import product

sub_key_12 = [1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0]

def bruteforce_sub_key(sub_key, idx):
    inversed = [0] * 56
    for i in range(len(sub_key)):
        inversed[PC_2[i] - 1] = sub_key[i]
    guess_pos = [9, 18, 22, 25, 35, 38, 43, 54]
    guess_list = list(product(range(2), repeat=8))
    for j in range(8):
        inversed[guess_pos[j] - 1] = guess_list[idx][j]
    return inversed

def get_sub_keys(single_key, rounds):
    lkey = single_key[:28]
    rkey = single_key[28:]
    i = rounds + 1
    while i < rounds + 16:
        j = 0
        while j < SHIFT[i % 16]:
            lkey.append(lkey[0])
            del lkey[0]
            rkey.append(rkey[0])
            del rkey[0]
            j += 1
        sub_keys[i % 16] = permute(PC_2, lkey + rkey)
        i += 1

ciphertext = open('ciphertext.txt', 'rb').read()
print len(ciphertext)

def bruteforce_plaintext():
    rounds = 11
    sub_keys[rounds] = sub_key_12
    for i in range(2 ** 8):
        single_key = bruteforce_sub_key(sub_key_12, i)
        get_sub_keys(single_key, rounds)
        plaintext = ''
        for j in range(len(ciphertext) / 8):
            block = convert_string_to_bitlist(ciphertext[8 * j:8 * j + 8])
            plaintext += convert_bitlist_to_string(des_cipher(block, DECRYPT))
        if plaintext.startswith('zjgsuctf'):
            print plaintext
            return True
    return False

assert bruteforce_plaintext()
```
