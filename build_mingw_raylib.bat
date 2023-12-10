@echo off
set EXTRA=-lmsvcrt -Lraylib/lib -lraylib -Iraylib/include -DOMG_RAYLIB_DYNAMIC=0 -DOMG_SUPPORT_SDL2=0
gcc src/*.c -Iinclude -municode -DOMG_DLL_BUILD -Wpedantic -Wall -Wextra %EXTRA% -shared -fPIC -o omega.dll -mwindows && gcc main.c -Wall -Wextra -Wpedantic %EXTRA% -Iinclude -lomega -mwindows -municode -L. -o omgapp.exe
