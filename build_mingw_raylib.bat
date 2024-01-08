@echo off
set EXTRA=-Lraylib/lib -lraylibdll -Iraylib/include -DOMG_RAYLIB_DYNAMIC=1 -DOMG_SUPPORT_SDL2=0
gcc src/*.c -Iinclude -municode -DOMG_DLL_BUILD -Wpedantic -Wall -Wextra %EXTRA% -shared -fPIC -o omega.dll -mwindows && gcc main.c -Wall -Wextra -Wpedantic %EXTRA% -Iinclude -lomega -municode -L. -o omgapp.exe
