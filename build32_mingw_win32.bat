@echo off
set EXTRA=-lkernel32 -lgcc -lntdll -lgdi32 -luser32 -lm -lmingw32 -DOMG_SUPPORT_RAYLIB=1 -nostdlib -municode -DNOSTDLIB -nostdlib
set EXTRA=%EXTRA% -DOMG_WINAPI_DYNAMIC=0 -DOMG_WINAPI_DYNAMIC_UGLY=1 -DOMG_WINAPI_STATIC_COMPAT=1 -DOMG_SDL2_DYNAMIC=1
gcc -m32 src/*.c -Iinclude -DOMG_DLL_BUILD -Wpedantic -Wall -Wextra %EXTRA% -shared -fPIC -o omega.dll && gcc -m32 main.c -Wall -Wextra -Wpedantic -Iinclude -lomega -L. -o omgapp.exe %EXTRA%
