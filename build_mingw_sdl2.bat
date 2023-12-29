@echo off
set LIBS=-lkernel32 -lgcc -lntdll -LSDL2/lib -lSDL2 -DOMG_SUPPORT_RAYLIB=0 -nostdlib
gcc src/*.c -ISDL2/include -Iinclude -DNOSTDLIB -nostdlib -municode -DOMG_SDL2_DYNAMIC=0 -DOMG_DLL_BUILD -Wpedantic -Wall -Wextra %LIBS% -shared -fPIC -o omega.dll -mwindows && gcc main.c -Wall -Wextra -Wpedantic %LIBS% -Iinclude -DOMG_SDL2_DYNAMIC=0 -DNOSTDLIB -nostdlib -lSDL2main -lomega -ISDL2/include -mwindows -municode -L. -o omgapp.exe
