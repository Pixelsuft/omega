@echo off
set LIBS=-lkernel32 -lgcc -lntdll
rem gcc main.c src/*.c -municode -DOMG_STATIC -DNOSTDLIB %LIBS% -Wall -Wextra -Wpedantic -Iinclude -nostdlib -mwindows %* -o omgapp.exe
gcc src/*.c -Iinclude -nostdlib -municode -DOMG_DLL_BUILD -DOMG_SUPPORT_SDL2=0 -DNOSTDLIB -Wpedantic -Wall -Wextra %LIBS% -shared -fPIC -o omega.dll -mwindows && gcc main.c -Wall -Wextra -Wpedantic -DNOSTDLIB -DOMG_SUPPORT_SDL2=0 %LIBS% -Iinclude -nostdlib -lomega -mwindows -municode -L. -o omgapp.exe
