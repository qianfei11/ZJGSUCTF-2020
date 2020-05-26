#!/usr/bin/env python
plaintext = 'zjgsuctf{vMfUn!}'

'''
def encode(arr):
    result = ''
    for i in range(16):
        if i % 5 == 0:
            c = ord(arr[i]) + 20
            result += chr(c)
        elif i % 5 == 1:
            c = ord(arr[i]) - 15
            result += chr(c)
        elif i % 5 == 2:
            c = ord(arr[i]) * 1
            result += chr(c)
        elif i % 5 == 3:
            c = ord(arr[i]) / 1
            result += chr(c)
        elif i % 5 == 4:
            c = ord(arr[i]) ^ 73
            result += chr(c)
    return result

ciphertext = encode(plaintext)
for c in ciphertext:
    print hex(ord(c))
print ciphertext

def decode(arr):
    result = ''
    for i in range(16):
        if i % 5 == 0:
            c = ord(arr[i]) - 20
            result += chr(c)
        elif i % 5 == 1:
            c = ord(arr[i]) + 15
            result += chr(c)
        elif i % 5 == 2:
            c = ord(arr[i]) / 1
            result += chr(c)
        elif i % 5 == 3:
            c = ord(arr[i]) * 1
            result += chr(c)
        elif i % 5 == 4:
            c = ord(arr[i]) ^ 73
            result += chr(c)
    return result

print decode(ciphertext)

for i in range(16):
    if i % 5 == 0:
        print 'MOV regs[0], vm_stack[{}]'.format(i)
        print 'ADD regs[0], 20'
        print 'MOV vm_stack[0x20+{}], regs[0]'.format(i)
    elif i % 5 == 1:
        print 'MOV regs[1], vm_stack[{}]'.format(i)
        print 'SUB regs[1], 16'
        print 'MOV vm_stack[0x20+{}], regs[1]'.format(i)
    elif i % 5 == 2:
        print 'MOV regs[2], vm_stack[{}]'.format(i)
        print 'MUL regs[2], 1'
        print 'MOV vm_stack[0x20+{}], regs[2]'.format(i)
    elif i % 5 == 3:
        print 'MOV regs[3], vm_stack[{}]'.format(i)
        print 'DIV regs[3], 1'
        print 'MOV vm_stack[0x20+{}], regs[3]'.format(i)
    elif i % 5 == 4:
        print 'MOV regs[4], vm_stack[{}]'.format(i)
        print 'XOR regs[4], 73'
        print 'MOV vm_stack[0x20+{}], regs[4]'.format(i)
'''

for i in range(16):
    if i % 5 == 0:
        print ' MOV, OP_REG, 0, {}, ADD, 0, 20, MOV, OP_MEM, 0, {},'.format(i, 0x20+i)
    elif i % 5 == 1:
        print ' MOV, OP_REG, 1, {}, SUB, 1, 15, MOV, OP_MEM, 1, {},'.format(i, 0x20+i)
    elif i % 5 == 2:
        print ' MOV, OP_REG, 2, {}, MUL, 2, 1, MOV, OP_MEM, 2, {},'.format(i, 0x20+i)
    elif i % 5 == 3:
        print ' MOV, OP_REG, 3, {}, DIV, 3, 1, MOV, OP_MEM, 3, {},'.format(i, 0x20+i)
    elif i % 5 == 4:
        print ' MOV, OP_REG, 4, {}, XOR, 4, 73, MOV, OP_MEM, 4, {},'.format(i, 0x20+i)
print ' RET'

