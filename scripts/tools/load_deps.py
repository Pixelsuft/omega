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
include = os.path.join(cwd, 'include', 'omega_libs')
print('Pixelsuft Load Deps System :D')

if not os.path.isdir(src):
    print('Error: not main dir')
    sys.exit(1)
if not os.path.isdir(include):
    os.mkdir(include)
