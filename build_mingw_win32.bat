@echo off
set EXTRA=-lkernel32 -lgcc -lntdll -lm -DOMG_SUPPORT_RAYLIB=1 -lmingw32 -nostdlib -DNOSTDLIB -mwindows -municode
rem set EXTRA=%EXTRA% -DOMG_WINAPI_DYNAMIC=0 -DOMG_WINAPI_DYNAMIC_UGLY=0 -DOMG_WINAPI_STATIC_COMPAT=1 -DOMG_SDL2_DYNAMIC=0
gcc src/*.c -Iinclude -DOMG_DLL_BUILD -Wpedantic -Wall -Wextra %EXTRA% -shared -fPIC -o omega.dll && gcc main.c -Wall -Wextra -Wpedantic -Iinclude -lomega -L. -o omgapp.exe %EXTRA%
