# eznative_400

这道题里 JNI 是用 Dynamic Register，所以定位要到 JNI_OnLoad 来找到对应的函数（比较有逻辑）。算法和前一题一样也是用对称密码，搜 S 盒的方法也有用。不过这里的算法用 [findcrypt](https://github.com/polymorf/findcrypt-yara) 可以直接找到是 Blowfish。大致可以看出 key 是 NULL，密文用 Base64 编码，同时采用 ECB 模式：

```cpp
  env_1 = env;
  plaintext_2 = plaintext;
  len_1 = len;
  v19 = 0LL;
  k = 0LL;
  buf = (char *)malloc(8 * len);
  plaintext_3 = get_string_utf_chars(env_1, (__int64)plaintext_2, 0LL);
  plaintext_1 = get_string_utf_chars(env_1, (__int64)plaintext_2, 0LL);
  __android_log_print(6LL, "MY_TAG", "%s", plaintext_1);
  key_setup((__int64)&k, (__int64)&key, 8uLL);
  for ( i = 0LL; i < len_1; ++i )
  {
    block_encrypt((unsigned int *)&plaintext_3[8 * i], block, &key);
    for ( j = 0LL; j <= 7; ++j )
    {
      buf[8 * i + j] = block[j];
      __android_log_print(6LL, "MY_TAG", "%02x", (unsigned __int8)buf[8 * i + j]);
    }
  }
  base64_result = base64_encrypt((__int64)buf, 8 * len_1, 0LL);
  base64_result_1 = new_string_utf(env_1, base64_result);
  base64_result_2 = get_string_utf_chars(env_1, (__int64)base64_result_1, 0LL);
  len_2 = strlen(base64_result_2);
  result = (jstring)__android_log_print(6LL, "MY_TAG", "length = %d", len_2);
```

脚本：

```python
#!/usr/bin/env python
from Crypto.Cipher import Blowfish

key = '\x00' * 8
ciphertext = 'iogHhlbYf5Iu87/J3DSiXjXZhCYeYwDVSahRn4hcB8l2xbmbkuEAUg=='
cipher = Blowfish.new(key, Blowfish.MODE_ECB)
plaintext = cipher.decrypt(ciphertext.decode('base64'))
print plaintext
```
