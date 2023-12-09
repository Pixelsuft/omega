if [ -z $CC ]; then
    CC=gcc
fi
CC=gcc
SDL2_FLAGS=$(sdl2-config --cflags --libs)
$CC src/*.c $SDL2_FLAGS $LIBS -Iinclude -DNOSTDLIB -DOMG_SDL2_DYNAMIC=0 -DOMG_DLL_BUILD -Wpedantic -Wall -Wextra -shared -fPIC -o libomega.so && $CC main.c -Wall -Wextra -Wpedantic -Iinclude -DOMG_SDL2_DYNAMIC=0 -DNOSTDLIB $SDL2_FLAGS $LIBS -lomega -L. -o omgapp
