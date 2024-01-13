import os
import sys
import requests


def download(url: str, out_path: str) -> None:
    print('Downloading', url)
    resp = requests.get(url)
    data = resp.content
    open(out_path, 'wb').write(data)


def download_bytes(url: str) -> bytes:
    print('Downloading', url)
    resp = requests.get(url)
    return resp.content


def write_bytes(bt: bytes, out_path: str) -> None:
    open(out_path, 'wb').write(bt)


cwd = os.path.dirname(__file__)
if cwd:
    cwd = os.path.join(cwd, '..', '..')
else:
    cwd = os.getcwd()
src = os.path.join(cwd, 'src')
include = os.path.join(cwd, 'include', 'libs')
print('Pixelsuft Load Deps System :D')

if not os.path.isdir(src) or not os.path.isdir(include):
    print('Error: not main dir')
    sys.exit(1)

download('https://github.com/lvandeve/lodepng/raw/master/lodepng.h', os.path.join(include, 'lodepng.h'))
lodepng = download_bytes('https://github.com/lvandeve/lodepng/raw/master/lodepng.cpp').replace(
    b'#include "lodepng.h"', b'#include <libs/lodepng.h>'
).replace(b'return malloc(size);', b'return OMG_MALLOC(omg_get_default_omega()->mem, size);').replace(
    b'return realloc(ptr, new_size);', b'return OMG_REALLOC(omg_get_default_omega()->mem, ptr, new_size);'
).replace(b'free(ptr);', b'OMG_FREE(omg_get_default_omega()->mem, ptr);')
lodepng = b'''#include <omega/config.h>

#if OMG_SUPPORT_LODEPNG
#include <omega/omega.h>
#include <omega/ostd.h>

''' + lodepng + b'''
#endif
'''
write_bytes(lodepng, os.path.join(src, 'lodepng.c'))
