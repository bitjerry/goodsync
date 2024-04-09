# GoodSync

对 GoodSync 文件名加解密算法的逆向工程    
帮助 GoodSync 用户更自由地切换到其它同步工具, 不受 GoodSync 约束

项目仅实现加解密功能, 不包括目录文件处理, 同步等功能, 如果你有好的想法, 欢迎 pr 我们一起完善代码  
项目使用 c 和 openssl 实现, 另提供 python 扩展便于嵌入到 python 项目, 如果你有其它语言的实现, 欢迎 pr

使用其他语言阅读：[English](/README.md) | 简体中文

## 编译

### 前置工作

配置好`OPENSSL_ROOT_DIR`环境变量

### c

`src` 文件下包含一个使用示例 `example.c`  
使用 cmake 编译或任何其它工具

### python

```shell
cd goodsync/python  
python -m build
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
