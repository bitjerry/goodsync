# !/usr/bin/python3
# -*-coding: UTF-8-*-
"""
@Time: 2024-04-01 22:17
@Author: Mr.lin
@Version: v1
@File: setup
"""
import os
import platform
import subprocess
import sys
from pathlib import Path

from setuptools import setup, Extension

if sys.version_info < (3, 8):
    raise RuntimeError("GoodSync requires Python 3.8+")

print("===========================================")
print("===========GoodSync Tools Builder==========")
print("===============Author: Mr.lin===============")
print("===========================================")
print("Platform: ", platform.platform())
print("System: ", platform.system())
print("Machine: ", platform.machine())
print("Architecture: ", platform.architecture())
print("Python: ", sys.version)
print("Python Path: ", sys.path)
print("===========================================")
print("===========================================")

OPENSSL_INCLUDE_DIR_ENV: str = os.getenv("OPENSSL_INCLUDE_DIR")
OPENSSL_LIBRARY_DIR_ENV: str = os.getenv("OPENSSL_LIBRARY_DIR")
OPENSSL_LIBRARIES_ENV: str = os.getenv("OPENSSL_LIBRARIES")
EXTRA_LINK_ARGS_ENV: str = os.getenv("EXTRA_LINK_ARGS")
ENABLE_PKG_CONFIG_ENV: bool = os.getenv("ENABLE_PKG_CONFIG", False)

# debug only
# print("========All Environment========")
# for _k, _v in os.environ.items():
#     print(f" ==> {_k}: {_v}")
# print("===============================")

BUILD_DIR = Path(__file__).parent.absolute()

PACKAGE = "goodsync"


def pkgconfig(package, kw):
    flag_map = {'-I': 'include_dirs', '-L': 'library_dirs', '-l': 'libraries'}
    cmd = f'pkg-config --cflags --libs {package}'
    print("The pkg-config command is: ", cmd)
    output = subprocess.getoutput(cmd)
    print("The pkg-config command execution result is: ", output)
    for token in output.strip().split():
        if token[0] == '-':
            kw.setdefault(flag_map.get(token[:2]), []).append(token[2:])
        else:
            kw.setdefault("libraries", []).append(token)
    return kw


def gen_extension_kw(
        kw: dict, openssl_include_dir: list, openssl_library_dir: list, openssl_libraries: list, **kwargs):
    """
     Use pkg-config to supplement parameters not found

     NOTE: Not available on all platforms

     If you use pkg-config to find dependencies, the specified dependencies will not take effect.

    :param kw:
    :param openssl_include_dir:
    :param openssl_library_dir:
    :param openssl_libraries:
    :return:
    """
    if ENABLE_PKG_CONFIG_ENV:
        try:
            print(f"Try to use pkg-config to find OpenSSL. Platform: {platform.system()}")
            _kw = {}
            pkgconfig("openssl", _kw)
            print("Find openssl configuration is: ", _kw)
            kw['include_dirs'] = _kw.get('include_dirs')
            kw['library_dirs'] = _kw.get('library_dirs')
            kw['libraries'] = _kw.get('libraries')
        except Exception as e:
            print(e)
    else:
        if openssl_include_dir:
            for _dir in openssl_include_dir:
                _dir_p = Path(_dir)
                print("The openssl include dir absolute path: ", _dir_p.absolute())
                for file in _dir_p.iterdir():
                    print(file)
            kw['include_dirs'] = openssl_include_dir
            print("The specified include path is available ==> ", openssl_include_dir)
        else:
            print("Cannot find OpenSSL include directory")

        if openssl_library_dir:
            for _dir in openssl_library_dir:
                _dir_p = Path(_dir)
                print("The openssl library dir absolute path: ", _dir_p.absolute())
                for file in Path(_dir_p).iterdir():
                    print(file)
            kw['library_dirs'] = openssl_library_dir
            print("The specified lib path is available ==> ", openssl_library_dir)
        else:
            print("Cannot find OpenSSL library directory")

        if openssl_libraries:
            print("The openssl libraries: ")
            for file in openssl_libraries:
                print(file)
            kw['libraries'] = openssl_libraries
            print("The specified libraries is available ==> ", openssl_libraries)
        else:
            print("Cannot find OpenSSL libraries")
    kw.update(kwargs)


