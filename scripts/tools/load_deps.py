import os
import sys
import requests


def download(url: str, out_path: str) -> None:
    print('Downloading', url)
    resp = requests.get(url)
    data = resp.content
    open(out_path, 'wb').write(data)


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

download('https://github.com/lvandeve/lodepng/raw/master/lodepng.cpp', os.path.join(src, 'lodepng.c'))
download('https://github.com/lvandeve/lodepng/raw/master/lodepng.h', os.path.join(include, 'lodepng.h'))
