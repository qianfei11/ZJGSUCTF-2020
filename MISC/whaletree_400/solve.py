#!/usr/bin/env python
import os
import hashlib

hashmd5 = lambda m: hashlib.md5(m).hexdigest()

x = '2d7b81239b77deb7fbda026d9521939b'
y = '49390dd9695e7ab7c49fbea6697bc1a9'

def tree(filePath):
    files = os.listdir(filePath)
    for f in files:
        newFilePath = os.path.join(filePath, f)
        if os.path.isdir(newFilePath):
            tree(newFilePath)
        elif newFilePath.endswith('.jpg'):
            with open(newFilePath, 'rb') as d:
                e = hashmd5(d.read())
                if e != x and e != y:
                    print newFilePath
                    print e
        else:
            print '???'

tree('TREE')
# ZJGSUCTF{T2eeeeeee3_&wh4le_aka_d0cker}

