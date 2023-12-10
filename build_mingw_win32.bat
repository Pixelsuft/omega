@echo off
set EXTRA=-lkernel32 -lgcc -lntdll -DOMG_SUPPORT_SDL2=0 -DOMG_SUPPORT_RAYLIB=0
rem gcc main.c src/*.c -municode -DOMG_STATIC -DNOSTDLIB %LIBS% -Wall -Wextra -Wpedantic -Iinclude -nostdlib -mwindows %* -o omgapp.exe
gcc src/*.c -Iinclude -nostdlib -municode -DOMG_DLL_BUILD -DNOSTDLIB -Wpedantic -Wall -Wextra %EXTRA% -shared -fPIC -o omega.dll -mwindows && gcc main.c -Wall -Wextra -Wpedantic -DNOSTDLIB %EXTRA% -Iinclude -nostdlib -lomega -mwindows -municode -L. -o omgapp.exe
