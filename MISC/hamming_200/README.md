# hamming_200

> 参考：[ENCRYPT CTF 2019](https://ctftime.org/task/7999)

74 汉明码，给 7 个 bit，还原出正确的原文。这道题比较好心没有设 alarm，而且只有 100 轮，看 WP 很多人都是手输的。我赋的脚本是用 pwntools 写的，其实还可以用 socket。这里放一下 [@qffnb（@nyqx）](https://nyqx.gitee.io/vuepress/)的脚本：

```python
import socket

# 汉明码答案(7,4)
def hamming(socket_str):
    c1 = int(socket_str[0])
    c2 = int(socket_str[1])
    c3 = int(socket_str[2])
    c4 = int(socket_str[3])
    c5 = int(socket_str[4])
    c6 = int(socket_str[5])
    c7 = int(socket_str[6])

    C1 = (c1 + c3 + c5 + c7) % 2
    C2 = (c2 + c3 + c6 + c7) % 2
    C3 = (c4 + c5 + c6 + c7) % 2
    P = C3 * 4 + C2 * 2 + C1

    if P == 3:
        c3 = (c3 + 1) % 2
    if P == 5:
        c5 = (c5 + 1) % 2
    if P == 6:
        c6 = (c6 + 1) % 2
    if P == 7:
        c7 = (c7 + 1) % 2

    return str(c3) + str(c5) + str(c6) + str(c7) + '\n'

# 连接服务端
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect(('10.21.13.112', 11002))
# 先接收信息
print(sock.recv(1024).decode())
# 按个回车，告诉对方我来了
sock.send('\n'.encode())
# 然后对方就给出第一题
ham_string = sock.recv(1024).decode()
print(ham_string)
# 根据第一题，给它答案
sock.send(hamming(ham_string[45:52]).encode())
# 设置超时时间
sock.settimeout(5)
# 等待对方第二题
ham_string = sock.recv(1024).decode()
print(ham_string)
# 这里发现还要再接收一次，但是没有关系
ham_string = sock.recv(1024).decode()
print(ham_string)

# 因为第一次接收的信息更多，所以提出去
# 接下来返回的信息格式是一样的，就可以开始循环
# 重复它个一百遍
for i in range(100):
    sock.send(hamming(ham_string[14:21]).encode())
    sock.settimeout(5)
    ham_string = sock.recv(1024).decode()
    print(ham_string)
    ham_string = sock.recv(1024).decode()
    print(ham_string)
# 有始有终
sock.close()
```
