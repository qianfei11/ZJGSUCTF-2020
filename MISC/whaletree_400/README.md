# whaletree_400

> 参考：[riceteacatpanda 2020](https://ctftime.org/task/10324)

根据提示在 Docker Hub 找到镜像：

```bash
docker pull assassinq/whale-and-tree:latest
```

新开一个容器可以看到在 `/root` 下有一个 TREE 文件夹：

```bash
$ tree ./TREE/ | grep jpg | head
|-- 01T9URRTFS.jpg
|-- 055DOEO.jpg
|-- 06ZVJYB.jpg
|-- 08Q3QAZT.jpg
|-- 0BBFRQL.jpg
|-- 0BMININZ.jpg
|-- 0BUUN8RGL.jpg
|-- 0CYEN.jpg
|-- 0D2KQZM1.jpg
|-- 0D3WH.jpg
```

用哈希来定位到一个不一样的图片（除了 `2d7b81239b77deb7fbda026d9521939b` 和 `49390dd9695e7ab7c49fbea6697bc1a9` 之外）：

```bash
$ python
Python 2.7.12 (default, Apr 15 2020, 17:07:12)
[GCC 5.4.0 20160609] on linux2
Type "help", "copyright", "credits" or "license" for more information.
>>> #!/usr/bin/env python
... import os
>>> import hashlib
>>>
>>> hashmd5 = lambda m: hashlib.md5(m).hexdigest()
>>>
>>> x = '2d7b81239b77deb7fbda026d9521939b'
>>> y = '49390dd9695e7ab7c49fbea6697bc1a9'
>>>
>>> def tree(filePath):
...     files = os.listdir(filePath)
...     for f in files:
...         newFilePath = os.path.join(filePath, f)
...         if os.path.isdir(newFilePath):
...             tree(newFilePath)
...         elif newFilePath.endswith('.jpg'):
...             with open(newFilePath, 'rb') as d:
...                 e = hashmd5(d.read())
...                 if e != x and e != y:
...                     print newFilePath
...                     print e
...         else:
...             print '???'
...
>>> tree('TREE')
TREE/789I7U7/ENP92.jpg
56985867273c227f6675cb73e644b44e
```

用 `docker cp` 把图片复制出来就能看到 flag。
