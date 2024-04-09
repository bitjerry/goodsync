# GoodSync

Reverse engineering of GoodSync file name encryption and decryption algorithms  
Help GoodSync users switch to other synchronization tools more freely without being bound by GoodSync

The project only implements encryption and decryption functions, and does not include directory or file processing,
synchronization and other functions. If you have good ideas, welcome to PR and let us improve the code together
The project is implemented using c and openssl. It also provides python extensions for easy embedding into python
projects. If you have implementations in other languages, you are welcome to PR

Read in other languages: English | [简体中文](README.ZH-CN.MD)

## Build

### Prepare

> You can find compiled python wheels in [releases](https://github.com/bitjerry/goodsync/releases).
> If they don't run on your system, try build them

### c

Configure the environment variable `OPENSSL_ROOT_DIR` as the location of the openssl installation package

```shell
cmake -B ./cmake_build
cmake --build ./cmake_build --config Release
```

### python

Configure environment variables:

- OPENSSL_INCLUDE_DIR: openssl include folder path
- OPENSSL_LIBRARY_DIR: openssl library folder path
- OPENSSL_LIBRARIES: Crypto library, eg: libcrypto
- _CROSS_COMPILE_FILE: Cross-compile the project. Note that usually this is not available. Cross-compiling python
  extensions is very difficult. You need to specify the openssl dependency path for the corresponding architecture.
- ENABLE_PKG_CONFIG: Boolean value, whether to use pkg-conifg to search openssl, need install pkg-config maybe

```shell
cd goodsync/python  
python -m build
```

## Run

### python

```shell

```

## License

---
Apache-2.0 © [bitjerry](/LICENSE)

*2024/4/1*
*Mr.lin*