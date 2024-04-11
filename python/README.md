# GoodSync

Reverse engineering of GoodSync file name encryption and decryption algorithms

This project will help GoodSync users switch to other synchronization tools more freely without being bound by GoodSync

[GITHUB](https://github.com/bitjerry/goodsync) | [AUTHOR](https://github.com/bitjerry)

## Usage

```python
from goodsync import *

gs = GoodSync("0123456789")
print(gs.key)

encrypted_filename = gs.encrypt_name("test")
print(encrypted_filename)

filename = gs.decrypt_name(encrypted_filename)
print(filename)
```

## ⚖️License

---
Apache-2.0 © [bitjerry](https://github.com/bitjerry/goodsync/blob/main/LICENSE)

*2024/4/1*
*Mr.lin*