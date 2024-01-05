@echo off
set EXTRA=-lkernel32 -lgcc -lntdll -lm -lmingw32 -Wl,-V -DOMG_SUPPORT_RAYLIB=0
rem set EXTRA=%EXTRA% -luser32 -DOMG_SUPPORT_SDL2=0 -DOMG_SUPPORT_RAYLIB=0 -DOMG_WINAPI_DYNAMIC=0
gcc -m32 src/*.c -Iinclude -nostdlib -municode -DOMG_DLL_BUILD -DNOSTDLIB -Wpedantic -Wall -Wextra %EXTRA% -shared -fPIC -o omega.dll -mwindows && gcc -m32 main.c -Wall -Wextra -Wpedantic -DNOSTDLIB -Iinclude -nostdlib -lomega -mwindows -municode -L. -o omgapp.exe %EXTRA%
