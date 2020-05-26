# rcrc_300

rc2 对称加密，可以把 keygen 里的 S 盒拿去搜一下：

```
.data:00000000006020A0 byte_6020A0     db 0D9h                 ; DATA XREF: keygen+C8↑r
.data:00000000006020A0                                         ; keygen+12E↑r ...
.data:00000000006020A1                 db  78h ; x
.data:00000000006020A2                 db 0F9h
.data:00000000006020A3                 db 0C4h
.data:00000000006020A4                 db  19h
.data:00000000006020A5                 db 0DDh
.data:00000000006020A6                 db 0B5h
.data:00000000006020A7                 db 0EDh
.data:00000000006020A8                 db  28h ; (
.data:00000000006020A9                 db 0E9h
.data:00000000006020AA                 db 0FDh
.data:00000000006020AB                 db  79h ; y
.data:00000000006020AC                 db  4Ah ; J
.data:00000000006020AD                 db 0A0h
.data:00000000006020AE                 db 0D8h
.data:00000000006020AF                 db  9Dh
.data:00000000006020B0                 db 0C6h
.data:00000000006020B1                 db  7Eh ; ~
.data:00000000006020B2                 db  37h ; 7
.data:00000000006020B3                 db  83h
.data:00000000006020B4                 db  2Bh ; +
.data:00000000006020B5                 db  76h ; v
.data:00000000006020B6                 db  53h ; S
.data:00000000006020B7                 db  8Eh
.data:00000000006020B8                 db  62h ; b
.data:00000000006020B9                 db  4Ch ; L
.data:00000000006020BA                 db  64h ; d
.data:00000000006020BB                 db  88h
.data:00000000006020BC                 db  44h ; D
```

key 和 ciphertext 都在 main 函数里很好定位，根据 IDA 里的逻辑可以看出用的是 ECB 模式：

```python
#!/usr/bin/env python
from rc2 import *

key = 'haliluya'
rc2_ctx = RC2(bytearray(key, 'ascii'))
ciphertext = [20, 180, 57, 50, 17, 164, 240, 38, 110, 45, 59, 122, 186, 229, 114, 109, 246, 93, 201, 140, 108, 36, 16, 150, 114, 86, 247, 126, 89, 69, 210, 56, 231, 106, 211, 66, 71, 128, 38, 26, 144, 190, 253, 20, 191, 28, 52, 221]
ciphertext = ''.join(chr(c) for c in ciphertext)
plaintext = rc2_ctx.decrypt(ciphertext, MODE_ECB)
print plaintext
```
