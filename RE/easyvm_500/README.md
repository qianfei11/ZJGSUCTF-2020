# easyvm_500

参考了[广外 zs0zrc 的文章](https://xz.aliyun.com/t/3851)实现的一道题，没接触过的话逆起来应该会比较困难。实际的加密算法没有很麻烦，但单纯 IDA 逆逻辑还是比较复杂的。脚本：

```python
#!/usr/bin/env python
ciphertext = [0x8E, 0x5B, 0x67, 0x73, 0x3C, 0x77, 0x65, 0x66, 0x7B, 0x3F, 0x61, 0x57, 0x55, 0x6E, 0x68, 0x91]

def decode(arr):
    result = ''
    for i in range(16):
        if i % 5 == 0:
            c = arr[i] - 20
            result += chr(c)
        elif i % 5 == 1:
            c = arr[i] + 15
            result += chr(c)
        elif i % 5 == 2:
            c = arr[i] / 1
            result += chr(c)
        elif i % 5 == 3:
            c = arr[i] * 1
            result += chr(c)
        elif i % 5 == 4:
            c = arr[i] ^ 73
            result += chr(c)
    return result

print decode(ciphertext)
```
