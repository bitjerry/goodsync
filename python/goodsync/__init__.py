# !/usr/bin/python3
# -*-coding: UTF-8-*-
"""
@Time: 2024-04-01 22:27
@Author: Mr.lin
@Version: v1
@File: __init__
"""

__version__ = "0.0.3"

if __package__ or "." in __name__:
    from goodsync import _GoodSync
else:
    import _GoodSync


class GSNativeError(Exception):
    """
    Throw exception from python c module

    Any encryption or decryption exception
    """
    ...


def encrypt_master_key(master_key: str):
    """
    GoodSync encrypts and stores the user's password for each task

    :param master_key: The key in the GoodSync task
    :return:
    """
    try:
        return _GoodSync.encrypt_master_key(master_key)
    except _GoodSync.error as e:
        raise GSNativeError(e)


def encrypt_name(key: str, name: str) -> str:
    """
    According to GoodSync encryption algorithm

    Use key to encrypt file or directory name

    :param key: From `encrypt_master_key` function return
    :param name: Original file or directory name
    :return:
    """
    try:
        return _GoodSync.encrypt_name(key, name)
    except _GoodSync.error as e:
        raise GSNativeError(e)


def decrypt_name(key: str, name: str) -> str:
    """
    According to GoodSync decryption algorithm

    Use key to decrypt file or directory name encrypted by GoodSync

    :param key: From `encrypt_master_key` function return
    :param name: Encrypted file or directory name
    :return:
    """
    try:
        return _GoodSync.decrypt_name(key, name)
    except _GoodSync.error as e:
        raise GSNativeError(e)


class GoodSync:
    """
    Corresponds to a task of GoodSync
    """

    def __init__(self, master_key: str, suffix: str = ".gszip"):
        """
        Initialize a GoodSync task using key

        :param master_key: The key in the GoodSync task
        :param suffix: GoodSync file suffix
        """
        self.key: str = encrypt_master_key(master_key)
        self.suffix: str = suffix
        self.gs_zipfs_state: str = self.encrypt_name("gs_zipfs_state")

    def encrypt_name(self, name: str) -> str:
        """
        See function `encrypt_name`

        :param name:
        :return:
        """
        return encrypt_name(self.key, name)

    def decrypt_name(self, name: str) -> str:
        """
        See function `decrypt_name`

        :param name:
        :return:
        """
        return decrypt_name(self.key, name)

    def encrypt_name_with_suffix(self, name: str) -> str:
        return self.encrypt_name(name) + self.suffix

    def decrypt_name_with_suffix(self, name: str) -> str:
        return self.decrypt_name(name) + self.suffix
