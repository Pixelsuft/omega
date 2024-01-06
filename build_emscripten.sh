emcc main.c src/*.c -Iinclude -DOMG_SUPPORT_RAYLIB=0 -DOMG_SDL2_DYNAMIC=0 -Wall -Wextra -s ALLOW_MEMORY_GROWTH=1 --preload-file assets -o embuild/index.html -s WASM=1 -s USE_SDL=2
