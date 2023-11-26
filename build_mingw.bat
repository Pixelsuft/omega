@echo off
gcc src/*.c -Iinclude -DOMG_DLL_BUILD -Wall -Wpedantic -Wextra -shared -fPIC -o omega.dll && gcc main.c -Wpedantic -Wall -Wextra -Iinclude -lomega %* -L. -o omgapp.exe
