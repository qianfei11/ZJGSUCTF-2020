# basic_200

原理是 [Base64 隐写](https://www.tr0y.wang/2017/06/14/Base64steg/index.html)，因为 Base64 后面需要补 0 来对齐，在等号前面的部分有机会塞 2 个或者 4 个 bit，通过一大段 Base64 就能隐藏到很多 bit。

```python
#!/usr/bin/env python
import base64
import re

b64char = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/'

with open('base64.txt', 'r') as f:
    content = f.readlines()
#print content

res = ''
plaintext = ''
for l in content:
    line = l.replace('\n', '')
    if line[-2] == '=':
        msg = bin(b64char.index(line[-3]))[2:].zfill(4)
        plaintext += msg[-4:]
    elif line[-1] == '=':
        msg = bin(b64char.index(line[-2]))[2:].zfill(2)
        plaintext += msg[-2:]
    t = base64.b64decode(line)
    res += t
print res
print plaintext
plaintext = re.findall('.{8}', plaintext)
plaintext = ''.join([chr(int(i, 2)) for i in plaintext])
print plaintext
```
