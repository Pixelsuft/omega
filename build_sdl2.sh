if [ -z $CC ]; then
    CC=gcc
fi
SDL2_FLAGS=$(sdl2-config --cflags --libs)
EXTRA="-lSDL2main -DOMG_SDL2_DYNAMIC=0 -DOMG_SUPPORT_RAYLIB=0 -fno-stack-protector -Wno-gnu-zero-variadic-macro-arguments -Wno-empty-translation-unit"
$CC src/*.c $SDL2_FLAGS -Iinclude -DOMG_DLL_BUILD -Wpedantic -Wall -Wextra $EXTRA -shared -fPIC -o libomega.so && $CC main.c -Wall -Wextra -Wpedantic $EXTRA -Iinclude $SDL2_FLAGS -lomega -L. -o omgapp
