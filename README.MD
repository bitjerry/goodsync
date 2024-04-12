# GoodSync

Reverse engineering of GoodSync file name encryption and decryption algorithms  
Help GoodSync users switch to other synchronization tools more freely without being bound by GoodSync

The project only implements encryption and decryption functions, and does not include directory or file processing,
synchronization and other functions. If you have good ideas, welcome to PR and let us improve the code together.

We also provide python extensions for easy embedding into python
projects. If you have implementations in other languages, you are welcome to PR

Read in other languages: English | [简体中文](README.ZH-CN.MD)

## Build

### Prepare

Install openssl development package

### c

Configure the environment variable

- `OPENSSL_INCLUDE_DIR`: Openssl include folder path. For example, on Windows it
  is `C:\Program Files\OpenSSL-Win64\include`
- `OPENSSL_LIBRARY_DIR`: Openssl library folder path. For example, on Windows it is `C:\Program Files\OpenSSL-Win64\lib`
- `OPENSSL_LIBRARIES`: Dependency library name. Usually, just `crypto` is OK

Configure your cmake tool

```shell
cmake -B ./cmake_build
cmake --build ./cmake_build --config Release
```

### python

#### Note

If you don't know enough about C and Python, I recommend using
the [released](https://github.com/bitjerry/goodsync/releases) wheel.

Usually you don't need to configure environment variables, the builder will automatically look for the location of
openssl.

By default, it is dynamically linked, you need to use `auditwheel` or similar tools to fix it, this is common sense
about python packaging, if you plan to build it you should get this knowledge.

#### Environment Variables ( Optional )

- `OPENSSL_INCLUDE_DIR`: Openssl include folder path. For example, on Windows it
  is `C:\Program Files\OpenSSL-Win64\include`
- `OPENSSL_LIBRARY_DIR`: Openssl library folder path. For example, on Windows it is `C:\Program Files\OpenSSL-Win64\lib`
- `OPENSSL_LIBRARIES`: Dependency library name. Usually, just `crypto` is OK
- `EXTRA_LINK_ARGS`: This parameter will be appended directly to the linker
- `ENABLE_PKG_CONFIG`: whether to use pkg-config to search openssl, need install pkg-config maybe

```shell
cd goodsync/python  

python -m pip install build

python -m build
```

## Install

```shell
python -m pip install goodsync
```

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

## License

---
Apache-2.0 © [bitjerry](/LICENSE)

*2024/4/1*
*Mr.lin*