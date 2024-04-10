# GoodSync

对 GoodSync 文件名加解密算法的逆向工程    
帮助 GoodSync 用户更自由地切换到其它同步工具, 不受 GoodSync 约束

项目仅实现加解密功能, 不包括目录文件处理, 同步等功能, 如果你有好的想法, 欢迎 pr 我们一起完善代码  
项目使用 c 和 openssl 实现, 另提供 python 扩展便于嵌入到 python 项目, 如果你有其它语言的实现, 欢迎 pr

使用其他语言阅读：[English](/README.md) | 简体中文

## 编译

### 前置工作

安装 openssl 开发包

### c

配置环境变量 `OPENSSL_ROOT_DIR` 为 openssl 安装位置

```shell
cmake -B ./cmake_build
cmake --build ./cmake_build --config Release
```

### python

#### 注意

如果你不熟悉 c 和 python, 建议使用发行的轮子 [released](https://github.com/bitjerry/goodsync/releases)

通常不需要配置环境变量，构建过程会自动寻找 openssl 的位置。
默认情况下，是动态链接，你需要使用 `auditwheel` 或类似的工具来修复它，这是关于 python 打包的常识，如果你打算构建它，你应该了解这些知识。

#### 环境变量

- `OPENSSL_INCLUDE_DIR`: openssl include 文件夹
- `OPENSSL_LIBRARY_DIR`: openssl lib 文件夹
- `OPENSSL_LIBRARIES`: 依赖库名. 通常, 在 Windows 上为 `libcrypto`, 在 Linux 或 macos 上为 `crypto`
- `EXTRA_LINK_ARGS`: 在链接过程中, 被直接追加到链接器后
- `ENABLE_PKG_CONFIG`: 是否启用 `pkg-config` 寻找 openssl 依赖包. 一般情况下用不到, 有时你需要安装 `pkg-config`

```shell
cd goodsync/python  

python -m pip install build

python -m build
```

## 安装

```shell
python -m pip install *.whl 
```

## 用法

```python
from goodsync import *

gs = GoodSync("0123456789")
print(gs.key)

encrypted_filename = gs.encrypt_name("test")
print(encrypted_filename)

filename = gs.decrypt_name(encrypted_filename)
print(filename)
```

## 后记

该项目的诞生纯粹源于 goodsync 不愿意向我透露他们的文件名加密算法, 以及他们傲慢的客服

理论上使用该算法加密的字符串总能被 goodsync 识别  
但随着 goodsync 版本更新, 该算法不总能解密 goodsync 加密字符串  
如果我有时间, 我会持续跟进 goodsync 版本更新, 截至 goodsync 版本 21.5.5.4

代码完全开源, 即使 goodsync 窃密我们也不会窃取你的密码和数据

## 许可证

---
Apache-2.0 © [bitjerry](/LICENSE)

*2024/4/1*
*Mr.lin*
