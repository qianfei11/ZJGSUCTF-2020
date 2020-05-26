#!/usr/bin/env python
# -*- encoding=utf-8 -*-

ENCRYPT = 0
DECRYPT = 1
BLOCK_SIZE = 8

# 明文分组初始置换表
IP = [58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4, 62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16,
      8, 57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3, 61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7]
# 末置换表
IP_ = [40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31, 38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61,
       29, 36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27, 34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25]

# S盒压缩置换表（6bit->4bit）
S1 = [14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7, 0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3,
      8, 4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0, 15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13]
S2 = [15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10, 3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11,
      5, 0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15, 13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9]
S3 = [10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8, 13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15,
      1, 13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7, 1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12]
S4 = [7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15, 13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14,
      9, 10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4, 3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14]
S5 = [2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9, 14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8,
      6, 4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14, 11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3]
S6 = [12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11, 10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3,
      8, 9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6, 4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13]
S7 = [4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1, 13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8,
      6, 1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2, 6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12]
S8 = [13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7, 1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9,
      2, 7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8, 2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11]
S = [S1, S2, S3, S4, S5, S6, S7, S8]

# P盒置换表（32bit）
P = [16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31,
     10, 2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25]

# 密钥置换表（64bit->56bit）
PC_1 = [57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18, 10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44,
        36, 63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22, 14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4]
# 密钥压缩置换表（56bit->48bit）
PC_2 = [14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2,
        41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32]

# 明文扩展置换表（32bit->48bit）
E = [32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9, 8, 9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17, 16,
     17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25, 24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1]

# 子密钥循环左移位数表（16次）
SHIFT = [1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1]

sub_keys = [[0] * 48] * 16

def convert_string_to_bitlist(string):
    data = [ord(c) for c in string]
    l = len(data) * 8
    result = [0] * l
    pos = 0
    for ch in data:
        i = 7
        while i >= 0:
            if ch & (1 << i) != 0:
                result[pos] = 1
            else:
                result[pos] = 0
            pos += 1
            i -= 1
    return result

def convert_bitlist_to_string(bitlist):
    result = []
    pos = 0
    c = 0
    while pos < len(bitlist):
        c += bitlist[pos] << (7 - pos % 8)
        if pos % 8 == 7:
            result.append(c)
            c = 0
        pos += 1
    return ''.join([chr(c) for c in result])

def permute(table, block):
    return list(map(lambda x: block[x - 1], table))

def create_sub_keys(key):
    key = permute(PC_1, convert_string_to_bitlist(key))
    i = 0
    lkey = key[:28]
    rkey = key[28:]
    while i < 16:
        j = 0
        while j < SHIFT[i]:
            lkey.append(lkey[0])
            del lkey[0]
            rkey.append(rkey[0])
            del rkey[0]
            j += 1
        sub_keys[i] = permute(PC_2, lkey + rkey)
        # print sub_keys[i]
        i += 1

def des_cipher(block, choice):
    block = permute(IP, block)
    lblock = block[:32]
    rblock = block[32:]
    if choice == ENCRYPT:
        iteration = 0
        iteration_adjustment = 1
    elif choice == DECRYPT:
        iteration = 15
        iteration_adjustment = -1
    i = 0
    while i < 16:
        rtemp = rblock[:]
        rblock = permute(E, rblock)
        rblock = list(map(lambda x, y: x ^ y, rblock, sub_keys[iteration]))
        b = [rblock[:6], rblock[6:12], rblock[12:18], rblock[18:24],
             rblock[24:30], rblock[30:36], rblock[36:42], rblock[42:]]
        j = 0
        bn = [0] * 32
        pos = 0
        while j < 8:
            row = (b[j][0] << 1) + b[j][5]
            col = (b[j][1] << 3) + (b[j][2] << 2) + (b[j][3] << 1) + b[j][4]
            v = S[j][(16 * row) + col]
            bn[pos] = (v & 8) >> 3
            bn[pos + 1] = (v & 4) >> 2
            bn[pos + 2] = (v & 2) >> 1
            bn[pos + 3] = v & 1
            pos += 4
            j += 1
        rblock = permute(P, bn)
        rblock = list(map(lambda x, y: x ^ y, rblock, lblock))
        lblock = rtemp
        i += 1
        iteration += iteration_adjustment
    final = permute(IP_, rblock + lblock)
    return final

def cipher(data, key, iv, choice):
    if len(data) % BLOCK_SIZE != 0:
        raise ValueError("Data length illegal.")
    if len(key) != 8 or len(iv) != 8:
        raise ValueError("key/iv\'s length should be 8.")
    create_sub_keys(key)
    iv = convert_string_to_bitlist(iv)
    i = 0
    result = []
    while i < len(data):
        block = convert_string_to_bitlist(data[i:i + 8])
        if choice == ENCRYPT:
            block = list(map(lambda x, y: x ^ y, block, iv))
            block = des_cipher(block, choice)
            iv = block
        elif choice == DECRYPT:
            temp = block[:]
            block = des_cipher(block, choice)
            block = list(map(lambda x, y: x ^ y, block, iv))
            iv = temp
        result.append(convert_bitlist_to_string(block))
        i += 8
    return ''.join(result)

def encipher(data, key, iv):
    return cipher(data, key, iv, ENCRYPT).encode('base64')

def decipher(data, key, iv):
    return cipher(data.decode('base64'), key, iv, DECRYPT)

if __name__ == '__main__':
    key = '#qianfei'
    iv = '0' * 8
    data = '#qianfei11111111'
    ciphertext = encipher(data, key, iv)
    # print [ord(c) for c in ciphertext]
    print ciphertext
    plaintext = decipher(ciphertext, key, iv)
    # print [ord(c) for c in plaintext]
    print plaintext
