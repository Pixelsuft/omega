mkdir -p embuild
emcc main.c src/*.c -Iinclude -DOMG_DEBUG=0 -DOMG_SUPPORT_RAYLIB=0 -DOMG_SDL2_DYNAMIC=0 -DOMG_SDL2_IMAGE_DYNAMIC=0 -DOMG_SDL2_MIXER_DYNAMIC=0 -Wall -Wextra -s ALLOW_MEMORY_GROWTH=1 --preload-file assets -o embuild/index.html -s WASM=1 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2 -s SDL2_IMAGE_FORMATS='["png"]' -s SDL2_MIXER_FORMATS='["mp3", "ogg"]'
