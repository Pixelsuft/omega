# OMEGA
Simple framework for creating 2D games in C
## Functionality
 - Multiple backends (Dynamic/static backends loading at real time) (SDL2, Raylib, Win32) <br />
 - Libraries config using [`config.h` or preprocessor macros](include/omega/config.h) <br />
 - Window management (SDL2, Raylib) <br />
 - Renderer (Direct3D, OpenGL, OpenGLES, WebGL, Software) (using SDL2, Raylib) <br />
 - Image loading (SDL2_image, Raylib, libspng) <br />
 - Audio (Music/Sounds) playback (SDL2_mixer, Raylib, FMOD, Web) <br />
 - TTF (TrueType Font) Rendering (SDL2_ttf, Raylib) <br />
 - Bitmap fonts <br />
 - LDtk tilemaps import
## Is it written in C?
Yes, it it's written in pure C. <br />
It also does some tricks with C to extend its power. For example:
 - OOP using structs, function pointers, etc <br />
 - Magic macros like `OMG_INFO` which accepts different number of arguments of different types <br />
 - Things like tracing memory allocation by code file, function, line <br />
 - Simple error handling using `bool` return type <br />
 - Using native win32 api instead of C stdlib on Windows <br />
 - Many other tricks
## Portability
It's only limited to C language and backend libraries. I was able to run examples on:
 - from Windows XP (including ReactOS) to Windows 11 (Windows 2000 is only supported with extended kernel) <br />
 - UWP (Windows 11) <br />
 - Linux <br />
 - UNIX (FeeBSD, NetBSD, OpenBSD) <br />
 - Web (SDL2 + Emscripten) <br />
 - Android (thanks to SDL2)
## Examples
There are some examples. <br />
### [Simple Example](examples/example1.c)
This one shows main functionality of the framework
### [Advanced Example](main.c)
This one shows working with scenes, animations, tilemap rendering
### [Simple Game](game/aamain.c)
This is a mini-platformer (written in ~3 days) which shows what you can do with my framework
## License
LGPL-3.0 License. See [LICENSE.md](LICENSE.md). <br />
In short, feel free to use it in your projects, but you may not close source code of this framework, if you modify it.
