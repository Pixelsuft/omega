if [ -z $CC ]; then
    CC=gcc
fi
CC=gcc
SDL2_FLAGS=$(sdl2-config --cflags --libs)
LIBS="-lSDL2main -fno-stack-protector -fno-builtin -Wno-gnu-zero-variadic-macro-arguments -Wno-empty-translation-unit"
# -Wno-language-extension-token -Wno-gnu-zero-variadic-macro-arguments -Wno-empty-translation-unit
$CC $LIBS src/*.c $SDL2_FLAGS -Iinclude -nostdlib -DNOSTDLIB -DOMG_SDL2_DYNAMIC=0 -DOMG_DLL_BUILD -Wpedantic -Wall -Wextra -shared -fPIC -o libomega.so && $CC $LIBS main.c -Wall -Wextra -Wpedantic -Iinclude -nostdlib -DOMG_SDL2_DYNAMIC=0 -DNOSTDLIB $SDL2_FLAGS -lomega -L. -o omgapp