def find_openssl_dev(kw):
    openssl_library_dir: list = []
    openssl_include_dir: list = []
    openssl_libraries: list = []
    extra_link_args: list = EXTRA_LINK_ARGS_ENV.split() if EXTRA_LINK_ARGS_ENV else []

    if OPENSSL_LIBRARY_DIR_ENV is not None:
        _openssl_library_dir = Path(OPENSSL_LIBRARY_DIR_ENV)
        if _openssl_library_dir.exists():
            for lib in _openssl_library_dir.iterdir():
                if lib.is_file() and "crypto" in lib.name:
                    openssl_library_dir = [str(_openssl_library_dir)]

    if OPENSSL_INCLUDE_DIR_ENV is not None:
        _openssl_include_dir = Path(OPENSSL_INCLUDE_DIR_ENV)
        if _openssl_include_dir.exists():
            if (_openssl_include_dir / "openssl").exists():
                openssl_include_dir = [str(_openssl_include_dir)]

    # Auto find openssl path if env are not specified
    if platform.system() == "Windows":
        openssl_libraries = OPENSSL_LIBRARIES_ENV.split() if OPENSSL_LIBRARIES_ENV else ["libcrypto"]
        openssl_root_dirs = None
        if not openssl_library_dir or not openssl_include_dir:
            for f in Path(r"C:\Program Files").glob("*openssl*"):
                if f.is_dir():
                    openssl_root_dirs = f.absolute()
            if openssl_root_dirs is None:
                for f in Path(r"C:\Program Files (x86)").glob("*openssl*"):
                    if f.is_dir():
                        openssl_root_dirs = f.absolute()
            if openssl_root_dirs is None:
                raise Exception("Cannot find OpenSSL installation on Windows")
            if not openssl_library_dir:
                openssl_library_dir = [str(openssl_root_dirs / "lib")]
            if not openssl_include_dir:
                openssl_include_dir = [str(openssl_root_dirs / "include")]
    elif platform.system() == "Darwin":
        openssl_libraries = OPENSSL_LIBRARIES_ENV.split() if OPENSSL_LIBRARIES_ENV else ["crypto"]
        openssl_root_dirs = None
        if not openssl_library_dir or not openssl_include_dir:
            try:
                cmd = "brew --prefix openssl@3"
                print("Search mac openssl root: ", cmd)
                output = subprocess.getoutput(cmd)
                print("The brew command execution result is: ", output)
                openssl_root_dirs = Path(output.strip())
            except Exception as e:
                print(e)
            if openssl_root_dirs is None:
                raise Exception("Cannot find OpenSSL installation on MacOS")
            if not openssl_library_dir:
                openssl_library_dir = [str(openssl_root_dirs / "lib")]
            if not openssl_include_dir:
                openssl_include_dir = [str(openssl_root_dirs / "include")]
    else:
        # Dynamic link libraries always exist, but static link libraries may not exist
        openssl_include_dir = ["/usr/include"] if OPENSSL_INCLUDE_DIR_ENV is None else OPENSSL_INCLUDE_DIR_ENV.split()
        openssl_libraries = ["crypto"] if OPENSSL_LIBRARIES_ENV is None else OPENSSL_LIBRARIES_ENV.split()

    gen_extension_kw(kw, openssl_include_dir, openssl_library_dir, openssl_libraries, extra_link_args=extra_link_args)


extension_kwargs = {
    'sources': [
        f"{PACKAGE}/goodsync_warp.c",
        f"{PACKAGE}/goodsync.c",
        f"{PACKAGE}/crc.c",
        f"{PACKAGE}/base.c"
    ]
}

find_openssl_dev(extension_kwargs)

setup(
    ext_modules=[
        Extension(
            f'{PACKAGE}._GoodSync',
            **extension_kwargs
        )
    ]
)
