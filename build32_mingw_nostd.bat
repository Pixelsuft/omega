@echo off
set LIBS=-lkernel32 -lgcc -lntdll
rem gcc main.c src/*.c -municode -DOMG_STATIC -DNOSTDLIB %LIBS% -Wall -Wextra -Wpedantic -Iinclude -nostdlib -mwindows %* -o omgapp.exe
gcc -m32 src/*.c -Iinclude -nostdlib -municode -DOMG_DLL_BUILD -DNOSTDLIB -Wpedantic -Wall -Wextra %LIBS% -shared -fPIC -o omega.dll -mwindows && gcc main.c -Wall -Wextra -Wpedantic -DNOSTDLIB %LIBS% -Iinclude -nostdlib -lomega -mwindows -m32 -municode -L. -o omgapp.exe
