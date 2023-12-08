@echo off
set LIBS=-lkernel32 -lgcc -lntdll
gcc -m32 src/*.c -Iinclude -nostdlib -municode -DOMG_SUPPORT_SDL2=0 -DOMG_DLL_BUILD -DNOSTDLIB -Wpedantic -Wall -Wextra %LIBS% -shared -fPIC -o omega.dll -mwindows && gcc main.c -Wall -Wextra -Wpedantic -DNOSTDLIB %LIBS% -Iinclude -nostdlib -DOMG_SUPPORT_SDL2=0 -lomega -mwindows -m32 -municode -L. -o omgapp.exe
