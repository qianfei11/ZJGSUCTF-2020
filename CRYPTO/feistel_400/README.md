# feistel_400

> 参考：护网杯 2018

```python
#!/usr/bin/env python
import os

def xor(a, b):
    assert len(a) == len(b)
    c = ''
    for i in range(len(a)):
        c += chr(ord(a[i]) ^ ord(b[i]))
    return c

def f(x, k):
    return xor(xor(x, k), 7)

def round(M, K):
    L = M[0:27]
    R = M[27:54]
    new_l = R
    new_r = xor(xor(R, L), K)
    return new_l + new_r

def fez(m, K):
    for i in K:
        m = round(m, i)
    return m

K = []
for i in range(7):
    K.append(os.urandom(27))
m = open('flag', 'rb').read()
assert len(m) < 54
m += os.urandom(54 - len(m))

test = os.urandom(54)
print test.encode('hex')
print fez(test, K).encode('hex')
print fez(m, K).encode('hex')
```

Feistel 结构，R 作为新的 L，L 和 R 两边和随机数进行 7 轮异或生成新的 R：

| round |            L            |                R                 |
| :---: | :---------------------: | :------------------------------: |
|   0   |           `l`           |               `r`                |
|   1   |           `r`           |           `r ^ l ^ k1`           |
|   2   |      `r ^ l ^ k1`       |          `l ^ k1 ^ k2`           |
|   3   |      `l ^ k1 ^ k2`      |          `r ^ k2 ^ k3`           |
|   4   |      `r ^ k2 ^ k3`      |      `r ^ l ^ k1 ^ k3 ^ k4`      |
|   5   | `r ^ l ^ k1 ^ k3 ^ k4`  |     `l ^ k1 ^ k2 ^ k4 ^ k5`      |
|   6   | `l ^ k1 ^ k2 ^ k4 ^ k5` |     `r ^ k2 ^ k3 ^ k5 ^ k6`      |
|   7   | `r ^ k2 ^ k3 ^ k5 ^ k6` | `r ^ l ^ k1 ^ k3 ^ k4 ^ k6 ^ k7` |

题目提供了一个 test，以及使用 K 对 test 产生的密文和对 flag 产生的密文，那么根据上面的推算，就能分别得出 k2356 和 k13467，从而计算出 flag。
