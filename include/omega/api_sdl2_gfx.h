/*

SDL2_gfxPrimitives.h: graphics primitives for SDL

Copyright (C) 2012-2014  Andreas Schiffler
Additions for BBC BASIC (C) 2016-2020 Richard Russell
Adopted for Omega (C) 2024 Pixelsuft

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.

Andreas Schiffler -- aschiffler at ferzkopp dot net
Richard Russell -- richard at rtrussell dot co dot uk
Pixelsuft -- pixelsuft at github dot com

*/

#pragma once

#include <math.h>
#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif
#include <omega/ostd.h>
#include <omega/api_sdl2.h>

#define SDL2_GFX_MAJOR 1
#define SDL2_GFX_MINOR 0
#define SDL2_GFX_MICRO 1

#define SDL2_GFX_STD_PREFIX

SDL2_GFX_STD_PREFIX void omg_sdl2_gfx_set_handle(OMG_Sdl2* sdl2, OMG_Std* std);
SDL2_GFX_STD_PREFIX int pixelRGBA(SDL_Renderer* renderer, int16_t x, int16_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
SDL2_GFX_STD_PREFIX int hlineRGBA(SDL_Renderer* renderer, int16_t x1, int16_t x2, int16_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
SDL2_GFX_STD_PREFIX int vlineRGBA(SDL_Renderer* renderer, int16_t x, int16_t y1, int16_t y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
SDL2_GFX_STD_PREFIX int rectangleRGBA(SDL_Renderer* renderer, int16_t x1, int16_t y1,
									  int16_t x2, int16_t y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
SDL2_GFX_STD_PREFIX int roundedRectangleRGBA(SDL_Renderer* renderer, int16_t x1, int16_t y1,
											 int16_t x2, int16_t y2, int16_t rad, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
SDL2_GFX_STD_PREFIX int boxRGBA(SDL_Renderer* renderer, int16_t x1, int16_t y1, int16_t x2,
								int16_t y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
SDL2_GFX_STD_PREFIX int roundedBoxRGBA(SDL_Renderer* renderer, int16_t x1, int16_t y1, int16_t x2,
									   int16_t y2, int16_t rad, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
SDL2_GFX_STD_PREFIX int lineRGBA(SDL_Renderer* renderer, int16_t x1, int16_t y1,
								 int16_t x2, int16_t y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
SDL2_GFX_STD_PREFIX int aalineRGBA(SDL_Renderer* renderer, int16_t x1, int16_t y1,
								   int16_t x2, int16_t y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
SDL2_GFX_STD_PREFIX int thickLineRGBA(SDL_Renderer* renderer, int16_t x1, int16_t y1, int16_t x2, int16_t y2,
									  uint8_t width, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
SDL2_GFX_STD_PREFIX int circleRGBA(SDL_Renderer* renderer, int16_t x, int16_t y, int16_t rad, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
SDL2_GFX_STD_PREFIX int arcRGBA(SDL_Renderer* renderer, int16_t x, int16_t y, int16_t rad, int16_t start, int16_t end,
								uint8_t r, uint8_t g, uint8_t b, uint8_t a);
SDL2_GFX_STD_PREFIX int aacircleRGBA(SDL_Renderer* renderer, int16_t x, int16_t y,
									 int16_t rad, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
SDL2_GFX_STD_PREFIX int filledCircleRGBA(SDL_Renderer* renderer, int16_t x, int16_t y,
										 int16_t rad, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
SDL2_GFX_STD_PREFIX int ellipseRGBA(SDL_Renderer* renderer, int16_t x, int16_t y,
									int16_t rx, int16_t ry, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
SDL2_GFX_STD_PREFIX int aaellipseRGBA(SDL_Renderer* renderer, int16_t x, int16_t y,
									  int16_t rx, int16_t ry, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
SDL2_GFX_STD_PREFIX int filledEllipseRGBA(SDL_Renderer* renderer, int16_t x, int16_t y,
										  int16_t rx, int16_t ry, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
SDL2_GFX_STD_PREFIX int pieRGBA(SDL_Renderer* renderer, int16_t x, int16_t y, int16_t rad,
								int16_t start, int16_t end, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
SDL2_GFX_STD_PREFIX int filledPieRGBA(SDL_Renderer* renderer, int16_t x, int16_t y, int16_t rad,
									  int16_t start, int16_t end, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
SDL2_GFX_STD_PREFIX int trigonRGBA(SDL_Renderer* renderer, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3,
								   uint8_t r, uint8_t g, uint8_t b, uint8_t a);
SDL2_GFX_STD_PREFIX int aatrigonRGBA(SDL_Renderer* renderer, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3,
									 uint8_t r, uint8_t g, uint8_t b, uint8_t a);
SDL2_GFX_STD_PREFIX int filledTrigonRGBA(SDL_Renderer* renderer, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3,
										 uint8_t r, uint8_t g, uint8_t b, uint8_t a);
SDL2_GFX_STD_PREFIX int polygonRGBA(SDL_Renderer* renderer, const int16_t* vx, const int16_t* vy,
									int n, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
SDL2_GFX_STD_PREFIX int aapolygonRGBA(SDL_Renderer* renderer, const int16_t* vx, const int16_t* vy,
									  int n, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
SDL2_GFX_STD_PREFIX int filledPolygonRGBA(SDL_Renderer* renderer, const int16_t* vx,
										  const int16_t* vy, int n, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
SDL2_GFX_STD_PREFIX int texturedPolygon(SDL_Renderer* renderer, const int16_t* vx, const int16_t* vy, int n, SDL_Surface* texture, int texture_dx, int texture_dy);
SDL2_GFX_STD_PREFIX int bezierRGBA(SDL_Renderer* renderer, const int16_t* vx, const int16_t* vy, int n, int s, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
SDL2_GFX_STD_PREFIX double _evaluateBezier(double* data, int ndata, double t);
SDL2_GFX_STD_PREFIX int thickEllipseRGBA(SDL_Renderer* renderer, int16_t xc, int16_t yc, int16_t xr, int16_t yr, uint8_t r, uint8_t g, uint8_t b, uint8_t a, uint8_t thick);
SDL2_GFX_STD_PREFIX int thickArcRGBA(SDL_Renderer* renderer, int16_t xc, int16_t yc, int16_t rad, int16_t start, int16_t end, uint8_t r, uint8_t g, uint8_t b, uint8_t a, uint8_t thick);
SDL2_GFX_STD_PREFIX int thickCircleRGBA(SDL_Renderer* renderer, int16_t x, int16_t y, int16_t rad, uint8_t r, uint8_t g, uint8_t b, uint8_t a, uint8_t thick);
SDL2_GFX_STD_PREFIX int filledPolyBezierRGBA(SDL_Renderer* renderer, const int16_t* x, const int16_t* y, int n, int s, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
SDL2_GFX_STD_PREFIX int aaFilledEllipseRGBA(SDL_Renderer* renderer, float cx, float cy, float rx, float ry, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
SDL2_GFX_STD_PREFIX int aaFilledPolygonRGBA(SDL_Renderer* renderer, const double* vx, const double* vy, int n, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
SDL2_GFX_STD_PREFIX int aaFilledPieRGBA(SDL_Renderer* renderer, float cx, float cy, float rx, float ry, float start, float end, uint32_t chord, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
SDL2_GFX_STD_PREFIX int aaArcRGBA(SDL_Renderer* renderer, float cx, float cy, float rx, float ry, float start, float end, float thick, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
SDL2_GFX_STD_PREFIX int aaBezierRGBA(SDL_Renderer* renderer, double* x, double* y, int n, int s, float thick, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
SDL2_GFX_STD_PREFIX int aaFilledPolyBezierRGBA(SDL_Renderer* renderer, double* x, double* y, int n, int s, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
