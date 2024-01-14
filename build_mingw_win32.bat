@echo off
set EXTRA=-lkernel32 -lgcc -lntdll -lm -DOMG_SUPPORT_RAYLIB=0 -lmingw32
rem gcc main.c src/*.c -municode -DOMG_STATIC -DNOSTDLIB %LIBS% -Wall -Wextra -Wpedantic -Iinclude -nostdlib -mwindows %* -o omgapp.exe
gcc src/*.c -Iinclude -municode -DOMG_DLL_BUILD -Wpedantic -Wall -Wextra %EXTRA% -shared -fPIC -o omega.dll -mwindows && gcc main.c -Wall -Wextra -Wpedantic -Iinclude -lomega -mwindows -municode -L. -o omgapp.exe %EXTRA%
