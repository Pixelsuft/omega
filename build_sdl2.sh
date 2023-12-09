if [ -z $CC ]; then
    CC=gcc
fi
SDL2_FLAGS=$(sdl2-config --cflags --libs)
LIBS="-lSDL2main -fno-stack-protector -Wno-gnu-zero-variadic-macro-arguments -Wno-empty-translation-unit"
$CC src/*.c $SDL2_FLAGS -Iinclude -DOMG_SDL2_DYNAMIC=0 -DOMG_DLL_BUILD -Wpedantic -Wall -Wextra $LIBS -shared -fPIC -o libomega.so && $CC main.c -Wall -Wextra -Wpedantic $LIBS -Iinclude -DOMG_SDL2_DYNAMIC=0 $SDL2_FLAGS -lomega -L. -o omgapp
