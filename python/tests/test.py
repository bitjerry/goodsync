# !/usr/bin/python3
# -*-coding: UTF-8-*-
"""
@Time: 2024-04-04 1:19
@Author: Mr.lin
@Version: v1
@File: test
"""

from goodsync import *

gs = GoodSync("0123456789")
print(gs.key)
encrypted_filename = gs.encrypt_name("test")
print(encrypted_filename)
filename = gs.decrypt_name(encrypted_filename)
print(filename)
print(filename.encode().hex())

import base64

c = base64.b64decode(gs.key[:8])
for i in c:
    print(hex(i), end=" ")
