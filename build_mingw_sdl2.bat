@echo off
set LIBS=-lkernel32 -lgcc -lntdll -LSDL2/lib -lSDL2
gcc src/*.c -Iinclude -DNOSTDLIB -nostdlib -municode -DOMG_SDL2_DYNAMIC=0 -DOMG_DLL_BUILD -Wpedantic -Wall -Wextra %LIBS% -shared -fPIC -o omega.dll -mwindows && gcc main.c -Wall -Wextra -Wpedantic %LIBS% -Iinclude -DOMG_SDL2_DYNAMIC=0 -DNOSTDLIB -nostdlib -lSDL2main -lomega -mwindows -municode -L. -o omgapp.exe
