@echo off
set LIBS=-lkernel32 -lgcc -lntdll -Lraylib/lib -lraylib
gcc src/*.c -Iraylib/include -Iinclude -DNOSTDLIB -nostdlib -municode -DOMG_RAYLIB_DYNAMIC=0 -DOMG_DLL_BUILD -Wpedantic -Wall -Wextra %LIBS% -shared -fPIC -o omega.dll -mwindows && gcc main.c -Wall -Wextra -Wpedantic %LIBS% -Iinclude -DOMG_RAYLIB_DYNAMIC=0 -DNOSTDLIB -nostdlib -lSDL2main -lomega -Iraylib/include -mwindows -municode -L. -o omgapp.exe
