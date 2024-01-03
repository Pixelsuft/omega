/*

SDL2_gfxPrimitives.c: graphics primitives for SDL2 renderers

Copyright (C) 2012-2014  Andreas Schiffler
Modifications and additions for BBC BASIC (C) 2016-2020 Richard Russell
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

// TODO: Avoid This shit
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <omega/api_sdl2_gfx.h>

typedef struct {
	int16_t x, y;
	int dx, dy, s1, s2, swapdir, error;
	uint32_t count;
} SDL2_gfxBresenhamIterator;

static OMG_Sdl2* sdl2_handle = NULL;

void omg_sdl2_gfx_set_handle(OMG_Sdl2* sdl2) {
	sdl2_handle = sdl2;
}

int pixel(SDL_Renderer *renderer, int16_t x, int16_t y) {
	return SDL_RenderDrawPoint(renderer, x, y);
}

int pixelColor(SDL_Renderer *renderer, int16_t x, int16_t y, uint32_t color)
{
	uint8_t *c = (uint8_t *)&color;
	return pixelRGBA(renderer, x, y, c[0], c[1], c[2], c[3]);
}

int pixelRGBA(SDL_Renderer *renderer, int16_t x, int16_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	int result = 0;
	if (a != 255)
		result |= SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);
	result |= SDL_RenderDrawPoint(renderer, x, y);
	return result;
}

int pixelRGBAWeight(SDL_Renderer *renderer, int16_t x, int16_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a, uint32_t weight)
{

	uint32_t ax = a;
	ax = ((ax * weight) >> 8);
	if (ax > 255)
	{
		a = 255;
	}
	else
	{
		a = (uint8_t)(ax & 0x000000ff);
	}

	return pixelRGBA(renderer, x, y, r, g, b, a);
}

int hline(SDL_Renderer *renderer, int16_t x1, int16_t x2, int16_t y)
{
	return SDL_RenderDrawLine(renderer, x1, y, x2, y);
	;
}

int hlineColor(SDL_Renderer *renderer, int16_t x1, int16_t x2, int16_t y, uint32_t color)
{
	uint8_t *c = (uint8_t *)&color;
	return hlineRGBA(renderer, x1, x2, y, c[0], c[1], c[2], c[3]);
}

int hlineRGBA(SDL_Renderer *renderer, int16_t x1, int16_t x2, int16_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	int result = 0;
	if (a != 255)
		result |= SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);
	result |= SDL_RenderDrawLine(renderer, x1, y, x2, y);
	return result;
}

int vline(SDL_Renderer *renderer, int16_t x, int16_t y1, int16_t y2)
{
	return SDL_RenderDrawLine(renderer, x, y1, x, y2);
	;
}

int vlineColor(SDL_Renderer *renderer, int16_t x, int16_t y1, int16_t y2, uint32_t color)
{
	uint8_t *c = (uint8_t *)&color;
	return vlineRGBA(renderer, x, y1, y2, c[0], c[1], c[2], c[3]);
}

int vlineRGBA(SDL_Renderer *renderer, int16_t x, int16_t y1, int16_t y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	int result = 0;
	if (a != 255)
		result |= SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);
	result |= SDL_RenderDrawLine(renderer, x, y1, x, y2);
	return result;
}

int rectangleColor(SDL_Renderer *renderer, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint32_t color)
{
	uint8_t *c = (uint8_t *)&color;
	return rectangleRGBA(renderer, x1, y1, x2, y2, c[0], c[1], c[2], c[3]);
}

int rectangleRGBA(SDL_Renderer *renderer, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	int result;
	int16_t tmp;
	SDL_Rect rect;

	if (x1 == x2)
	{
		if (y1 == y2)
		{
			return (pixelRGBA(renderer, x1, y1, r, g, b, a));
		}
		else
		{
			return (vlineRGBA(renderer, x1, y1, y2, r, g, b, a));
		}
	}
	else
	{
		if (y1 == y2)
		{
			return (hlineRGBA(renderer, x1, x2, y1, r, g, b, a));
		}
	}

	if (x1 > x2)
	{
		tmp = x1;
		x1 = x2;
		x2 = tmp;
	}

	if (y1 > y2)
	{
		tmp = y1;
		y1 = y2;
		y2 = tmp;
	}

	rect.x = x1;
	rect.y = y1;
	rect.w = x2 - x1;
	rect.h = y2 - y1;

	result = 0;
	if (a != 255)
		result |= SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);
	result |= SDL_RenderDrawRect(renderer, &rect);
	return result;
}

int roundedRectangleColor(SDL_Renderer *renderer, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t rad, uint32_t color)
{
	uint8_t *c = (uint8_t *)&color;
	return roundedRectangleRGBA(renderer, x1, y1, x2, y2, rad, c[0], c[1], c[2], c[3]);
}

int roundedRectangleRGBA(SDL_Renderer *renderer, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t rad, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	int result = 0;
	int16_t tmp;
	int16_t w, h;
	int16_t xx1, xx2;
	int16_t yy1, yy2;

	if (renderer == NULL)
	{
		return -1;
	}

	if (rad < 0)
	{
		return -1;
	}

	if (rad <= 1)
	{
		return rectangleRGBA(renderer, x1, y1, x2, y2, r, g, b, a);
	}

	if (x1 == x2)
	{
		if (y1 == y2)
		{
			return (pixelRGBA(renderer, x1, y1, r, g, b, a));
		}
		else
		{
			return (vlineRGBA(renderer, x1, y1, y2, r, g, b, a));
		}
	}
	else
	{
		if (y1 == y2)
		{
			return (hlineRGBA(renderer, x1, x2, y1, r, g, b, a));
		}
	}

	if (x1 > x2)
	{
		tmp = x1;
		x1 = x2;
		x2 = tmp;
	}

	if (y1 > y2)
	{
		tmp = y1;
		y1 = y2;
		y2 = tmp;
	}

	w = x2 - x1;
	h = y2 - y1;

	if ((rad * 2) > w)
	{
		rad = w / 2;
	}
	if ((rad * 2) > h)
	{
		rad = h / 2;
	}

	xx1 = x1 + rad;
	xx2 = x2 - rad;
	yy1 = y1 + rad;
	yy2 = y2 - rad;
	result |= arcRGBA(renderer, xx1, yy1, rad, 180, 270, r, g, b, a);
	result |= arcRGBA(renderer, xx2, yy1, rad, 270, 360, r, g, b, a);
	result |= arcRGBA(renderer, xx1, yy2, rad, 90, 180, r, g, b, a);
	result |= arcRGBA(renderer, xx2, yy2, rad, 0, 90, r, g, b, a);

	if (xx1 <= xx2)
	{
		result |= hlineRGBA(renderer, xx1, xx2, y1, r, g, b, a);
		result |= hlineRGBA(renderer, xx1, xx2, y2, r, g, b, a);
	}
	if (yy1 <= yy2)
	{
		result |= vlineRGBA(renderer, x1, yy1, yy2, r, g, b, a);
		result |= vlineRGBA(renderer, x2, yy1, yy2, r, g, b, a);
	}

	return result;
}

int roundedBoxColor(SDL_Renderer *renderer, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t rad, uint32_t color)
{
	uint8_t *c = (uint8_t *)&color;
	return roundedBoxRGBA(renderer, x1, y1, x2, y2, rad, c[0], c[1], c[2], c[3]);
}

int roundedBoxRGBA(SDL_Renderer *renderer, int16_t x1, int16_t y1, int16_t x2,
				   int16_t y2, int16_t rad, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	int result;
	int16_t w, h, r2, tmp;
	int16_t cx = 0;
	int16_t cy = rad;
	int16_t ocx = (int16_t)0xffff;
	int16_t ocy = (int16_t)0xffff;
	int16_t df = 1 - rad;
	int16_t d_e = 3;
	int16_t d_se = -2 * rad + 5;
	int16_t xpcx, xmcx, xpcy, xmcy;
	int16_t ypcy, ymcy, ypcx, ymcx;
	int16_t x, y, dx, dy;

	if (renderer == NULL)
	{
		return -1;
	}

	if (rad < 0)
	{
		return -1;
	}

	if (rad <= 1)
	{
		return boxRGBA(renderer, x1, y1, x2, y2, r, g, b, a);
	}

	if (x1 == x2)
	{
		if (y1 == y2)
		{
			return (pixelRGBA(renderer, x1, y1, r, g, b, a));
		}
		else
		{
			return (vlineRGBA(renderer, x1, y1, y2, r, g, b, a));
		}
	}
	else
	{
		if (y1 == y2)
		{
			return (hlineRGBA(renderer, x1, x2, y1, r, g, b, a));
		}
	}

	if (x1 > x2)
	{
		tmp = x1;
		x1 = x2;
		x2 = tmp;
	}

	if (y1 > y2)
	{
		tmp = y1;
		y1 = y2;
		y2 = tmp;
	}

	w = x2 - x1 + 1;
	h = y2 - y1 + 1;

	r2 = rad + rad;
	if (r2 > w)
	{
		rad = w / 2;
		r2 = rad + rad;
	}
	if (r2 > h)
	{
		rad = h / 2;
	}

	x = x1 + rad;
	y = y1 + rad;
	dx = x2 - x1 - rad - rad;
	dy = y2 - y1 - rad - rad;

	result = 0;
	if (a != 255)
		result |= SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);

	do
	{
		xpcx = x + cx;
		xmcx = x - cx;
		xpcy = x + cy;
		xmcy = x - cy;
		if (ocy != cy)
		{
			if (cy > 0)
			{
				ypcy = y + cy;
				ymcy = y - cy;
				result |= hline(renderer, xmcx, xpcx + dx, ypcy + dy);
				result |= hline(renderer, xmcx, xpcx + dx, ymcy);
			}
			else
			{
				result |= hline(renderer, xmcx, xpcx + dx, y);
			}
			ocy = cy;
		}
		if (ocx != cx)
		{
			if (cx != cy)
			{
				if (cx > 0)
				{
					ypcx = y + cx;
					ymcx = y - cx;
					result |= hline(renderer, xmcy, xpcy + dx, ymcx);
					result |= hline(renderer, xmcy, xpcy + dx, ypcx + dy);
				}
				else
				{
					result |= hline(renderer, xmcy, xpcy + dx, y);
				}
			}
			ocx = cx;
		}

		if (df < 0)
		{
			df += d_e;
			d_e += 2;
			d_se += 2;
		}
		else
		{
			df += d_se;
			d_e += 2;
			d_se += 4;
			cy--;
		}
		cx++;
	} while (cx <= cy);

	if (dx > 0 && dy > 0)
	{
		result |= boxRGBA(renderer, x1, y1 + rad + 1, x2, y2 - rad, r, g, b, a);
	}

	return (result);
}

int boxColor(SDL_Renderer *renderer, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint32_t color)
{
	uint8_t *c = (uint8_t *)&color;
	return boxRGBA(renderer, x1, y1, x2, y2, c[0], c[1], c[2], c[3]);
}

int boxRGBA(SDL_Renderer *renderer, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	int result;
	int16_t tmp;
	SDL_Rect rect;

	if (x1 == x2)
	{
		if (y1 == y2)
		{
			return (pixelRGBA(renderer, x1, y1, r, g, b, a));
		}
		else
		{
			return (vlineRGBA(renderer, x1, y1, y2, r, g, b, a));
		}
	}
	else
	{
		if (y1 == y2)
		{
			return (hlineRGBA(renderer, x1, x2, y1, r, g, b, a));
		}
	}

	if (x1 > x2)
	{
		tmp = x1;
		x1 = x2;
		x2 = tmp;
	}

	if (y1 > y2)
	{
		tmp = y1;
		y1 = y2;
		y2 = tmp;
	}

	rect.x = x1;
	rect.y = y1;
	rect.w = x2 - x1 + 1;
	rect.h = y2 - y1 + 1;

	result = 0;
	if (a != 255)
		result |= SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);
	result |= SDL_RenderFillRect(renderer, &rect);
	return result;
}

int line(SDL_Renderer *renderer, int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{

	return SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

int lineColor(SDL_Renderer *renderer, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint32_t color)
{
	uint8_t *c = (uint8_t *)&color;
	return lineRGBA(renderer, x1, y1, x2, y2, c[0], c[1], c[2], c[3]);
}

int lineRGBA(SDL_Renderer *renderer, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{

	int result = 0;
	if (a != 255)
		result |= SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);
	result |= SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
	return result;
}

#define AAlevels 256
#define AAbits 8

int _aalineRGBA(SDL_Renderer *renderer, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a, int draw_endpoint)
{
	int32_t xx0, yy0, xx1, yy1;
	int result;
	uint32_t intshift, erracc, erradj;
	uint32_t erracctmp, wgt;
	int dx, dy, tmp, xdir, y0p1, x0pxdir;

	xx0 = x1;
	yy0 = y1;
	xx1 = x2;
	yy1 = y2;

	if (yy0 > yy1)
	{
		tmp = yy0;
		yy0 = yy1;
		yy1 = tmp;
		tmp = xx0;
		xx0 = xx1;
		xx1 = tmp;
	}

	dx = xx1 - xx0;
	dy = yy1 - yy0;

	if (dx >= 0)
	{
		xdir = 1;
	}
	else
	{
		xdir = -1;
		dx = (-dx);
	}

	if (dx == 0)
	{

		if (draw_endpoint)
		{
			return (vlineRGBA(renderer, x1, y1, y2, r, g, b, a));
		}
		else
		{
			if (dy > 0)
			{
				return (vlineRGBA(renderer, x1, yy0, yy0 + dy, r, g, b, a));
			}
			else
			{
				return (pixelRGBA(renderer, x1, y1, r, g, b, a));
			}
		}
	}
	else if (dy == 0)
	{

		if (draw_endpoint)
		{
			return (hlineRGBA(renderer, x1, x2, y1, r, g, b, a));
		}
		else
		{
			if (dx > 0)
			{
				return (hlineRGBA(renderer, xx0, xx0 + (xdir * dx), y1, r, g, b, a));
			}
			else
			{
				return (pixelRGBA(renderer, x1, y1, r, g, b, a));
			}
		}
	}
	else if ((dx == dy) && (draw_endpoint))
	{

		return (lineRGBA(renderer, x1, y1, x2, y2, r, g, b, a));
	}

	result = 0;

	erracc = 0;

	intshift = 32 - AAbits;

	result |= pixelRGBA(renderer, x1, y1, r, g, b, a);

	if (dy > dx)
	{

		erradj = ((dx << 16) / dy) << 16;

		x0pxdir = xx0 + xdir;
		while (--dy)
		{
			erracctmp = erracc;
			erracc += erradj;
			if (erracc <= erracctmp)
			{

				xx0 = x0pxdir;
				x0pxdir += xdir;
			}
			yy0++;

			wgt = (erracc >> intshift) & 255;
			result |= pixelRGBAWeight(renderer, xx0, yy0, r, g, b, a, 255 - wgt);
			result |= pixelRGBAWeight(renderer, x0pxdir, yy0, r, g, b, a, wgt);
		}
	}
	else
	{

		erradj = ((dy << 16) / dx) << 16;

		y0p1 = yy0 + 1;
		while (--dx)
		{

			erracctmp = erracc;
			erracc += erradj;
			if (erracc <= erracctmp)
			{

				yy0 = y0p1;
				y0p1++;
			}
			xx0 += xdir;

			wgt = (erracc >> intshift) & 255;
			result |= pixelRGBAWeight(renderer, xx0, yy0, r, g, b, a, 255 - wgt);
			result |= pixelRGBAWeight(renderer, xx0, y0p1, r, g, b, a, wgt);
		}
	}

	if (draw_endpoint)
	{

		result |= pixelRGBA(renderer, x2, y2, r, g, b, a);
	}

	return (result);
}

int aalineColor(SDL_Renderer *renderer, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint32_t color)
{
	uint8_t *c = (uint8_t *)&color;
	return _aalineRGBA(renderer, x1, y1, x2, y2, c[0], c[1], c[2], c[3], 1);
}

int aalineRGBA(SDL_Renderer *renderer, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	return _aalineRGBA(renderer, x1, y1, x2, y2, r, g, b, a, 1);
}

int circleColor(SDL_Renderer *renderer, int16_t x, int16_t y, int16_t rad, uint32_t color)
{
	uint8_t *c = (uint8_t *)&color;
	return ellipseRGBA(renderer, x, y, rad, rad, c[0], c[1], c[2], c[3]);
}

int circleRGBA(SDL_Renderer *renderer, int16_t x, int16_t y, int16_t rad, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	return ellipseRGBA(renderer, x, y, rad, rad, r, g, b, a);
}

int arcColor(SDL_Renderer *renderer, int16_t x, int16_t y, int16_t rad, int16_t start, int16_t end, uint32_t color)
{
	uint8_t *c = (uint8_t *)&color;
	return arcRGBA(renderer, x, y, rad, start, end, c[0], c[1], c[2], c[3]);
}

int arcRGBA(SDL_Renderer *renderer, int16_t x, int16_t y, int16_t rad, int16_t start, int16_t end, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	int result;
	int16_t cx = 0;
	int16_t cy = rad;
	int16_t df = 1 - rad;
	int16_t d_e = 3;
	int16_t d_se = -2 * rad + 5;
	int16_t xpcx, xmcx, xpcy, xmcy;
	int16_t ypcy, ymcy, ypcx, ymcx;
	uint8_t drawoct;
	int startoct, endoct, oct, stopval_start = 0, stopval_end = 0;
	double dstart, dend, temp = 0.;

	if (rad < 0)
	{
		return (-1);
	}

	if (rad == 0)
	{
		return (pixelRGBA(renderer, x, y, r, g, b, a));
	}

	drawoct = 0;

	start %= 360;
	end %= 360;

	while (start < 0)
		start += 360;
	while (end < 0)
		end += 360;
	start %= 360;
	end %= 360;

	startoct = start / 45;
	endoct = end / 45;
	oct = startoct - 1;

	do
	{
		oct = (oct + 1) % 8;

		if (oct == startoct)
		{

			dstart = (double)start;
			switch (oct)
			{
			case 0:
			case 3:
				temp = sin(dstart * M_PI / 180.);
				break;
			case 1:
			case 6:
				temp = cos(dstart * M_PI / 180.);
				break;
			case 2:
			case 5:
				temp = -cos(dstart * M_PI / 180.);
				break;
			case 4:
			case 7:
				temp = -sin(dstart * M_PI / 180.);
				break;
			}
			temp *= rad;
			stopval_start = (int)temp;

			if (oct % 2)
				drawoct |= (1 << oct);
			else
				drawoct &= 255 - (1 << oct);
		}
		if (oct == endoct)
		{

			dend = (double)end;
			switch (oct)
			{
			case 0:
			case 3:
				temp = sin(dend * M_PI / 180);
				break;
			case 1:
			case 6:
				temp = cos(dend * M_PI / 180);
				break;
			case 2:
			case 5:
				temp = -cos(dend * M_PI / 180);
				break;
			case 4:
			case 7:
				temp = -sin(dend * M_PI / 180);
				break;
			}
			temp *= rad;
			stopval_end = (int)temp;

			if (startoct == endoct)
			{

				if (start > end)
				{

					drawoct = 255;
				}
				else
				{
					drawoct &= 255 - (1 << oct);
				}
			}
			else if (oct % 2)
				drawoct &= 255 - (1 << oct);
			else
				drawoct |= (1 << oct);
		}
		else if (oct != startoct)
		{
			drawoct |= (1 << oct);
		}
	} while (oct != endoct);

	result = 0;
	if (a != 255)
		result |= SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);

	do
	{
		ypcy = y + cy;
		ymcy = y - cy;
		if (cx > 0)
		{
			xpcx = x + cx;
			xmcx = x - cx;

			if (drawoct & 4)
				result |= pixel(renderer, xmcx, ypcy);
			if (drawoct & 2)
				result |= pixel(renderer, xpcx, ypcy);
			if (drawoct & 32)
				result |= pixel(renderer, xmcx, ymcy);
			if (drawoct & 64)
				result |= pixel(renderer, xpcx, ymcy);
		}
		else
		{
			if (drawoct & 96)
				result |= pixel(renderer, x, ymcy);
			if (drawoct & 6)
				result |= pixel(renderer, x, ypcy);
		}

		xpcy = x + cy;
		xmcy = x - cy;
		if (cx > 0 && cx != cy)
		{
			ypcx = y + cx;
			ymcx = y - cx;
			if (drawoct & 8)
				result |= pixel(renderer, xmcy, ypcx);
			if (drawoct & 1)
				result |= pixel(renderer, xpcy, ypcx);
			if (drawoct & 16)
				result |= pixel(renderer, xmcy, ymcx);
			if (drawoct & 128)
				result |= pixel(renderer, xpcy, ymcx);
		}
		else if (cx == 0)
		{
			if (drawoct & 24)
				result |= pixel(renderer, xmcy, y);
			if (drawoct & 129)
				result |= pixel(renderer, xpcy, y);
		}

		if (stopval_start == cx)
		{

			if (drawoct & (1 << startoct))
				drawoct &= 255 - (1 << startoct);
			else
				drawoct |= (1 << startoct);
		}
		if (stopval_end == cx)
		{
			if (drawoct & (1 << endoct))
				drawoct &= 255 - (1 << endoct);
			else
				drawoct |= (1 << endoct);
		}

		if (df < 0)
		{
			df += d_e;
			d_e += 2;
			d_se += 2;
		}
		else
		{
			df += d_se;
			d_e += 2;
			d_se += 4;
			cy--;
		}
		cx++;
	} while (cx <= cy);

	return (result);
}

int aacircleColor(SDL_Renderer *renderer, int16_t x, int16_t y, int16_t rad, uint32_t color)
{
	uint8_t *c = (uint8_t *)&color;
	return aaellipseRGBA(renderer, x, y, rad, rad, c[0], c[1], c[2], c[3]);
}

int aacircleRGBA(SDL_Renderer *renderer, int16_t x, int16_t y, int16_t rad, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{

	return aaellipseRGBA(renderer, x, y, rad, rad, r, g, b, a);
}

int filledCircleColor(SDL_Renderer *renderer, int16_t x, int16_t y, int16_t rad, uint32_t color)
{
	uint8_t *c = (uint8_t *)&color;
	return filledEllipseRGBA(renderer, x, y, rad, rad, c[0], c[1], c[2], c[3]);
}

int filledCircleRGBA(SDL_Renderer *renderer, int16_t x, int16_t y, int16_t rad, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	return filledEllipseRGBA(renderer, x, y, rad, rad, r, g, b, a);
}

int ellipseColor(SDL_Renderer *renderer, int16_t x, int16_t y, int16_t rx, int16_t ry, uint32_t color)
{
	uint8_t *c = (uint8_t *)&color;
	return ellipseRGBA(renderer, x, y, rx, ry, c[0], c[1], c[2], c[3]);
}

int ellipseRGBA(SDL_Renderer *renderer, int16_t x, int16_t y, int16_t rx, int16_t ry, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	int result;
	int ix, iy;
	int h, i, j, k;
	int oh, oi, oj, ok;
	int xmh, xph, ypk, ymk;
	int xmi, xpi, ymj, ypj;
	int xmj, xpj, ymi, ypi;
	int xmk, xpk, ymh, yph;

	if ((rx < 0) || (ry < 0))
	{
		return (-1);
	}

	if (rx == 0)
	{
		return (vlineRGBA(renderer, x, y - ry, y + ry, r, g, b, a));
	}

	if (ry == 0)
	{
		return (hlineRGBA(renderer, x - rx, x + rx, y, r, g, b, a));
	}

	result = 0;
	if (a != 255)
		result |= SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);

	oh = oi = oj = ok = 0xFFFF;

	if (rx > ry)
	{
		ix = 0;
		iy = rx * 64;

		do
		{
			h = (ix + 32) >> 6;
			i = (iy + 32) >> 6;
			j = (h * ry) / rx;
			k = (i * ry) / rx;

			if (((ok != k) && (oj != k)) || ((oj != j) && (ok != j)) || (k != j))
			{
				xph = x + h;
				xmh = x - h;
				if (k > 0)
				{
					ypk = y + k;
					ymk = y - k;
					result |= pixel(renderer, xmh, ypk);
					result |= pixel(renderer, xph, ypk);
					result |= pixel(renderer, xmh, ymk);
					result |= pixel(renderer, xph, ymk);
				}
				else
				{
					result |= pixel(renderer, xmh, y);
					result |= pixel(renderer, xph, y);
				}
				ok = k;
				xpi = x + i;
				xmi = x - i;
				if (j > 0)
				{
					ypj = y + j;
					ymj = y - j;
					result |= pixel(renderer, xmi, ypj);
					result |= pixel(renderer, xpi, ypj);
					result |= pixel(renderer, xmi, ymj);
					result |= pixel(renderer, xpi, ymj);
				}
				else
				{
					result |= pixel(renderer, xmi, y);
					result |= pixel(renderer, xpi, y);
				}
				oj = j;
			}

			ix = ix + iy / rx;
			iy = iy - ix / rx;

		} while (i > h);
	}
	else
	{
		ix = 0;
		iy = ry * 64;

		do
		{
			h = (ix + 32) >> 6;
			i = (iy + 32) >> 6;
			j = (h * rx) / ry;
			k = (i * rx) / ry;

			if (((oi != i) && (oh != i)) || ((oh != h) && (oi != h) && (i != h)))
			{
				xmj = x - j;
				xpj = x + j;
				if (i > 0)
				{
					ypi = y + i;
					ymi = y - i;
					result |= pixel(renderer, xmj, ypi);
					result |= pixel(renderer, xpj, ypi);
					result |= pixel(renderer, xmj, ymi);
					result |= pixel(renderer, xpj, ymi);
				}
				else
				{
					result |= pixel(renderer, xmj, y);
					result |= pixel(renderer, xpj, y);
				}
				oi = i;
				xmk = x - k;
				xpk = x + k;
				if (h > 0)
				{
					yph = y + h;
					ymh = y - h;
					result |= pixel(renderer, xmk, yph);
					result |= pixel(renderer, xpk, yph);
					result |= pixel(renderer, xmk, ymh);
					result |= pixel(renderer, xpk, ymh);
				}
				else
				{
					result |= pixel(renderer, xmk, y);
					result |= pixel(renderer, xpk, y);
				}
				oh = h;
			}

			ix = ix + iy / ry;
			iy = iy - ix / ry;

		} while (i > h);
	}

	return (result);
}

#if defined(_MSC_VER)

#ifdef _M_X64
#include <emmintrin.h>
static __inline long
lrint(float f)
{
	return _mm_cvtss_si32(_mm_load_ss(&f));
}
#elif defined(_M_IX86)
__inline long int
lrint(double flt)
{
	int intgr;
	_asm
		{
		fld flt
			fistp intgr
		}
	;
	return intgr;
}
#elif defined(_M_ARM)
#include <armintr.h>
#pragma warning(push)
#pragma warning(disable : 4716)
__declspec(naked) long int lrint(double flt)
{
	__emit(0xEC410B10);
	__emit(0xEEBD0B40);
	__emit(0xEE100A10);
	__emit(0xE12FFF1E);
}
#pragma warning(pop)
#else
#error lrint needed for MSVC on non X86/AMD64/ARM targets.
#endif
#endif

int aaellipseColor(SDL_Renderer *renderer, int16_t x, int16_t y, int16_t rx, int16_t ry, uint32_t color)
{
	uint8_t *c = (uint8_t *)&color;
	return aaellipseRGBA(renderer, x, y, rx, ry, c[0], c[1], c[2], c[3]);
}

int aaellipseRGBA(SDL_Renderer *renderer, int16_t x, int16_t y, int16_t rx, int16_t ry, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	int result;
	int i;
	int a2, b2, ds, dt, dxt, t, s, d;
	int16_t xp, yp, xs, ys, dyt, od, xx, yy, xc2, yc2;
	float cp;
	double sab;
	uint8_t weight, iweight;

	if ((rx < 0) || (ry < 0))
	{
		return (-1);
	}

	if (rx == 0)
	{
		if (ry == 0)
		{
			return (pixel(renderer, x, y));
		}
		else
		{
			return (vline(renderer, x, y - ry, y + ry));
		}
	}
	else
	{
		if (ry == 0)
		{
			return (hline(renderer, x - rx, x + rx, y));
		}
	}

	a2 = rx * rx;
	b2 = ry * ry;

	ds = 2 * a2;
	dt = 2 * b2;

	xc2 = 2 * x;
	yc2 = 2 * y;

	sab = sqrt((double)(a2 + b2));
	od = (int16_t)lrint(sab * 0.01) + 1;
	dxt = (int16_t)lrint((double)a2 / sab) + od;

	t = 0;
	s = -2 * a2 * ry;
	d = 0;

	xp = x;
	yp = y - ry;

	result = 0;
	if (a != 255)
		result |= SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	result |= pixelRGBA(renderer, xp, yp, r, g, b, a);
	result |= pixelRGBA(renderer, xc2 - xp, yp, r, g, b, a);
	result |= pixelRGBA(renderer, xp, yc2 - yp, r, g, b, a);
	result |= pixelRGBA(renderer, xc2 - xp, yc2 - yp, r, g, b, a);

	for (i = 1; i <= dxt; i++)
	{
		xp--;
		d += t - b2;

		if (d >= 0)
			ys = yp - 1;
		else if ((d - s - a2) > 0)
		{
			if ((2 * d - s - a2) >= 0)
				ys = yp + 1;
			else
			{
				ys = yp;
				yp++;
				d -= s + a2;
				s += ds;
			}
		}
		else
		{
			yp++;
			ys = yp + 1;
			d -= s + a2;
			s += ds;
		}

		t -= dt;

		if (s != 0)
		{
			cp = (float)abs(d) / (float)abs(s);
			if (cp > 1.0)
			{
				cp = 1.0;
			}
		}
		else
		{
			cp = 1.0;
		}

		weight = (uint8_t)(cp * 255);
		iweight = 255 - weight;

		xx = xc2 - xp;
		result |= pixelRGBAWeight(renderer, xp, yp, r, g, b, a, iweight);
		result |= pixelRGBAWeight(renderer, xx, yp, r, g, b, a, iweight);

		result |= pixelRGBAWeight(renderer, xp, ys, r, g, b, a, weight);
		result |= pixelRGBAWeight(renderer, xx, ys, r, g, b, a, weight);

		yy = yc2 - yp;
		result |= pixelRGBAWeight(renderer, xp, yy, r, g, b, a, iweight);
		result |= pixelRGBAWeight(renderer, xx, yy, r, g, b, a, iweight);

		yy = yc2 - ys;
		result |= pixelRGBAWeight(renderer, xp, yy, r, g, b, a, weight);
		result |= pixelRGBAWeight(renderer, xx, yy, r, g, b, a, weight);
	}

	dyt = (int16_t)lrint((double)b2 / sab) + od;

	for (i = 1; i <= dyt; i++)
	{
		yp++;
		d -= s + a2;

		if (d <= 0)
			xs = xp + 1;
		else if ((d + t - b2) < 0)
		{
			if ((2 * d + t - b2) <= 0)
				xs = xp - 1;
			else
			{
				xs = xp;
				xp--;
				d += t - b2;
				t -= dt;
			}
		}
		else
		{
			xp--;
			xs = xp - 1;
			d += t - b2;
			t -= dt;
		}

		s += ds;

		if (t != 0)
		{
			cp = (float)abs(d) / (float)abs(t);
			if (cp > 1.0)
			{
				cp = 1.0;
			}
		}
		else
		{
			cp = 1.0;
		}

		weight = (uint8_t)(cp * 255);
		iweight = 255 - weight;

		xx = xc2 - xp;
		yy = yc2 - yp;
		result |= pixelRGBAWeight(renderer, xp, yp, r, g, b, a, iweight);
		result |= pixelRGBAWeight(renderer, xx, yp, r, g, b, a, iweight);

		result |= pixelRGBAWeight(renderer, xp, yy, r, g, b, a, iweight);
		result |= pixelRGBAWeight(renderer, xx, yy, r, g, b, a, iweight);

		xx = xc2 - xs;
		result |= pixelRGBAWeight(renderer, xs, yp, r, g, b, a, weight);
		result |= pixelRGBAWeight(renderer, xx, yp, r, g, b, a, weight);

		result |= pixelRGBAWeight(renderer, xs, yy, r, g, b, a, weight);
		result |= pixelRGBAWeight(renderer, xx, yy, r, g, b, a, weight);
	}

	return (result);
}

int filledEllipseColor(SDL_Renderer *renderer, int16_t x, int16_t y, int16_t rx, int16_t ry, uint32_t color)
{
	uint8_t *c = (uint8_t *)&color;
	return filledEllipseRGBA(renderer, x, y, rx, ry, c[0], c[1], c[2], c[3]);
}

int filledEllipseRGBA(SDL_Renderer *renderer, int16_t x, int16_t y, int16_t rx, int16_t ry, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	int result;
	int ix, iy;
	int h, i, j, k;
	int oh, oi, oj, ok;
	int xmh, xph;
	int xmi, xpi;
	int xmj, xpj;
	int xmk, xpk;

	if ((rx < 0) || (ry < 0))
	{
		return (-1);
	}

	if (rx == 0)
	{
		return (vlineRGBA(renderer, x, y - ry, y + ry, r, g, b, a));
	}

	if (ry == 0)
	{
		return (hlineRGBA(renderer, x - rx, x + rx, y, r, g, b, a));
	}

	result = 0;
	if (a != 255)
		result |= SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);

	oh = oi = oj = ok = 0xFFFF;

	if (rx > ry)
	{
		ix = 0;
		iy = rx * 64;

		do
		{
			h = (ix + 32) >> 6;
			i = (iy + 32) >> 6;
			j = (h * ry) / rx;
			k = (i * ry) / rx;

			if ((ok != k) && (oj != k))
			{
				xph = x + h;
				xmh = x - h;
				if (k > 0)
				{
					result |= hline(renderer, xmh, xph, y + k);
					result |= hline(renderer, xmh, xph, y - k);
				}
				else
				{
					result |= hline(renderer, xmh, xph, y);
				}
				ok = k;
			}
			if ((oj != j) && (ok != j) && (k != j))
			{
				xmi = x - i;
				xpi = x + i;
				if (j > 0)
				{
					result |= hline(renderer, xmi, xpi, y + j);
					result |= hline(renderer, xmi, xpi, y - j);
				}
				else
				{
					result |= hline(renderer, xmi, xpi, y);
				}
				oj = j;
			}

			ix = ix + iy / rx;
			iy = iy - ix / rx;

		} while (i > h);
	}
	else
	{
		ix = 0;
		iy = ry * 64;

		do
		{
			h = (ix + 32) >> 6;
			i = (iy + 32) >> 6;
			j = (h * rx) / ry;
			k = (i * rx) / ry;

			if ((oi != i) && (oh != i))
			{
				xmj = x - j;
				xpj = x + j;
				if (i > 0)
				{
					result |= hline(renderer, xmj, xpj, y + i);
					result |= hline(renderer, xmj, xpj, y - i);
				}
				else
				{
					result |= hline(renderer, xmj, xpj, y);
				}
				oi = i;
			}
			if ((oh != h) && (oi != h) && (i != h))
			{
				xmk = x - k;
				xpk = x + k;
				if (h > 0)
				{
					result |= hline(renderer, xmk, xpk, y + h);
					result |= hline(renderer, xmk, xpk, y - h);
				}
				else
				{
					result |= hline(renderer, xmk, xpk, y);
				}
				oh = h;
			}

			ix = ix + iy / ry;
			iy = iy - ix / ry;

		} while (i > h);
	}

	return (result);
}

int _pieRGBA(SDL_Renderer *renderer, int16_t x, int16_t y, int16_t rad, int16_t start, int16_t end, uint8_t r, uint8_t g, uint8_t b, uint8_t a, uint8_t filled)
{
	int result;
	double angle, start_angle, end_angle;
	double deltaAngle;
	double dr;
	int numpoints, i;
	int16_t *vx, *vy;

	if (rad < 0)
	{
		return (-1);
	}

	start = start % 360;
	end = end % 360;

	if (rad == 0)
	{
		return (pixelRGBA(renderer, x, y, r, g, b, a));
	}

	dr = (double)rad;
	deltaAngle = 3.0 / dr;
	start_angle = (double)start * (2.0 * M_PI / 360.0);
	end_angle = (double)end * (2.0 * M_PI / 360.0);
	if (start > end)
	{
		end_angle += (2.0 * M_PI);
	}

	numpoints = 2;

	angle = start_angle;
	while (angle < end_angle)
	{
		angle += deltaAngle;
		numpoints++;
	}

	vx = vy = (int16_t *)malloc(2 * sizeof(int16_t) * numpoints);
	if (vx == NULL)
	{
		return (-1);
	}

	vy += numpoints;

	vx[0] = x;
	vy[0] = y;

	angle = start_angle;
	vx[1] = x + (int)(dr * cos(angle));
	vy[1] = y + (int)(dr * sin(angle));

	if (numpoints < 3)
	{
		result = lineRGBA(renderer, vx[0], vy[0], vx[1], vy[1], r, g, b, a);
	}
	else
	{

		i = 2;
		angle = start_angle;
		while (angle < end_angle)
		{
			angle += deltaAngle;
			if (angle > end_angle)
			{
				angle = end_angle;
			}
			vx[i] = x + (int)(dr * cos(angle));
			vy[i] = y + (int)(dr * sin(angle));
			i++;
		}

		if (filled)
		{
			result = filledPolygonRGBA(renderer, vx, vy, numpoints, r, g, b, a);
		}
		else
		{
			result = polygonRGBA(renderer, vx, vy, numpoints, r, g, b, a);
		}
	}

	free(vx);

	return (result);
}

int pieColor(SDL_Renderer *renderer, int16_t x, int16_t y, int16_t rad,
			 int16_t start, int16_t end, uint32_t color)
{
	uint8_t *c = (uint8_t *)&color;
	return _pieRGBA(renderer, x, y, rad, start, end, c[0], c[1], c[2], c[3], 0);
}

int pieRGBA(SDL_Renderer *renderer, int16_t x, int16_t y, int16_t rad,
			int16_t start, int16_t end, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	return _pieRGBA(renderer, x, y, rad, start, end, r, g, b, a, 0);
}

int filledPieColor(SDL_Renderer *renderer, int16_t x, int16_t y, int16_t rad, int16_t start, int16_t end, uint32_t color)
{
	uint8_t *c = (uint8_t *)&color;
	return _pieRGBA(renderer, x, y, rad, start, end, c[0], c[1], c[2], c[3], 1);
}

int filledPieRGBA(SDL_Renderer *renderer, int16_t x, int16_t y, int16_t rad,
				  int16_t start, int16_t end, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	return _pieRGBA(renderer, x, y, rad, start, end, r, g, b, a, 1);
}

int trigonColor(SDL_Renderer *renderer, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint32_t color)
{
	int16_t vx[3];
	int16_t vy[3];

	vx[0] = x1;
	vx[1] = x2;
	vx[2] = x3;
	vy[0] = y1;
	vy[1] = y2;
	vy[2] = y3;

	return (polygonColor(renderer, vx, vy, 3, color));
}

int trigonRGBA(SDL_Renderer *renderer, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3,
			   uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	int16_t vx[3];
	int16_t vy[3];

	vx[0] = x1;
	vx[1] = x2;
	vx[2] = x3;
	vy[0] = y1;
	vy[1] = y2;
	vy[2] = y3;

	return (polygonRGBA(renderer, vx, vy, 3, r, g, b, a));
}

int aatrigonColor(SDL_Renderer *renderer, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint32_t color)
{
	int16_t vx[3];
	int16_t vy[3];

	vx[0] = x1;
	vx[1] = x2;
	vx[2] = x3;
	vy[0] = y1;
	vy[1] = y2;
	vy[2] = y3;

	return (aapolygonColor(renderer, vx, vy, 3, color));
}

int aatrigonRGBA(SDL_Renderer *renderer, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3,
				 uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	int16_t vx[3];
	int16_t vy[3];

	vx[0] = x1;
	vx[1] = x2;
	vx[2] = x3;
	vy[0] = y1;
	vy[1] = y2;
	vy[2] = y3;

	return (aapolygonRGBA(renderer, vx, vy, 3, r, g, b, a));
}

int filledTrigonColor(SDL_Renderer *renderer, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint32_t color)
{
	int16_t vx[3];
	int16_t vy[3];

	vx[0] = x1;
	vx[1] = x2;
	vx[2] = x3;
	vy[0] = y1;
	vy[1] = y2;
	vy[2] = y3;

	return (filledPolygonColor(renderer, vx, vy, 3, color));
}

int filledTrigonRGBA(SDL_Renderer *renderer, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3,
					 uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	int16_t vx[3];
	int16_t vy[3];

	vx[0] = x1;
	vx[1] = x2;
	vx[2] = x3;
	vy[0] = y1;
	vy[1] = y2;
	vy[2] = y3;

	return (filledPolygonRGBA(renderer, vx, vy, 3, r, g, b, a));
}

int polygonColor(SDL_Renderer *renderer, const int16_t *vx, const int16_t *vy, int n, uint32_t color)
{
	uint8_t *c = (uint8_t *)&color;
	return polygonRGBA(renderer, vx, vy, n, c[0], c[1], c[2], c[3]);
}

int polygon(SDL_Renderer *renderer, const int16_t *vx, const int16_t *vy, int n)
{

	int result = 0;
	int i, nn;
	SDL_Point *points;

	if (vx == NULL)
	{
		return (-1);
	}
	if (vy == NULL)
	{
		return (-1);
	}

	if (n < 3)
	{
		return (-1);
	}

	nn = n + 1;
	points = (SDL_Point *)malloc(sizeof(SDL_Point) * nn);
	if (points == NULL)
	{
		return -1;
	}
	for (i = 0; i < n; i++)
	{
		points[i].x = vx[i];
		points[i].y = vy[i];
	}
	points[n].x = vx[0];
	points[n].y = vy[0];

	result |= SDL_RenderDrawLines(renderer, points, nn);
	free(points);

	return (result);
}

int polygonRGBA(SDL_Renderer *renderer, const int16_t *vx, const int16_t *vy, int n, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{

	int result;

	if (vx == NULL)
	{
		return (-1);
	}
	if (vy == NULL)
	{
		return (-1);
	}

	if (n < 3)
	{
		return (-1);
	}

	result = 0;
	if (a != 255)
		result |= SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);

	result |= polygon(renderer, vx, vy, n);

	return (result);
}

int aapolygonColor(SDL_Renderer *renderer, const int16_t *vx, const int16_t *vy, int n, uint32_t color)
{
	uint8_t *c = (uint8_t *)&color;
	return aapolygonRGBA(renderer, vx, vy, n, c[0], c[1], c[2], c[3]);
}

int aapolygonRGBA(SDL_Renderer *renderer, const int16_t *vx, const int16_t *vy, int n, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	int result;
	int i;
	const int16_t *x1, *y1, *x2, *y2;

	if (vx == NULL)
	{
		return (-1);
	}
	if (vy == NULL)
	{
		return (-1);
	}

	if (n < 3)
	{
		return (-1);
	}

	x1 = x2 = vx;
	y1 = y2 = vy;
	x2++;
	y2++;

	result = 0;
	for (i = 1; i < n; i++)
	{
		result |= _aalineRGBA(renderer, *x1, *y1, *x2, *y2, r, g, b, a, 0);
		x1 = x2;
		y1 = y2;
		x2++;
		y2++;
	}

	result |= _aalineRGBA(renderer, *x1, *y1, *vx, *vy, r, g, b, a, 0);

	return (result);
}

int _gfxPrimitivesCompareInt(const void *a, const void *b)
{
	return (*(const int *)a) - (*(const int *)b);
}

static int *gfxPrimitivesPolyIntsGlobal = NULL;

static int gfxPrimitivesPolyAllocatedGlobal = 0;

int filledPolygonRGBAMT(SDL_Renderer *renderer, const int16_t *vx, const int16_t *vy, int n, uint8_t r, uint8_t g, uint8_t b, uint8_t a, int **polyInts, int *polyAllocated)
{
	int result;
	int i;
	int y, xa, xb;
	int miny, maxy;
	int x1, y1;
	int x2, y2;
	int ind1, ind2;
	int ints;
	int *gfxPrimitivesPolyInts = NULL;
	int *gfxPrimitivesPolyIntsNew = NULL;
	int gfxPrimitivesPolyAllocated = 0;

	if (vx == NULL)
	{
		return (-1);
	}
	if (vy == NULL)
	{
		return (-1);
	}

	if (n < 3)
	{
		return -1;
	}

	if ((polyInts == NULL) || (polyAllocated == NULL))
	{

		gfxPrimitivesPolyInts = gfxPrimitivesPolyIntsGlobal;
		gfxPrimitivesPolyAllocated = gfxPrimitivesPolyAllocatedGlobal;
	}
	else
	{

		gfxPrimitivesPolyInts = *polyInts;
		gfxPrimitivesPolyAllocated = *polyAllocated;
	}

	if (!gfxPrimitivesPolyAllocated)
	{
		gfxPrimitivesPolyInts = (int *)malloc(sizeof(int) * n);
		gfxPrimitivesPolyAllocated = n;
	}
	else
	{
		if (gfxPrimitivesPolyAllocated < n)
		{
			gfxPrimitivesPolyIntsNew = (int *)realloc(gfxPrimitivesPolyInts, sizeof(int) * n);
			if (!gfxPrimitivesPolyIntsNew)
			{
				if (!gfxPrimitivesPolyInts)
				{
					free(gfxPrimitivesPolyInts);
					gfxPrimitivesPolyInts = NULL;
				}
				gfxPrimitivesPolyAllocated = 0;
			}
			else
			{
				gfxPrimitivesPolyInts = gfxPrimitivesPolyIntsNew;
				gfxPrimitivesPolyAllocated = n;
			}
		}
	}

	if (gfxPrimitivesPolyInts == NULL)
	{
		gfxPrimitivesPolyAllocated = 0;
	}

	if ((polyInts == NULL) || (polyAllocated == NULL))
	{
		gfxPrimitivesPolyIntsGlobal = gfxPrimitivesPolyInts;
		gfxPrimitivesPolyAllocatedGlobal = gfxPrimitivesPolyAllocated;
	}
	else
	{
		*polyInts = gfxPrimitivesPolyInts;
		*polyAllocated = gfxPrimitivesPolyAllocated;
	}

	if (gfxPrimitivesPolyInts == NULL)
	{
		return (-1);
	}

	miny = vy[0];
	maxy = vy[0];
	for (i = 1; (i < n); i++)
	{
		if (vy[i] < miny)
		{
			miny = vy[i];
		}
		else if (vy[i] > maxy)
		{
			maxy = vy[i];
		}
	}

	result = 0;
	for (y = miny; (y <= maxy); y++)
	{
		ints = 0;
		for (i = 0; (i < n); i++)
		{
			if (!i)
			{
				ind1 = n - 1;
				ind2 = 0;
			}
			else
			{
				ind1 = i - 1;
				ind2 = i;
			}
			y1 = vy[ind1];
			y2 = vy[ind2];
			if (y1 < y2)
			{
				x1 = vx[ind1];
				x2 = vx[ind2];
			}
			else if (y1 > y2)
			{
				y2 = vy[ind1];
				y1 = vy[ind2];
				x2 = vx[ind1];
				x1 = vx[ind2];
			}
			else
			{
				continue;
			}
			if (((y >= y1) && (y < y2)) || ((y == maxy) && (y > y1) && (y <= y2)))
			{
				gfxPrimitivesPolyInts[ints++] = ((65536 * (y - y1)) / (y2 - y1)) * (x2 - x1) + (65536 * x1);
			}
		}

		qsort(gfxPrimitivesPolyInts, ints, sizeof(int), _gfxPrimitivesCompareInt);

		result = 0;
		if (a != 255)
			result |= SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);

		for (i = 0; (i < ints); i += 2)
		{
			xa = gfxPrimitivesPolyInts[i] + 1;
			xa = (xa >> 16) + ((xa & 32768) >> 15);
			xb = gfxPrimitivesPolyInts[i + 1] - 1;
			xb = (xb >> 16) + ((xb & 32768) >> 15);
			result |= hline(renderer, xa, xb, y);
		}
	}

	return (result);
}

int filledPolygonColor(SDL_Renderer *renderer, const int16_t *vx, const int16_t *vy, int n, uint32_t color)
{
	uint8_t *c = (uint8_t *)&color;
	return filledPolygonRGBAMT(renderer, vx, vy, n, c[0], c[1], c[2], c[3], NULL, NULL);
}

int filledPolygonRGBA(SDL_Renderer *renderer, const int16_t *vx, const int16_t *vy, int n, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	return filledPolygonRGBAMT(renderer, vx, vy, n, r, g, b, a, NULL, NULL);
}

int _HLineTextured(SDL_Renderer *renderer, int16_t x1, int16_t x2, int16_t y, SDL_Texture *texture, int texture_w, int texture_h, int texture_dx, int texture_dy)
{
	int16_t w;
	int16_t xtmp;
	int result = 0;
	int texture_x_walker;
	int texture_y_start;
	SDL_Rect source_rect, dst_rect;
	int pixels_written, write_width;

	if (x1 > x2)
	{
		xtmp = x1;
		x1 = x2;
		x2 = xtmp;
	}

	w = x2 - x1 + 1;

	texture_x_walker = (x1 - texture_dx) % texture_w;
	if (texture_x_walker < 0)
	{
		texture_x_walker = texture_w + texture_x_walker;
	}

	texture_y_start = (y + texture_dy) % texture_h;
	if (texture_y_start < 0)
	{
		texture_y_start = texture_h + texture_y_start;
	}

	source_rect.y = texture_y_start;
	source_rect.x = texture_x_walker;
	source_rect.h = 1;

	dst_rect.y = y;
	dst_rect.h = 1;

	if (w <= texture_w - texture_x_walker)
	{
		source_rect.w = w;
		source_rect.x = texture_x_walker;
		dst_rect.x = x1;
		dst_rect.w = source_rect.w;
		result = (SDL_RenderCopy(renderer, texture, &source_rect, &dst_rect) == 0);
	}
	else
	{

		pixels_written = texture_w - texture_x_walker;
		source_rect.w = pixels_written;
		source_rect.x = texture_x_walker;
		dst_rect.x = x1;
		dst_rect.w = source_rect.w;
		result |= (SDL_RenderCopy(renderer, texture, &source_rect, &dst_rect) == 0);
		write_width = texture_w;

		source_rect.x = 0;
		while (pixels_written < w)
		{
			if (write_width >= w - pixels_written)
			{
				write_width = w - pixels_written;
			}
			source_rect.w = write_width;
			dst_rect.x = x1 + pixels_written;
			dst_rect.w = source_rect.w;
			result |= (SDL_RenderCopy(renderer, texture, &source_rect, &dst_rect) == 0);
			pixels_written += write_width;
		}
	}

	return result;
}

int texturedPolygonMT(SDL_Renderer *renderer, const int16_t *vx, const int16_t *vy, int n,
					  SDL_Surface *texture, int texture_dx, int texture_dy, int **polyInts, int *polyAllocated)
{
	int result;
	int i;
	int y, xa, xb;
	int minx, maxx, miny, maxy;
	int x1, y1;
	int x2, y2;
	int ind1, ind2;
	int ints;
	int *gfxPrimitivesPolyInts = NULL;
	int *gfxPrimitivesPolyIntsTemp = NULL;
	int gfxPrimitivesPolyAllocated = 0;
	SDL_Texture *textureAsTexture = NULL;

	if (n < 3)
	{
		return -1;
	}

	if ((polyInts == NULL) || (polyAllocated == NULL))
	{

		gfxPrimitivesPolyInts = gfxPrimitivesPolyIntsGlobal;
		gfxPrimitivesPolyAllocated = gfxPrimitivesPolyAllocatedGlobal;
	}
	else
	{

		gfxPrimitivesPolyInts = *polyInts;
		gfxPrimitivesPolyAllocated = *polyAllocated;
	}

	if (!gfxPrimitivesPolyAllocated)
	{
		gfxPrimitivesPolyInts = (int *)malloc(sizeof(int) * n);
		gfxPrimitivesPolyAllocated = n;
	}
	else
	{
		if (gfxPrimitivesPolyAllocated < n)
		{
			gfxPrimitivesPolyIntsTemp = (int *)realloc(gfxPrimitivesPolyInts, sizeof(int) * n);
			if (gfxPrimitivesPolyIntsTemp == NULL)
			{

				return (-1);
			}
			gfxPrimitivesPolyInts = gfxPrimitivesPolyIntsTemp;
			gfxPrimitivesPolyAllocated = n;
		}
	}

	if (gfxPrimitivesPolyInts == NULL)
	{
		gfxPrimitivesPolyAllocated = 0;
	}

	if ((polyInts == NULL) || (polyAllocated == NULL))
	{
		gfxPrimitivesPolyIntsGlobal = gfxPrimitivesPolyInts;
		gfxPrimitivesPolyAllocatedGlobal = gfxPrimitivesPolyAllocated;
	}
	else
	{
		*polyInts = gfxPrimitivesPolyInts;
		*polyAllocated = gfxPrimitivesPolyAllocated;
	}

	if (gfxPrimitivesPolyInts == NULL)
	{
		return (-1);
	}

	miny = vy[0];
	maxy = vy[0];
	minx = vx[0];
	maxx = vx[0];
	for (i = 1; (i < n); i++)
	{
		if (vy[i] < miny)
		{
			miny = vy[i];
		}
		else if (vy[i] > maxy)
		{
			maxy = vy[i];
		}
		if (vx[i] < minx)
		{
			minx = vx[i];
		}
		else if (vx[i] > maxx)
		{
			maxx = vx[i];
		}
	}

	textureAsTexture = SDL_CreateTextureFromSurface(renderer, texture);
	if (textureAsTexture == NULL)
	{
		return -1;
	}
	SDL_SetTextureBlendMode(textureAsTexture, SDL_BLENDMODE_BLEND);

	result = 0;
	for (y = miny; (y <= maxy); y++)
	{
		ints = 0;
		for (i = 0; (i < n); i++)
		{
			if (!i)
			{
				ind1 = n - 1;
				ind2 = 0;
			}
			else
			{
				ind1 = i - 1;
				ind2 = i;
			}
			y1 = vy[ind1];
			y2 = vy[ind2];
			if (y1 < y2)
			{
				x1 = vx[ind1];
				x2 = vx[ind2];
			}
			else if (y1 > y2)
			{
				y2 = vy[ind1];
				y1 = vy[ind2];
				x2 = vx[ind1];
				x1 = vx[ind2];
			}
			else
			{
				continue;
			}
			if (((y >= y1) && (y < y2)) || ((y == maxy) && (y > y1) && (y <= y2)))
			{
				gfxPrimitivesPolyInts[ints++] = ((65536 * (y - y1)) / (y2 - y1)) * (x2 - x1) + (65536 * x1);
			}
		}

		qsort(gfxPrimitivesPolyInts, ints, sizeof(int), _gfxPrimitivesCompareInt);

		for (i = 0; (i < ints); i += 2)
		{
			xa = gfxPrimitivesPolyInts[i] + 1;
			xa = (xa >> 16) + ((xa & 32768) >> 15);
			xb = gfxPrimitivesPolyInts[i + 1] - 1;
			xb = (xb >> 16) + ((xb & 32768) >> 15);
			result |= _HLineTextured(renderer, xa, xb, y, textureAsTexture, texture->w, texture->h, texture_dx, texture_dy);
		}
	}

	SDL_RenderPresent(renderer);
	SDL_DestroyTexture(textureAsTexture);

	return (result);
}

int texturedPolygon(SDL_Renderer *renderer, const int16_t *vx, const int16_t *vy, int n, SDL_Surface *texture, int texture_dx, int texture_dy)
{

	return (texturedPolygonMT(renderer, vx, vy, n, texture, texture_dx, texture_dy, NULL, NULL));
}

static SDL_Texture *gfxPrimitivesFont[256];

static const unsigned char *currentFontdata = gfxPrimitivesFontdata;

static uint32_t charWidth = 8;

static uint32_t charHeight = 8;

static uint32_t charWidthLocal = 8;

static uint32_t charHeightLocal = 8;

static uint32_t charPitch = 1;

static uint32_t charRotation = 0;
static uint32_t charZoomX = 1;
static uint32_t charZoomY = 1;

static uint32_t charSize = 8;

void gfxPrimitivesSetFont(const void *fontdata, uint32_t cw, uint32_t ch)
{
	int i;

	if ((fontdata) && (cw) && (ch))
	{
		currentFontdata = (unsigned char *)fontdata;
		charWidth = cw;
		charHeight = ch;
	}
	else
	{
		currentFontdata = gfxPrimitivesFontdata;
		charWidth = 8;
		charHeight = 8;
	}

	charPitch = (charWidth + 7) / 8;
	charSize = charPitch * charHeight;

	if ((charRotation == 1) || (charRotation == 3))
	{
		charWidthLocal = charHeight;
		charHeightLocal = charWidth;
	}
	else
	{
		charWidthLocal = charWidth;
		charHeightLocal = charHeight;
	}

	for (i = 0; i < 256; i++)
	{
		if (gfxPrimitivesFont[i])
		{
			SDL_DestroyTexture(gfxPrimitivesFont[i]);
			gfxPrimitivesFont[i] = NULL;
		}
	}
}

const void *gfxPrimitivesGetFont(void)
{
	return currentFontdata;
}

void gfxPrimitivesSetFontRotation(uint32_t rotation)
{
	int i;

	rotation = rotation & 3;
	if (charRotation != rotation)
	{

		charRotation = rotation;

		if ((charRotation == 1) || (charRotation == 3))
		{
			charWidthLocal = charHeight;
			charHeightLocal = charWidth;
		}
		else
		{
			charWidthLocal = charWidth;
			charHeightLocal = charHeight;
		}

		for (i = 0; i < 256; i++)
		{
			if (gfxPrimitivesFont[i])
			{
				SDL_DestroyTexture(gfxPrimitivesFont[i]);
				gfxPrimitivesFont[i] = NULL;
			}
		}
	}
}

void gfxPrimitivesSetFontZoom(uint32_t zoomx, uint32_t zoomy)
{
	int i;

	charZoomX = zoomx;
	charZoomY = zoomy;

	for (i = 0; i < 256; i++)
	{
		if (gfxPrimitivesFont[i])
		{
			SDL_DestroyTexture(gfxPrimitivesFont[i]);
			gfxPrimitivesFont[i] = NULL;
		}
	}
}

int characterRGBA(SDL_Renderer *renderer, int16_t x, int16_t y, char c, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	SDL_Rect srect;
	SDL_Rect drect;
	int result;
	uint32_t ix, iy;
	const unsigned char *charpos;
	uint8_t *curpos;
	uint8_t patt, mask;
	uint8_t *linepos;
	uint32_t pitch;
	SDL_Surface *character;
	SDL_Surface *rotatedCharacter;
	SDL_Surface *zoomedCharacter;
	uint32_t ci;

	srect.x = 0;
	srect.y = 0;
	srect.w = charWidthLocal * charZoomX;
	srect.h = charHeightLocal * charZoomY;

	drect.x = x;
	drect.y = y;
	drect.w = charWidthLocal * charZoomX;
	drect.h = charHeightLocal * charZoomY;

	ci = (unsigned char)c;

	if (gfxPrimitivesFont[ci] == NULL)
	{

		character = SDL_CreateRGBSurface(SDL_SWSURFACE,
										 charWidth, charHeight, 32,
										 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
		if (character == NULL)
		{
			return (-1);
		}

		charpos = currentFontdata + ci * charSize;
		linepos = (uint8_t *)character->pixels;
		pitch = character->pitch;

		patt = 0;
		for (iy = 0; iy < charHeight; iy++)
		{
			mask = 0x00;
			curpos = linepos;
			for (ix = 0; ix < charWidth; ix++)
			{
				if (!(mask >>= 1))
				{
					patt = *charpos++;
					mask = 0x80;
				}
				if (patt & mask)
				{
					*(uint32_t *)curpos = 0xffffffff;
				}
				else
				{
					*(uint32_t *)curpos = 0;
				}
				curpos += 4;
			}
			linepos += pitch;
		}

		if ((charZoomX != 1) || (charZoomY != 1))
		{
			zoomedCharacter = zoomSurface(character, (double)charZoomX,
										  (double)charZoomY, SMOOTHING_OFF);
			SDL_FreeSurface(character);
			character = zoomedCharacter;
		}

		if (charRotation > 0)
		{
			rotatedCharacter = rotateSurface90Degrees(character, charRotation);
			SDL_FreeSurface(character);
			character = rotatedCharacter;
		}

		gfxPrimitivesFont[ci] = SDL_CreateTextureFromSurface(renderer, character);
		SDL_FreeSurface(character);

		if (gfxPrimitivesFont[ci] == NULL)
		{
			return (-1);
		}
	}

	result = 0;
	result |= SDL_SetTextureColorMod(gfxPrimitivesFont[ci], r, g, b);
	result |= SDL_SetTextureAlphaMod(gfxPrimitivesFont[ci], a);

	result |= SDL_RenderCopy(renderer, gfxPrimitivesFont[ci], &srect, &drect);

	return (result);
}

int characterColor(SDL_Renderer *renderer, int16_t x, int16_t y, char c, uint32_t color)
{
	uint8_t *co = (uint8_t *)&color;
	return characterRGBA(renderer, x, y, c, co[0], co[1], co[2], co[3]);
}

int stringColor(SDL_Renderer *renderer, int16_t x, int16_t y, const char *s, uint32_t color)
{
	uint8_t *c = (uint8_t *)&color;
	return stringRGBA(renderer, x, y, s, c[0], c[1], c[2], c[3]);
}

int stringRGBA(SDL_Renderer *renderer, int16_t x, int16_t y, const char *s, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	int result = 0;
	int16_t curx = x;
	int16_t cury = y;
	const char *curchar = s;

	while (*curchar && !result)
	{
		result |= characterRGBA(renderer, curx, cury, *curchar, r, g, b, a);
		switch (charRotation)
		{
		case 0:
			curx += charWidthLocal * charZoomX;
			break;
		case 2:
			curx -= charWidthLocal * charZoomX;
			break;
		case 1:
			cury += charHeightLocal * charZoomY;
			break;
		case 3:
			cury -= charHeightLocal * charZoomY;
			break;
		}
		curchar++;
	}

	return (result);
}

double _evaluateBezier(double *data, int ndata, double t)
{
	double mu, result;
	int n, k, kn, nn, nkn;
	double blend, muk, munk;

	if (t < 0.0)
	{
		return (data[0]);
	}
	if (t >= (double)ndata)
	{
		return (data[ndata - 1]);
	}

	mu = t / (double)ndata;

	n = ndata - 1;
	result = 0.0;
	muk = 1;
	munk = pow(1 - mu, (double)n);
	for (k = 0; k <= n; k++)
	{
		nn = n;
		kn = k;
		nkn = n - k;
		blend = muk * munk;
		muk *= mu;
		munk /= (1 - mu);
		while (nn >= 1)
		{
			blend *= nn;
			nn--;
			if (kn > 1)
			{
				blend /= (double)kn;
				kn--;
			}
			if (nkn > 1)
			{
				blend /= (double)nkn;
				nkn--;
			}
		}
		result += data[k] * blend;
	}

	return (result);
}

int bezierColor(SDL_Renderer *renderer, const int16_t *vx, const int16_t *vy, int n, int s, uint32_t color)
{
	uint8_t *c = (uint8_t *)&color;
	return bezierRGBA(renderer, vx, vy, n, s, c[0], c[1], c[2], c[3]);
}

int bezierRGBA(SDL_Renderer *renderer, const int16_t *vx, const int16_t *vy, int n, int s, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	int result;
	int i;
	double *x, *y, t, stepsize;
	int16_t x1, y1, x2, y2;

	if (n < 3)
	{
		return (-1);
	}
	if (s < 2)
	{
		return (-1);
	}

	stepsize = (double)1.0 / (double)s;

	if ((x = (double *)malloc(sizeof(double) * (n + 1))) == NULL)
	{
		return (-1);
	}
	if ((y = (double *)malloc(sizeof(double) * (n + 1))) == NULL)
	{
		free(x);
		return (-1);
	}
	for (i = 0; i < n; i++)
	{
		x[i] = (double)vx[i];
		y[i] = (double)vy[i];
	}
	x[n] = (double)vx[0];
	y[n] = (double)vy[0];

	result = 0;
	if (a != 255)
		result |= SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);

	t = 0.0;
	x1 = (int16_t)lrint(_evaluateBezier(x, n + 1, t));
	y1 = (int16_t)lrint(_evaluateBezier(y, n + 1, t));
	for (i = 0; i <= (n * s); i++)
	{
		t += stepsize;
		x2 = (int16_t)_evaluateBezier(x, n, t);
		y2 = (int16_t)_evaluateBezier(y, n, t);
		result |= line(renderer, x1, y1, x2, y2);
		x1 = x2;
		y1 = y2;
	}

	free(x);
	free(y);

	return (result);
}

int _bresenhamInitialize(SDL2_gfxBresenhamIterator *b, int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
	int temp;

	if (b == NULL)
	{
		return (-1);
	}

	b->x = x1;
	b->y = y1;

	if ((b->dx = x2 - x1) != 0)
	{
		if (b->dx < 0)
		{
			b->dx = -b->dx;
			b->s1 = -1;
		}
		else
		{
			b->s1 = 1;
		}
	}
	else
	{
		b->s1 = 0;
	}

	if ((b->dy = y2 - y1) != 0)
	{
		if (b->dy < 0)
		{
			b->dy = -b->dy;
			b->s2 = -1;
		}
		else
		{
			b->s2 = 1;
		}
	}
	else
	{
		b->s2 = 0;
	}

	if (b->dy > b->dx)
	{
		temp = b->dx;
		b->dx = b->dy;
		b->dy = temp;
		b->swapdir = 1;
	}
	else
	{
		b->swapdir = 0;
	}

	b->count = (b->dx < 0) ? 0 : (unsigned int)b->dx;
	b->dy <<= 1;
	b->error = b->dy - b->dx;
	b->dx <<= 1;

	return (0);
}

int _bresenhamIterate(SDL2_gfxBresenhamIterator *b)
{
	if (b == NULL)
	{
		return (-1);
	}

	if (b->count == 0)
	{
		return (2);
	}

	while (b->error >= 0)
	{
		if (b->swapdir)
		{
			b->x += b->s1;
		}
		else
		{
			b->y += b->s2;
		}

		b->error -= b->dx;
	}

	if (b->swapdir)
	{
		b->y += b->s2;
	}
	else
	{
		b->x += b->s1;
	}

	b->error += b->dy;
	b->count--;

	return ((b->count) ? 0 : 1);
}

static void x_perpendicular(SDL_Renderer *B,
							int x0, int y0, int dx, int dy, int xstep, int ystep,
							int einit, int w_left, int w_right, int winit)
{
	int x, y, threshold, E_diag, E_square;
	int tk;
	int error;
	int p, q;

	threshold = dx - 2 * dy;
	E_diag = -2 * dx;
	E_square = 2 * dy;
	p = q = 0;

	y = y0;
	x = x0;
	error = einit;
	tk = dx + dy - winit;

	while (tk <= w_left)
	{
		SDL_RenderDrawPoint(B, x, y);
		if (error >= threshold)
		{
			x = x + xstep;
			error = error + E_diag;
			tk = tk + 2 * dy;
		}
		error = error + E_square;
		y = y + ystep;
		tk = tk + 2 * dx;
		q++;
	}

	y = y0;
	x = x0;
	error = -einit;
	tk = dx + dy + winit;

	while (tk <= w_right)
	{
		if (p)
			SDL_RenderDrawPoint(B, x, y);
		if (error > threshold)
		{
			x = x - xstep;
			error = error + E_diag;
			tk = tk + 2 * dy;
		}
		error = error + E_square;
		y = y - ystep;
		tk = tk + 2 * dx;
		p++;
	}

	if (q == 0 && p < 2)
		SDL_RenderDrawPoint(B, x0, y0);
}

static void x_varthick_line(SDL_Renderer *B, int style,
							int x0, int y0, int dx, int dy, int xstep, int ystep,
							double thickness, int pxstep, int pystep)
{
	int p_error, error, x, y, threshold, E_diag, E_square, length, p;
	int w_left, w_right;
	double D;

	p_error = 0;
	error = 0;
	y = y0;
	x = x0;
	threshold = dx - 2 * dy;
	E_diag = -2 * dx;
	E_square = 2 * dy;
	length = dx + 1;
	D = sqrt(dx * dx + dy * dy);
	w_left = thickness * D + 0.5;
	w_right = 2.0 * thickness * D + 0.5;
	w_right -= w_left;

	for (p = 0; p < length; p++)
	{
		style = (style << 1) | (style < 0);

		if (style < 0)
			x_perpendicular(B, x, y, dx, dy, pxstep, pystep,
							p_error, w_left, w_right, error);
		if (error >= threshold)
		{
			y = y + ystep;
			error = error + E_diag;
			if (p_error >= threshold)
			{
				if (style < 0)
					x_perpendicular(B, x, y, dx, dy, pxstep, pystep,
									(p_error + E_diag + E_square),
									w_left, w_right, error);
				p_error = p_error + E_diag;
			}
			p_error = p_error + E_square;
		}
		error = error + E_square;
		x = x + xstep;
	}
}

static void y_perpendicular(SDL_Renderer *B,
							int x0, int y0, int dx, int dy, int xstep, int ystep,
							int einit, int w_left, int w_right, int winit)
{
	int x, y, threshold, E_diag, E_square;
	int tk;
	int error;
	int p, q;

	p = q = 0;
	threshold = dy - 2 * dx;
	E_diag = -2 * dy;
	E_square = 2 * dx;

	y = y0;
	x = x0;
	error = -einit;
	tk = dx + dy + winit;

	while (tk <= w_left)
	{
		SDL_RenderDrawPoint(B, x, y);
		if (error > threshold)
		{
			y = y + ystep;
			error = error + E_diag;
			tk = tk + 2 * dx;
		}
		error = error + E_square;
		x = x + xstep;
		tk = tk + 2 * dy;
		q++;
	}

	y = y0;
	x = x0;
	error = einit;
	tk = dx + dy - winit;

	while (tk <= w_right)
	{
		if (p)
			SDL_RenderDrawPoint(B, x, y);
		if (error >= threshold)
		{
			y = y - ystep;
			error = error + E_diag;
			tk = tk + 2 * dx;
		}
		error = error + E_square;
		x = x - xstep;
		tk = tk + 2 * dy;
		p++;
	}

	if (q == 0 && p < 2)
		SDL_RenderDrawPoint(B, x0, y0);
}

static void y_varthick_line(SDL_Renderer *B, int style,
							int x0, int y0, int dx, int dy, int xstep, int ystep,
							double thickness, int pxstep, int pystep)
{
	int p_error, error, x, y, threshold, E_diag, E_square, length, p;
	int w_left, w_right;
	double D;

	p_error = 0;
	error = 0;
	y = y0;
	x = x0;
	threshold = dy - 2 * dx;
	E_diag = -2 * dy;
	E_square = 2 * dx;
	length = dy + 1;
	D = sqrt(dx * dx + dy * dy);
	w_left = thickness * D + 0.5;
	w_right = 2.0 * thickness * D + 0.5;
	w_right -= w_left;

	for (p = 0; p < length; p++)
	{
		style = (style << 1) | (style < 0);

		if (style < 0)
			y_perpendicular(B, x, y, dx, dy, pxstep, pystep,
							p_error, w_left, w_right, error);
		if (error >= threshold)
		{
			x = x + xstep;
			error = error + E_diag;
			if (p_error >= threshold)
			{
				if (style < 0)
					y_perpendicular(B, x, y, dx, dy, pxstep, pystep,
									p_error + E_diag + E_square,
									w_left, w_right, error);
				p_error = p_error + E_diag;
			}
			p_error = p_error + E_square;
		}
		error = error + E_square;
		y = y + ystep;
	}
}

void draw_varthick_line(SDL_Renderer *B, int style,
						int x0, int y0, int x1, int y1, double thickness)
{
	int dx, dy, xstep, ystep;
	int pxstep = 0, pystep = 0;

	dx = x1 - x0;
	dy = y1 - y0;
	xstep = ystep = 1;

	if (dx < 0)
	{
		dx = -dx;
		xstep = -1;
	}
	if (dy < 0)
	{
		dy = -dy;
		ystep = -1;
	}

	if (dx == 0)
		xstep = 0;
	if (dy == 0)
		ystep = 0;

	switch (xstep + ystep * 4)
	{
	case -1 + -1 * 4:
		pystep = -1;
		pxstep = 1;
		break;
	case -1 + 0 * 4:
		pystep = -1;
		pxstep = 0;
		break;
	case -1 + 1 * 4:
		pystep = 1;
		pxstep = 1;
		break;
	case 0 + -1 * 4:
		pystep = 0;
		pxstep = -1;
		break;
	case 0 + 0 * 4:
		pystep = 0;
		pxstep = 0;
		break;
	case 0 + 1 * 4:
		pystep = 0;
		pxstep = 1;
		break;
	case 1 + -1 * 4:
		pystep = -1;
		pxstep = -1;
		break;
	case 1 + 0 * 4:
		pystep = -1;
		pxstep = 0;
		break;
	case 1 + 1 * 4:
		pystep = 1;
		pxstep = -1;
		break;
	}

	if (dx > dy)
		x_varthick_line(B, style, x0, y0, dx, dy, xstep, ystep,
						thickness + 1.0,
						pxstep, pystep);
	else
		y_varthick_line(B, style, x0, y0, dx, dy, xstep, ystep,
						thickness + 1.0,
						pxstep, pystep);
	return;
}

static int LineStyle = -1;

int thickLineColor(SDL_Renderer *renderer, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t width, uint32_t color)
{
	uint8_t *c = (uint8_t *)&color;
	LineStyle = -1;
	return thickLineRGBA(renderer, x1, y1, x2, y2, width, c[0], c[1], c[2], c[3]);
}

int thickLineColorStyle(SDL_Renderer *renderer, int16_t x1, int16_t y1, int16_t x2, int16_t y2,
						uint8_t width, uint32_t color, int style)
{
	uint8_t *c = (uint8_t *)&color;
	LineStyle = style;
	return thickLineRGBA(renderer, x1, y1, x2, y2, width, c[0], c[1], c[2], c[3]);
}

int thickLineRGBA(SDL_Renderer *renderer, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t width, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	int result;
	int wh;

	if (renderer == NULL)
	{
		return -1;
	}
	if (width < 1)
	{
		return -1;
	}

	if ((x1 == x2) && (y1 == y2))
	{
		wh = width / 2;
		return boxRGBA(renderer, x1 - wh, y1 - wh, x2 + wh, y2 + wh, r, g, b, a);
	}

	result = 0;
	if (a != 255)
		result |= SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);

	draw_varthick_line(renderer, LineStyle, x1, y1, x2, y2, (double)width);
	return (result);
}

int RedefineChar(SDL_Renderer *renderer, char c, unsigned char *charpos, uint32_t width, uint32_t height)
{
	uint32_t ix, iy;
	uint8_t *curpos;
	uint8_t patt, mask;
	uint8_t *linepos;
	uint32_t pitch;
	SDL_Surface *character;
	SDL_Surface *zoomedCharacter;
	uint32_t ci;

	ci = (unsigned char)c;

	if (gfxPrimitivesFont[ci] != NULL)
		SDL_DestroyTexture(gfxPrimitivesFont[ci]);

	character = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32,
									 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
	if (character == NULL)
		return (-1);

	linepos = (uint8_t *)character->pixels;
	pitch = character->pitch;

	patt = 0;
	for (iy = 0; iy < height; iy++)
	{
		mask = 0x00;
		curpos = linepos;
		for (ix = 0; ix < width; ix++)
		{
			if (!(mask >>= 1))
			{
				patt = *charpos++;
				mask = 0x80;
			}
			if (patt & mask)
			{
				*(uint32_t *)curpos = 0xffffffff;
			}
			else
			{
				*(uint32_t *)curpos = 0;
			}
			curpos += 4;
		}
		linepos += pitch;
	}

	if ((charZoomX != 1) || (charZoomY != 1))
	{
		zoomedCharacter = zoomSurface(character, (double)charZoomX,
									  (double)charZoomY, SMOOTHING_OFF);
		SDL_FreeSurface(character);
		character = zoomedCharacter;
	}

	gfxPrimitivesFont[ci] = SDL_CreateTextureFromSurface(renderer, character);
	SDL_FreeSurface(character);

	if (gfxPrimitivesFont[ci] == NULL)
	{
		return (-1);
	}
	return 0;
}

static int renderdrawline(SDL_Renderer *renderer, int x1, int y1, int x2, int y2)
{
	int result;
#ifndef __EMSCRIPTEN__
	if ((x1 == x2) && (y1 == y2))
		result = SDL_RenderDrawPoint(renderer, x1, y1);
	else if (y1 == y2)
	{
		int x;
		if (x1 > x2)
		{
			x = x1;
			x1 = x2;
			x2 = x;
		}
		SDL_Point *points = (SDL_Point *)malloc((x2 - x1 + 1) * sizeof(SDL_Point));
		if (points == NULL)
			return -1;
		for (x = x1; x <= x2; x++)
		{
			points[x - x1].x = x;
			points[x - x1].y = y1;
		}
		result = SDL_RenderDrawPoints(renderer, points, x2 - x1 + 1);
		free(points);
	}
	else if (x1 == x2)
	{
		int y;
		if (y1 > y2)
		{
			y = y1;
			y1 = y2;
			y2 = y;
		}
		SDL_Point *points = (SDL_Point *)malloc((y2 - y1 + 1) * sizeof(SDL_Point));
		if (points == NULL)
			return -1;
		for (y = y1; y <= y2; y++)
		{
			points[y - y1].x = x1;
			points[y - y1].y = y;
		}
		result = SDL_RenderDrawPoints(renderer, points, y2 - y1 + 1);
		free(points);
	}
	else
#endif
		result = SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
	return result;
}

static int hlinecliparc(SDL_Renderer *renderer, int x1, int x2, int y, int xc, int yc, double s, double f)
{
	int result = 0;
	double a1, a2;
	a1 = atan2(y, x1);
	a2 = atan2(y, x2);
	if (a1 > a2)
	{
		double a = a1;
		a1 = a2;
		a2 = a;
		int x = x1;
		x1 = x2;
		x2 = x;
	}
	if (f < s)
	{
		if ((a1 > f) && (a2 < s))
			return result;
		if ((a1 < s) && (a1 > f))
			x1 = y / tan(s);
		if ((a2 > f) && (a2 < s))
			x2 = y / tan(f);
		if ((a1 < f) && (a2 > s))
		{
			result |= renderdrawline(renderer, x1 + xc, y + yc, y / tan(f) + xc, y + yc);
			result |= renderdrawline(renderer, y / tan(s) + xc, y + yc, x2 + xc, y + yc);
			return result;
		}
	}
	else
	{
		if ((a1 > f) || (a2 < s))
			return result;
		if (a1 < s)
			x1 = y / tan(s);
		if (a2 > f)
			x2 = y / tan(f);
	}
	result |= renderdrawline(renderer, x1 + xc, y + yc, x2 + xc, y + yc);
	return result;
}

int thickEllipseRGBA(SDL_Renderer *renderer, int16_t xc, int16_t yc, int16_t xr, int16_t yr, uint8_t r, uint8_t g, uint8_t b, uint8_t a, uint8_t thick)
{
	int result = 0;
	int xi, yi, xo, yo, x, y, z;
	double xi2, yi2, xo2, yo2;

	if (thick <= 1)
		return ellipseRGBA(renderer, xc, yc, xr, yr, r, g, b, a);

	xi = xr - thick / 2;
	xo = xi + thick - 1;
	yi = yr - thick / 2;
	yo = yi + thick - 1;

	if ((xi <= 0) || (yi <= 0))
		return -1;

	xi2 = xi * xi;
	yi2 = yi * yi;
	xo2 = xo * xo;
	yo2 = yo * yo;

	if (a != 255)
		result |= SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);

	if (xr < yr)
	{
		for (x = -xo; x <= -xi; x++)
		{
			y = sqrt(yo2 * (1.0 - x * x / xo2)) + 0.5;
			result |= renderdrawline(renderer, xc + x, yc - y, xc + x, yc + y);
		}
		for (x = -xi + 1; x <= xi - 1; x++)
		{
			y = sqrt(yo2 * (1.0 - x * x / xo2)) + 0.5;
			z = sqrt(yi2 * (1.0 - x * x / xi2)) + 0.5;
			result |= renderdrawline(renderer, xc + x, yc + z, xc + x, yc + y);
			result |= renderdrawline(renderer, xc + x, yc - z, xc + x, yc - y);
		}
		for (x = xo; x >= xi; x--)
		{
			y = sqrt(yo2 * (1.0 - x * x / xo2)) + 0.5;
			result |= renderdrawline(renderer, xc + x, yc - y, xc + x, yc + y);
		}
	}
	else
	{
		for (y = -yo; y <= -yi; y++)
		{
			x = sqrt(xo2 * (1.0 - y * y / yo2)) + 0.5;
			result |= renderdrawline(renderer, xc - x, yc + y, xc + x, yc + y);
		}
		for (y = -yi + 1; y <= yi - 1; y++)
		{
			x = sqrt(xo2 * (1.0 - y * y / yo2)) + 0.5;
			z = sqrt(xi2 * (1.0 - y * y / yi2)) + 0.5;
			result |= renderdrawline(renderer, xc + z, yc + y, xc + x, yc + y);
			result |= renderdrawline(renderer, xc - z, yc + y, xc - x, yc + y);
		}
		for (y = yo; y >= yi; y--)
		{
			x = sqrt(xo2 * (1.0 - y * y / yo2)) + 0.5;
			result |= renderdrawline(renderer, xc - x, yc + y, xc + x, yc + y);
		}
	}
	return result;
}

int thickArcRGBA(SDL_Renderer *renderer, int16_t xc, int16_t yc, int16_t rad, int16_t start, int16_t end, uint8_t r, uint8_t g, uint8_t b, uint8_t a, uint8_t thick)
{
	int result = 0;
	int ri, ro, x, y, z;
	double ri2, ro2, s, f;

	if (thick <= 1)
		return arcRGBA(renderer, xc, yc, rad, start, end, r, g, b, a);

	while (start < -180)
		start += 360;
	while (start >= 180)
		start -= 360;
	while (end < -180)
		end += 360;
	while (end >= 180)
		end -= 360;
	s = M_PI * (double)start / 180.0;
	f = M_PI * (double)end / 180.0;
	if (start == end)
		return 0;

	ri = rad - thick / 2;
	ro = ri + thick - 1;
	if (ri <= 0)
		return -1;

	ri2 = ri * ri;
	ro2 = ro * ro;

	if (a != 255)
		result |= SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);

	for (y = -ro; y <= -ri; y++)
	{
		x = sqrt(ro2 * (1.0 - y * y / ro2)) + 0.5;
		result |= hlinecliparc(renderer, -x, x, y, xc, yc, s, f);
	}
	for (y = -ri + 1; y <= ri - 1; y++)
	{
		x = sqrt(ro2 * (1.0 - y * y / ro2)) + 0.5;
		z = sqrt(ri2 * (1.0 - y * y / ri2)) + 0.5;
		result |= hlinecliparc(renderer, z, x, y, xc, yc, s, f);
		result |= hlinecliparc(renderer, -z, -x, y, xc, yc, s, f);
	}
	for (y = ro; y >= ri; y--)
	{
		x = sqrt(ro2 * (1.0 - y * y / ro2)) + 0.5;
		result |= hlinecliparc(renderer, -x, x, y, xc, yc, s, f);
	}
	return result;
}

int thickCircleRGBA(SDL_Renderer *renderer, int16_t x, int16_t y, int16_t rad, uint8_t r, uint8_t g, uint8_t b, uint8_t a, uint8_t thick)
{
	return thickEllipseRGBA(renderer, x, y, rad, rad, r, g, b, a, thick);
}

int thickEllipseColor(SDL_Renderer *renderer, int16_t x, int16_t y, int16_t rx, int16_t ry, uint32_t color, uint8_t thick)
{
	uint8_t *c = (uint8_t *)&color;
	return thickEllipseRGBA(renderer, x, y, rx, ry, c[0], c[1], c[2], c[3], thick);
}

int thickArcColor(SDL_Renderer *renderer, int16_t x, int16_t y, int16_t rad, int16_t start, int16_t end, uint32_t color, uint8_t thick)
{
	uint8_t *c = (uint8_t *)&color;
	return thickArcRGBA(renderer, x, y, rad, start, end, c[0], c[1], c[2], c[3], thick);
}

int thickCircleColor(SDL_Renderer *renderer, int16_t x, int16_t y, int16_t rad, uint32_t color, uint8_t thick)
{
	uint8_t *c = (uint8_t *)&color;
	return thickEllipseRGBA(renderer, x, y, rad, rad, c[0], c[1], c[2], c[3], thick);
}

int filledPolyBezierRGBA(SDL_Renderer *renderer, const int16_t *x, const int16_t *y, int n, int s, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	int i, j, nbeziers, nverts, result;
	double t, stepsize;
	double x1, y1, x2, y2;
	double *dx, *dy;
	int16_t *vx, *vy;

	if ((n < 7) || (s < 2))
		return -1;

	if ((dx = (double *)malloc(sizeof(double) * n)) == NULL)
	{
		return (-1);
	}
	if ((dy = (double *)malloc(sizeof(double) * n)) == NULL)
	{
		free(dx);
		return (-1);
	}
	for (i = 0; i < n; i++)
	{
		dx[i] = (double)x[i];
		dy[i] = (double)y[i];
	}

	nbeziers = (n - 1) / 3;
	nverts = nbeziers * 4 * s + 1;
	vx = (int16_t *)malloc(nverts * 2 * sizeof(int16_t));
	if (vx == NULL)
	{
		free(dy);
		free(dx);
		return -1;
	}
	vy = vx + nverts;

	stepsize = 1.0 / (double)s;
	for (j = 0; j < nbeziers; j++)
	{
		t = 0.0;
		x1 = _evaluateBezier(dx + j * 3, 4, t);
		y1 = _evaluateBezier(dy + j * 3, 4, t);
		for (i = 0; i < 4 * s; i++)
		{
			t += stepsize;
			x2 = _evaluateBezier(dx + j * 3, 4, t);
			y2 = _evaluateBezier(dy + j * 3, 4, t);

			vx[i + j * s * 4] = floor(x1 + 0.5);
			vy[i + j * s * 4] = floor(y1 + 0.5);

			x1 = x2;
			y1 = y2;
		}
	}

	vx[j * s * 4] = floor(x1 + 0.5);
	vy[j * s * 4] = floor(y1 + 0.5);

	free(dy);
	free(dx);

	result = filledPolygonRGBA(renderer, vx, vy, nverts, r, g, b, a);

	free(vx);
	return (result);
}

int filledPolyBezierColor(SDL_Renderer *renderer, const int16_t *x, const int16_t *y, int n, int s, uint32_t color)
{
	uint8_t *c = (uint8_t *)&color;
	return filledPolyBezierRGBA(renderer, x, y, n, s, c[0], c[1], c[2], c[3]);
}

int aaFilledEllipseRGBA(SDL_Renderer *renderer, float cx, float cy, float rx, float ry, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	int n, xi, yi, result = 0;
	double s, v, x, y, dx, dy;

	if ((rx <= 0.0) || (ry <= 0.0))
		return -1;

	result |= SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	if (rx >= ry)
	{
		n = ry + 1;
		for (yi = cy - n - 1; yi <= cy + n + 1; yi++)
		{
			if (yi < (cy - 0.5))
				y = yi;
			else
				y = yi + 1;
			s = (y - cy) / ry;
			s = s * s;
			x = 0.5;
			if (s < 1.0)
			{
				x = rx * sqrt(1.0 - s);
				if (x >= 0.5)
				{
					result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);
					result |= renderdrawline(renderer, cx - x + 1, yi, cx + x - 1, yi);
				}
			}
			s = 8 * ry * ry;
			dy = fabs(y - cy) - 1.0;
			xi = cx - x;
			while (1)
			{
				dx = (cx - xi - 1) * ry / rx;
				v = s - 4 * (dx - dy) * (dx - dy);
				if (v < 0)
					break;
				v = (sqrt(v) - 2 * (dx + dy)) / 4;
				if (v < 0)
					break;
				if (v > 1.0)
					v = 1.0;
				result |= SDL_SetRenderDrawColor(renderer, r, g, b, (double)a * v);
				result |= SDL_RenderDrawPoint(renderer, xi, yi);
				xi -= 1;
			}
			xi = cx + x;
			while (1)
			{
				dx = (xi - cx) * ry / rx;
				v = s - 4 * (dx - dy) * (dx - dy);
				if (v < 0)
					break;
				v = (sqrt(v) - 2 * (dx + dy)) / 4;
				if (v < 0)
					break;
				if (v > 1.0)
					v = 1.0;
				result |= SDL_SetRenderDrawColor(renderer, r, g, b, (double)a * v);
				result |= SDL_RenderDrawPoint(renderer, xi, yi);
				xi += 1;
			}
		}
	}
	else
	{
		n = rx + 1;
		for (xi = cx - n - 1; xi <= cx + n + 1; xi++)
		{
			if (xi < (cx - 0.5))
				x = xi;
			else
				x = xi + 1;
			s = (x - cx) / rx;
			s = s * s;
			y = 0.5;
			if (s < 1.0)
			{
				y = ry * sqrt(1.0 - s);
				if (y >= 0.5)
				{
					result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);
					result |= renderdrawline(renderer, xi, cy - y + 1, xi, cy + y - 1);
				}
			}
			s = 8 * rx * rx;
			dx = fabs(x - cx) - 1.0;
			yi = cy - y;
			while (1)
			{
				dy = (cy - yi - 1) * rx / ry;
				v = s - 4 * (dy - dx) * (dy - dx);
				if (v < 0)
					break;
				v = (sqrt(v) - 2 * (dy + dx)) / 4;
				if (v < 0)
					break;
				if (v > 1.0)
					v = 1.0;
				result |= SDL_SetRenderDrawColor(renderer, r, g, b, (double)a * v);
				result |= SDL_RenderDrawPoint(renderer, xi, yi);
				yi -= 1;
			}
			yi = cy + y;
			while (1)
			{
				dy = (yi - cy) * rx / ry;
				v = s - 4 * (dy - dx) * (dy - dx);
				if (v < 0)
					break;
				v = (sqrt(v) - 2 * (dy + dx)) / 4;
				if (v < 0)
					break;
				if (v > 1.0)
					v = 1.0;
				result |= SDL_SetRenderDrawColor(renderer, r, g, b, (double)a * v);
				result |= SDL_RenderDrawPoint(renderer, xi, yi);
				yi += 1;
			}
		}
	}
	return result;
}

int aaFilledEllipseColor(SDL_Renderer *renderer, float cx, float cy, float rx, float ry, uint32_t color)
{
	uint8_t *c = (uint8_t *)&color;
	return aaFilledEllipseRGBA(renderer, cx, cy, rx, ry, c[0], c[1], c[2], c[3]);
}

static int _gfxPrimitivesCompareFloat2(const void *a, const void *b)
{
	float diff = *((float *)a + 1) - *((float *)b + 1);
	if (diff != 0.0)
		return (diff > 0) - (diff < 0);
	diff = *(float *)a - *(float *)b;
	return (diff > 0) - (diff < 0);
}

#define POLYSIZE 16384

int aaFilledPolygonRGBA(SDL_Renderer *renderer, const double *vx, const double *vy, int n, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	int i, j, xi, yi, result;
	double x1, x2, y0, y1, y2, minx, maxx, prec;
	float *list, *strip;

	if (n < 3)
		return -1;

	result = SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	minx = 99999.0;
	maxx = -99999.0;
	prec = 0.00001;
	for (i = 0; i < n; i++)
	{
		double x = vx[i];
		double y = fabs(vy[i]);
		if (x < minx)
			minx = x;
		if (x > maxx)
			maxx = x;
		if (y > prec)
			prec = y;
	}
	minx = floor(minx);
	maxx = floor(maxx);
	prec = floor(pow(2, 19) / prec);

	list = (float *)malloc(POLYSIZE * sizeof(float));
	if (list == NULL)
		return -2;

	yi = 0;
	y0 = floor(vy[n - 1] * prec) / prec;
	y1 = floor(vy[0] * prec) / prec;
	for (i = 1; i <= n; i++)
	{
		if (yi > POLYSIZE - 4)
		{
			free(list);
			return -2;
		}
		y2 = floor(vy[i % n] * prec) / prec;
		if (((y1 < y2) - (y1 > y2)) == ((y0 < y1) - (y0 > y1)))
		{
			list[yi++] = -100002.0;
			list[yi++] = y1;
			list[yi++] = -100002.0;
			list[yi++] = y1;
		}
		else
		{
			if (y0 != y1)
			{
				list[yi++] = (y1 < y0) - (y1 > y0) - 100002.0;
				list[yi++] = y1;
			}
			if (y1 != y2)
			{
				list[yi++] = (y1 < y2) - (y1 > y2) - 100002.0;
				list[yi++] = y1;
			}
		}
		y0 = y1;
		y1 = y2;
	}
	xi = yi;

	qsort(list, yi / 2, sizeof(float) * 2, _gfxPrimitivesCompareFloat2);

	for (i = 1; i <= n; i++)
	{
		double x, y;
		double d = 0.5 / prec;

		x1 = vx[i - 1];
		y1 = floor(vy[i - 1] * prec) / prec;
		x2 = vx[i % n];
		y2 = floor(vy[i % n] * prec) / prec;

		if (y2 < y1)
		{
			double tmp;
			tmp = x1;
			x1 = x2;
			x2 = tmp;
			tmp = y1;
			y1 = y2;
			y2 = tmp;
		}
		if (y2 != y1)
			y0 = (x2 - x1) / (y2 - y1);

		for (j = 1; j < xi; j += 4)
		{
			y = list[j];
			if (((y + d) <= y1) || (y == list[j + 4]))
				continue;
			if ((y -= d) >= y2)
				break;
			if (yi > POLYSIZE - 4)
			{
				free(list);
				return -2;
			}
			if (y > y1)
			{
				list[yi++] = x1 + y0 * (y - y1);
				list[yi++] = y;
			}
			y += d * 2.0;
			if (y < y2)
			{
				list[yi++] = x1 + y0 * (y - y1);
				list[yi++] = y;
			}
		}

		y = floor(y1) + 1.0;
		while (y <= y2)
		{
			x = x1 + y0 * (y - y1);
			if (yi > POLYSIZE - 2)
			{
				free(list);
				return -2;
			}
			list[yi++] = x;
			list[yi++] = y;
			y += 1.0;
		}
	}

	qsort(list, yi / 2, sizeof(float) * 2, _gfxPrimitivesCompareFloat2);

	strip = (float *)malloc((maxx - minx + 2) * sizeof(float));
	if (strip == NULL)
	{
		free(list);
		return -1;
	}
	memset(strip, 0, (maxx - minx + 2) * sizeof(float));
	n = yi;
	yi = list[1];
	j = 0;

	for (i = 0; i < n - 7; i += 4)
	{
		float x1 = list[i + 0];
		float y1 = list[i + 1];
		float x3 = list[i + 2];
		float x2 = list[i + j + 0];
		float y2 = list[i + j + 1];
		float x4 = list[i + j + 2];

		if (x1 + x3 == -200002.0)
			j += 4;
		else if (x1 + x3 == -200006.0)
			j -= 4;
		else if ((x1 >= minx) && (x2 >= minx))
		{
			if (x1 > x2)
			{
				float tmp = x1;
				x1 = x2;
				x2 = tmp;
			}
			if (x3 > x4)
			{
				float tmp = x3;
				x3 = x4;
				x4 = tmp;
			}

			for (xi = x1 - minx; xi <= x4 - minx; xi++)
			{
				float u, v;
				float x = minx + xi;
				if (x < x2)
					u = (x - x1 + 1) / (x2 - x1 + 1);
				else
					u = 1.0;
				if (x >= x3 - 1)
					v = (x4 - x) / (x4 - x3 + 1);
				else
					v = 1.0;
				if ((u > 0.0) && (v > 0.0))
					strip[xi] += (y2 - y1) * (u + v - 1.0);
			}
		}

		if ((yi == (list[i + 5] - 1.0)) || (i == n - 8))
		{
			for (xi = 0; xi <= maxx - minx; xi++)
			{
				if (strip[xi] != 0.0)
				{
					if (strip[xi] >= 0.996)
					{
						int x0 = xi;
						while (strip[++xi] >= 0.996)
							;
						xi--;
						result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);
						result |= renderdrawline(renderer, minx + x0, yi, minx + xi, yi);
					}
					else
					{
						result |= SDL_SetRenderDrawColor(renderer, r, g, b, a * strip[xi]);
						result |= SDL_RenderDrawPoint(renderer, minx + xi, yi);
					}
				}
			}
			memset(strip, 0, (maxx - minx + 2) * sizeof(float));
			yi++;
		}
	}

	free(list);
	free(strip);
	return result;
}

int aaFilledPolygonColor(SDL_Renderer *renderer, const double *vx, const double *vy, int n, uint32_t color)
{
	uint8_t *c = (uint8_t *)&color;
	return aaFilledPolygonRGBA(renderer, vx, vy, n, c[0], c[1], c[2], c[3]);
}

int aaFilledPieRGBA(SDL_Renderer *renderer, float cx, float cy, float rx, float ry,
					float start, float end, uint32_t chord, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	int nverts, i, result;
	double *vx, *vy;

	if ((rx <= 0) || (ry <= 0) || (start == end))
		return -1;

	start = fmod(start, 360.0) * 2.0 * M_PI / 360.0;
	end = fmod(end, 360.0) * 2.0 * M_PI / 360.0;
	while (start >= end)
		end += 2.0 * M_PI;

	nverts = (end - start) * sqrt(rx * ry) / M_PI;
	if (nverts < 2)
		nverts = 2;
	if (nverts > 180)
		nverts = 180;

	vx = vy = (double *)malloc(2 * sizeof(double) * (nverts + 1));
	if (vx == NULL)
		return (-1);

	vy += nverts + 1;

	for (i = 0; i < nverts; i++)
	{
		double angle = start + (end - start) * (double)i / (double)(nverts - 1);
		vx[i] = cx + rx * cos(angle);
		vy[i] = cy + ry * sin(angle);
	}

	vx[i] = cx;
	vy[i] = cy;

	result = aaFilledPolygonRGBA(renderer, vx, vy, nverts + 1 - (chord != 0), r, g, b, a);

	free(vx);

	return (result);
}

int aaFilledPieColor(SDL_Renderer *renderer, float cx, float cy, float rx, float ry, float start, float end, uint32_t chord, uint32_t color)
{
	uint8_t *c = (uint8_t *)&color;
	return aaFilledPieRGBA(renderer, cx, cy, rx, ry, start, end, chord, c[0], c[1], c[2], c[3]);
}

int aaArcRGBA(SDL_Renderer *renderer, float cx, float cy, float rx, float ry,
			  float start, float end, float thick, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	int nverts, i, result;
	double *vx, *vy;

	if ((rx <= 0) || (ry <= 0) || (start == end) || (thick <= 0))
		return -1;

	start = fmod(start, 360.0) * 2.0 * M_PI / 360.0;
	end = fmod(end, 360.0) * 2.0 * M_PI / 360.0;
	while (start >= end)
		end += 2.0 * M_PI;

	nverts = 2 * floor((end - start) * sqrt(rx * ry) / M_PI);
	if (nverts < 2)
		nverts = 2;
	if (nverts > 360)
		nverts = 360;

	vx = vy = (double *)malloc(2 * sizeof(double) * nverts);
	if (vx == NULL)
		return (-1);

	vy += nverts;

	for (i = 0; i < nverts / 2; i++)
	{
		double angle = start + (end - start) * (double)i / (double)(nverts / 2 - 1);
		vx[i] = cx + (rx + thick / 2) * cos(angle);
		vy[i] = cy + (ry + thick / 2) * sin(angle);
		vx[nverts - 1 - i] = cx + (rx - thick / 2) * cos(angle);
		vy[nverts - 1 - i] = cy + (ry - thick / 2) * sin(angle);
	}

	result = aaFilledPolygonRGBA(renderer, vx, vy, nverts, r, g, b, a);

	free(vx);

	return (result);
}

int aaArcColor(SDL_Renderer *renderer, float cx, float cy, float rx, float ry, float start, float end, float thick, uint32_t color)
{
	uint8_t *c = (uint8_t *)&color;
	return aaArcRGBA(renderer, cx, cy, rx, ry, start, end, thick, c[0], c[1], c[2], c[3]);
}

int aaBezierRGBA(SDL_Renderer *renderer, double *x, double *y, int n, int s, float thick, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	int i, nverts, result;
	double d, t, stepsize;
	double x1, y1, x2, y2, dx = 0.0, dy = 0.0;
	double *vx, *vy;

	if ((n < 3) || (s < 2))
		return -1;

	nverts = n * s * 2 + 2;
	vx = (double *)malloc(nverts * 2 * sizeof(double));
	if (vx == NULL)
		return -1;
	vy = vx + nverts;

	t = 0.0;
	stepsize = 1.0 / (double)s;
	x1 = _evaluateBezier(x, n, t);
	y1 = _evaluateBezier(y, n, t);
	for (i = 0; i < n * s; i++)
	{
		t += stepsize;
		x2 = _evaluateBezier(x, n, t);
		y2 = _evaluateBezier(y, n, t);

		dx = x2 - x1;
		dy = y2 - y1;
		d = thick * 0.5L / sqrt(dx * dx + dy * dy);
		dx *= d;
		dy *= d;

		vx[i] = x1 + dy;
		vy[i] = y1 - dx;
		vx[nverts - 1 - i] = x1 - dy;
		vy[nverts - 1 - i] = y1 + dx;

		x1 = x2;
		y1 = y2;
	}

	vx[i] = x1 + dy;
	vy[i] = y1 - dx;
	vx[nverts - 1 - i] = x1 - dy;
	vy[nverts - 1 - i] = y1 + dx;

	result = aaFilledPolygonRGBA(renderer, vx, vy, nverts, r, g, b, a);

	free(vx);
	return (result);
}

int aaBezierColor(SDL_Renderer *renderer, double *x, double *y, int n, int s, float thick, uint32_t color)
{
	uint8_t *c = (uint8_t *)&color;
	return aaBezierRGBA(renderer, x, y, n, s, thick, c[0], c[1], c[2], c[3]);
}

int aaFilledPolyBezierRGBA(SDL_Renderer *renderer, double *x, double *y, int n, int s, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	int i, j, nbeziers, nverts, result;
	double t, stepsize;
	double x1, y1, x2, y2;
	double *vx, *vy;

	if ((n < 7) || (s < 2))
		return -1;

	nbeziers = (n - 1) / 3;
	nverts = nbeziers * 4 * s + 1;
	vx = (double *)malloc(nverts * 2 * sizeof(double));
	if (vx == NULL)
		return -1;
	vy = vx + nverts;

	stepsize = 1.0 / (double)s;
	for (j = 0; j < nbeziers; j++)
	{
		t = 0.0;
		x1 = _evaluateBezier(x + j * 3, 4, t);
		y1 = _evaluateBezier(y + j * 3, 4, t);
		for (i = 0; i < 4 * s; i++)
		{
			t += stepsize;
			x2 = _evaluateBezier(x + j * 3, 4, t);
			y2 = _evaluateBezier(y + j * 3, 4, t);

			vx[i + j * s * 4] = x1;
			vy[i + j * s * 4] = y1;

			x1 = x2;
			y1 = y2;
		}
	}

	vx[j * s * 4] = x1;
	vy[j * s * 4] = y1;

	result = aaFilledPolygonRGBA(renderer, vx, vy, nverts, r, g, b, a);

	free(vx);
	return (result);
}

int aaFilledPolyBezierColor(SDL_Renderer *renderer, double *x, double *y, int n, int s, uint32_t color)
{
	uint8_t *c = (uint8_t *)&color;
	return aaFilledPolyBezierRGBA(renderer, x, y, n, s, c[0], c[1], c[2], c[3]);
}
