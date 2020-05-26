# breakdown_200

> 参考：ZJGSUCTF 2019

代码逻辑很简单，输入数字然后带上 flag 前后部分进行 Sha256：

```cpp
  v4 = std::operator<<<std::char_traits<char>>(&_bss_start, "What's the number? ");
  std::ostream::operator<<(v4, &std::endl<char,std::char_traits<char>>);
  num = read_int();
  std::ostream::operator<<((__int64)&v19, num);
  std::__cxx11::basic_stringstream<char,std::char_traits<char>,std::allocator<char>>::str(&v12, &s);
  std::operator+<char,std::char_traits<char>,std::allocator<char>>(&v13, "zjgsuctf{", &v12);
  std::operator+<char,std::char_traits<char>,std::allocator<char>>(&flag, &v13, "}");
  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::operator=(&y, &flag);
  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::~basic_string(&flag);
  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::~basic_string(&v13);
  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::~basic_string(&v12);
  SHA256_Init(&sha256);
  size = std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::size(&y);
  y_1 = std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::c_str(&y);
  SHA256_Update(&sha256, y_1, size);
  SHA256_Final(hash, &sha256);
  for ( i = 0; i <= 31; ++i )
  {
    sprintf(&buf, "%02x", (unsigned __int8)hash[i]);
    std::operator+<char,std::char_traits<char>,std::allocator<char>>(&v15, &r, &buf);
    std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::operator=(&r, &v15);
    std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::~basic_string(&v15);
  }
  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(&r_1, &r);
  check((__int64)&r_1);
```

直接爆破：

```python
#!/usr/bin/env python
import hashlib

i = 0
while True:
    flag = 'zjgsuctf{' + str(i) + '}'
    if hashlib.sha256(flag).hexdigest() == 'd4f7fa60a88997834870611851d5d8a1797ba4794dbc4b91a140b4b09785a246':
        print flag
        break
    i += 1
```
