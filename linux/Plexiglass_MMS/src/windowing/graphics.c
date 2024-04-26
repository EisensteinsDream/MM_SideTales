#include "windowing/graphics.h"

typedef struct
{
	int64_t x, y, w, h;
	uint8_t r, g, b, a, thickness;
} PLEX_GRAPHICS_INSTRUCT_RECT;

typedef struct
{
	int64_t x, y, x2, y2;
	uint8_t r, g, b, a, thickness;
} PLEX_GRAPHICS_INSTRUCT_LINE;

typedef struct
{
	int64_t x, y;
	int64_t length;
	double direction;
	uint8_t r, g, b, a, thickness;
} PLEX_GRAPHICS_INSTRUCT_LINE2;

typedef struct
{
	int64_t x, y, w, h;
	uint8_t r, g, b, a;
} PLEX_GRAPHICS_INSTRUCT_FILL_RECT;

typedef struct
{
	size_t texture;
	int64_t fx, fy, fw, fh, tx, ty, tw, th;
	uint8_t r, g, b;
	SDL_BlendMode textureBlendMode;
} PLEX_GRAPHICS_INSTRUCT_TEXTURE;

typedef struct
{
	size_t texture;
	int64_t fx, fy, fw, fh, tx, ty, tw, th;
	uint8_t r, g, b;
	double angle;
	SDL_BlendMode textureBlendMode;
} PLEX_GRAPHICS_INSTRUCT_TEXTURE_ROTATED;

typedef struct
{
	double wPerc, hPerc;
	int64_t ox, oy, ow, oh;
	uint8_t symbol, r, g, b;
	const char* alphaString;
	size_t texture;
	bool offset;
	SDL_BlendMode textureBlendMode;
} PLEX_GRAPHICS_INSTRUCT_FONT_LETTER;

typedef struct
{
	uint8_t r, g, b, a;
} PLEX_GRAPHICS_INSTRUCT_CLEAR;

typedef union
{
	PLEX_GRAPHICS_INSTRUCT_RECT rect;
	PLEX_GRAPHICS_INSTRUCT_LINE line;
	PLEX_GRAPHICS_INSTRUCT_LINE2 line2;
	PLEX_GRAPHICS_INSTRUCT_FILL_RECT fillRect;
	PLEX_GRAPHICS_INSTRUCT_TEXTURE texture;
	PLEX_GRAPHICS_INSTRUCT_TEXTURE_ROTATED textureRotated;
	PLEX_GRAPHICS_INSTRUCT_FONT_LETTER letter;
	PLEX_GRAPHICS_INSTRUCT_CLEAR clear;
} PLEX_GRAPHICS_INSTRUCT_CONTENTS;

typedef enum
{
	PLEX_GRAPHICS_INSTRUCT_TYPE_RECT = 0,
	PLEX_GRAPHICS_INSTRUCT_TYPE_LINE = 1,
	PLEX_GRAPHICS_INSTRUCT_TYPE_LINE2 = 2,
	PLEX_GRAPHICS_INSTRUCT_TYPE_FILL_RECT = 3,
	PLEX_GRAPHICS_INSTRUCT_TYPE_TEXTURE = 4,
	PLEX_GRAPHICS_INSTRUCT_TYPE_TEXTURE_ROTATE = 5,
	PLEX_GRAPHICS_INSTRUCT_TYPE_FONT_LETTER = 6,
	PLEX_GRAPHICS_INSTRUCT_TYPE_CLEAR = 7
} PLEX_GRAPHICS_INSTRUCT_TYPE;

typedef struct
{
	size_t order;
	bool offset;
	SDL_BlendMode drawBlendMode;
	PLEX_LOW_TEXTURE target;
	PLEX_GRAPHICS_INSTRUCT_TYPE type;
	PLEX_GRAPHICS_INSTRUCT_CONTENTS contents;
} PLEX_GRAPHICS_INSTRUCT;

static PLEX_EXPANDARRAY* p_drawList = NULL;

static size_t p_screenPlate = 0;

static bool p_showClickables = false;
static bool p_showTooltips = false;
static bool p_toggleTooltips = false;

static uint8_t p_refreshR = 0;
static uint8_t p_refreshG = 0;
static uint8_t p_refreshB = 0;

static size_t p_regularCursor = 0;
static size_t p_regularCursorClick = 0;
static size_t p_regularCursorDown = 0;

static void p_drawRect(const int64_t x, const int64_t y, const int64_t w, const int64_t h, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const uint8_t thickness, const bool offset, const SDL_BlendMode blendMode)
{
	const int64_t aX = x + (offset * PLEX_WOFFSET());
	const int64_t aY = y + (offset * PLEX_HOFFSET());

	SDL_Rect s[4];

	s[0].x = aX;
	s[0].y = aY;
	s[0].w = w;
	s[0].h = thickness;

	s[1].x = aX + w - thickness;
	s[1].y = aY;
	s[1].w = thickness;
	s[1].h = h;

	s[2].x = aX;
	s[2].y = aY + h - thickness;
	s[2].w = w;
	s[2].h = thickness;

	s[3].x = aX;
	s[3].y = aY;
	s[3].w = thickness;
	s[3].h = h;

	PLEX_low_fillRects(r, g, b, a, s, 4, blendMode);
}

static void p_drawLine(const int64_t x, const int64_t y, const int64_t x2, const int64_t y2, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const uint8_t thickness, const bool offset, const SDL_BlendMode blendMode)
{
	const double sideX = x2 - (double)x;
	const double sideY = y2 - (double)y;

	const double len = sqrt((sideY * sideY) + (sideX * sideX));
	const double rad = atan2(sideY, sideX);
	const double xshift = cos(rad);
	const double yshift = sin(rad);

	const int64_t ilen = len;

	double xIt = x;
	double yIt = y;

	SDL_Rect dots[ilen];

	for(size_t z = 0; z < ilen; ++z)
	{
		dots[z].x = xIt - thickness/2 + (offset * PLEX_WOFFSET());
		dots[z].y = yIt - thickness/2 + (offset * PLEX_HOFFSET());
		dots[z].w = thickness;
		dots[z].h = thickness;

		xIt += xshift;
		yIt += yshift;
	}

	PLEX_low_fillRects(r, g, b, a, dots, ilen, blendMode);
}

static void p_drawLine2(const int64_t x, const int64_t y, const int64_t length, const double direction, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const uint8_t thickness, const bool offset, const SDL_BlendMode blendMode)
{
	const double rads = PLEX_radians(direction);

	const int64_t x2 = x + length * cos(rads);
	const int64_t y2 = y + length * sin(rads);

	p_drawLine(x, y, x2, y2, r, g, b, a, thickness, offset, blendMode);
}

static void p_fillRect(const int64_t x, const int64_t y, const int64_t w, const int64_t h, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const bool offset, const SDL_BlendMode blendMode)
{
	const int64_t aX = x + (offset * PLEX_WOFFSET());
	const int64_t aY = y + (offset * PLEX_HOFFSET());

	SDL_Rect area;

	area.x = aX;
	area.y = aY;
	area.w = w;
	area.h = h;

	PLEX_low_fillRect(r, g, b, a, &area, blendMode);
}

static void p_printTexture(const size_t texture, const int64_t fx, const int64_t fy, const int64_t fw, const int64_t fh, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const uint8_t r, const uint8_t g, const uint8_t b, const bool offset, const SDL_BlendMode blendMode)
{
	PLEX_LOW_TEXTURE target = PLEX_innerTexture(texture);

	SDL_Rect from, to;

	uint8_t or = 255, og = 255, ob = 255;

	if(PLEX_low_isTextureNull(target))
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GRAPHICS, PLEX_ERROR_ISNONTEXTURE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_TEXTURE_TEXTURE, PLEX_ERRORMESS_GRAPHICS_ONPRINTINGTEXTURE);
		return;
	}

	from.x = fx;
	from.y = fy;
	from.w = fw;
	from.h = fh;

	to.x = tx + (offset * PLEX_WOFFSET());
	to.y = ty + (offset * PLEX_HOFFSET());
	to.w = tw;
	to.h = th;

	PLEX_low_getTintTexturesRGB(target, &or, &og, &ob);

	PLEX_low_tintTexturesRGB(&target, r, g, b);
	PLEX_low_copyTexture(target, &from, &to, blendMode);
	PLEX_low_tintTexturesRGB(&target, or, og, ob);
}

static void p_printTextureRotated(const size_t texture, const int64_t fx, const int64_t fy, const int64_t fw, const int64_t fh, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const uint8_t r, const uint8_t g, const uint8_t b, const double angle, const bool offset, const SDL_BlendMode blendMode)
{
	PLEX_LOW_TEXTURE target = PLEX_innerTexture(texture);

	SDL_Rect from, to;

	uint8_t or = 255, og = 255, ob = 255;

	if(PLEX_low_isTextureNull(target))
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GRAPHICS, PLEX_ERROR_ISNONTEXTURE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_TEXTURE_TEXTURE, PLEX_ERRORMESS_GRAPHICS_ONPRINTINGTEXTUREROTATED);
		return;
	}

	from.x = fx;
	from.y = fy;
	from.w = fw;
	from.h = fh;

	to.x = tx + (offset * PLEX_WOFFSET());
	to.y = ty + (offset * PLEX_HOFFSET());
	to.w = tw;
	to.h = th;

	PLEX_low_getTintTexturesRGB(target, &or, &og, &ob);

	PLEX_low_tintTexturesRGB(&target, r, g, b);
	PLEX_low_copyTextureRotated(target, &from, &to, angle, blendMode);
	PLEX_low_tintTexturesRGB(&target, or, og, ob);
}

static void p_printFontLetter(const double wPerc, const double hPerc, const int64_t ox, const int64_t oy, const int64_t ow, const int64_t oh, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t symbol, const char* const alphaString, const size_t texture, const bool offset, const SDL_BlendMode blendMode)
{
	PLEX_LOW_TEXTURE target = PLEX_innerTexture(texture);

	size_t symbolIt = 0;

	char next = 0;

	// must be uint32_t type:
	uint32_t tW = 0;
	uint32_t tH = 0;

	int64_t lW = 0;

	uint8_t or = 0;
	uint8_t og = 0;
	uint8_t ob = 0;

	SDL_Rect crop, output;

	next = alphaString[symbolIt];

	while(next != '\0')
	{
		if(symbol == next) break;
		symbolIt++;
		next = alphaString[symbolIt];
	}

	if(next == '\0') return;

	PLEX_textureWH(texture, &tW, &tH);

	lW = 1/wPerc;

	crop.x = (symbolIt % lW) * wPerc * tW;
	crop.y = (symbolIt/lW) * hPerc * tH;
	crop.w = tW * wPerc;
	crop.h = tH * hPerc;

	output.x = ox + (offset * PLEX_WOFFSET());
	output.y = oy + (offset * PLEX_HOFFSET());
	output.w = ow * PLEX_STANDNUM_LETTERBUFF;
	output.h = oh * PLEX_STANDNUM_LETTERBUFF;

	PLEX_low_getTintTexturesRGB(target, &or, &og, &ob);

	PLEX_low_tintTexturesRGB(&target, r, g, b);
	PLEX_low_copyTexture(target, &crop, &output, blendMode);
	PLEX_low_tintTexturesRGB(&target, or, og, ob);
}

static void p_addDrawList(PLEX_GRAPHICS_INSTRUCT_TYPE type, PLEX_GRAPHICS_INSTRUCT_CONTENTS contents, const size_t order, const bool offset, const size_t errorline, const char* const errorfile)
{
	PLEX_LOW_TEXTURE renderTarget = PLEX_low_getRenderTarget();

	PLEX_GRAPHICS_INSTRUCT* instruct = NULL;

	size_t insert = 0;

	if(p_drawList == NULL)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_GRAPHICS, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_GRAPHICS_ONADDINGTODRAWLIST);
		return;
	}

	insert = PLEX_expandArrayCount(p_drawList) - 1;

	for(size_t z = 0; z < p_drawList->count; ++z)
	{
		PLEX_GRAPHICS_INSTRUCT* entry = (PLEX_GRAPHICS_INSTRUCT*)PLEX_getExpandArrayEntry(z, p_drawList);

		if((!PLEX_low_compTextures(entry->target, renderTarget) && PLEX_low_lessTextures(entry->target, renderTarget)) || (PLEX_low_compTextures(entry->target, renderTarget) && entry->order < order))
		{
			insert = z;

			PLEX_incrementExpandArrayInt(p_drawList, errorline, errorfile);

			for(size_t u = p_drawList->count - 1; u > z; u--)
			{
				PLEX_GRAPHICS_INSTRUCT* entry2 = (PLEX_GRAPHICS_INSTRUCT*)PLEX_getExpandArrayEntry(u, p_drawList);
				PLEX_GRAPHICS_INSTRUCT* entry3 = (PLEX_GRAPHICS_INSTRUCT*)PLEX_getExpandArrayEntry(u - 1, p_drawList);

				memcpy(entry2, entry3, sizeof(PLEX_GRAPHICS_INSTRUCT));
			}

			break;
		}
	};

	if(!p_drawList->count || insert == p_drawList->count - 1)
	{
		PLEX_incrementExpandArrayInt(p_drawList, errorline, errorfile);
		instruct = (PLEX_GRAPHICS_INSTRUCT*)PLEX_getExpandArrayLast(p_drawList);
	}else{
		instruct = (PLEX_GRAPHICS_INSTRUCT*)PLEX_getExpandArrayEntry(insert, p_drawList);
	}

	instruct->order = order;
	instruct->offset = offset;
	instruct->type = type;
	instruct->target = renderTarget;
	instruct->drawBlendMode = PLEX_low_getRenderBlendMode();
	memcpy(&instruct->contents, &contents, sizeof(contents));
}

static void p_clear(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a)
{
	const SDL_BlendMode oldMode = PLEX_low_getRenderBlendMode();

	p_fillRect(0, 0, PLEX_WW(true), PLEX_WH(true), r, g, b, a, true, SDL_BLENDMODE_NONE);

	PLEX_low_setBlendTarget(oldMode);
}

static void p_drawDrawList(const size_t errorline, const char* const errorfile)
{
	if(p_drawList == NULL)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_GRAPHICS, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_GRAPHICS_ONDRAWDRAWLIST);
		return;
	}

	for(size_t ze = 0; ze < PLEX_expandArrayCount(p_drawList); ++ze)
	{
		PLEX_GRAPHICS_INSTRUCT* entry = (PLEX_GRAPHICS_INSTRUCT*)PLEX_getExpandArrayEntry(ze, p_drawList);

		PLEX_low_setRenderTarget(entry->target);

		switch(entry->type)
		{
			case PLEX_GRAPHICS_INSTRUCT_TYPE_RECT:
				p_drawRect(entry->contents.rect.x, entry->contents.rect.y, entry->contents.rect.w, entry->contents.rect.h, entry->contents.rect.r, entry->contents.rect.g, entry->contents.rect.b, entry->contents.rect.a, entry->contents.rect.thickness, entry->offset, entry->drawBlendMode);
			break;

			case PLEX_GRAPHICS_INSTRUCT_TYPE_LINE:
				p_drawLine(entry->contents.line.x, entry->contents.line.y, entry->contents.line.x2, entry->contents.line.y2, entry->contents.line.r, entry->contents.line.g, entry->contents.line.b, entry->contents.line.a, entry->contents.line.thickness, entry->offset, entry->drawBlendMode);
			break;

			case PLEX_GRAPHICS_INSTRUCT_TYPE_LINE2:
				p_drawLine2(entry->contents.line2.x, entry->contents.line2.y, entry->contents.line2.length, entry->contents.line2.direction, entry->contents.line2.r, entry->contents.line2.g, entry->contents.line2.b, entry->contents.line2.a, entry->contents.line2.thickness, entry->offset, entry->drawBlendMode);
			break;

			case PLEX_GRAPHICS_INSTRUCT_TYPE_FILL_RECT:
				p_fillRect(entry->contents.fillRect.x, entry->contents.fillRect.y, entry->contents.fillRect.w, entry->contents.fillRect.h, entry->contents.fillRect.r, entry->contents.fillRect.g, entry->contents.fillRect.b, entry->contents.fillRect.a, entry->offset, entry->drawBlendMode);
			break;

			case PLEX_GRAPHICS_INSTRUCT_TYPE_TEXTURE:
				p_printTexture(entry->contents.texture.texture, entry->contents.texture.fx, entry->contents.texture.fy, entry->contents.texture.fw, entry->contents.texture.fh, entry->contents.texture.tx, entry->contents.texture.ty, entry->contents.texture.tw, entry->contents.texture.th, entry->contents.texture.r, entry->contents.texture.g, entry->contents.texture.b, entry->offset, entry->drawBlendMode);
			break;

			case PLEX_GRAPHICS_INSTRUCT_TYPE_TEXTURE_ROTATE:
				p_printTextureRotated(entry->contents.textureRotated.texture, entry->contents.textureRotated.fx, entry->contents.textureRotated.fy, entry->contents.textureRotated.fw, entry->contents.textureRotated.fh, entry->contents.textureRotated.tx, entry->contents.textureRotated.ty, entry->contents.textureRotated.tw, entry->contents.textureRotated.th, entry->contents.textureRotated.r, entry->contents.textureRotated.g, entry->contents.textureRotated.b, entry->contents.textureRotated.angle, entry->offset, entry->drawBlendMode);
			break;

			case PLEX_GRAPHICS_INSTRUCT_TYPE_FONT_LETTER:
				p_printFontLetter(entry->contents.letter.wPerc, entry->contents.letter.hPerc, entry->contents.letter.ox, entry->contents.letter.oy, entry->contents.letter.ow, entry->contents.letter.oh, entry->contents.letter.r, entry->contents.letter.g, entry->contents.letter.b, entry->contents.letter.symbol, entry->contents.letter.alphaString, entry->contents.letter.texture, entry->offset, entry->drawBlendMode);
			break;

			case PLEX_GRAPHICS_INSTRUCT_TYPE_CLEAR:
				p_clear(entry->contents.clear.r, entry->contents.clear.g, entry->contents.clear.b, entry->contents.clear.a);
			break;
		}
	}

	PLEX_low_setRenderTarget(PLEX_innerTexture(p_screenPlate));

	PLEX_destroyExpandArray(&p_drawList);
	p_drawList = PLEX_genExpandArrayInt(PLEX_getMemProfileDrawlistPreload(), sizeof(PLEX_GRAPHICS_INSTRUCT), errorline, errorfile);
}

// pass as uint16_t rather than uint8_t to prevent cast overflow issues
static uint8_t p_getTintAmount(const uint16_t r, const uint16_t g, const uint16_t b)
{
	const uint16_t tintDeficit = (r + g + b)/3.0;

	return tintDeficit < 255 ? 255 - tintDeficit : 255;
}

static uint8_t p_getReverseTintAmount(const uint16_t r, const uint16_t g, const uint16_t b)
{
	return 255 - p_getTintAmount(r, g, b);
}

static size_t p_drawTextBoxTintedBase(const size_t order, const char* const message, const double wPerc, const double hPerc, const uint8_t fr, const uint8_t fg, const uint8_t fb, const uint8_t fa, const uint8_t br, const uint8_t bg, const uint8_t bb, const uint8_t ba, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const uint32_t fw, const uint32_t fh, const double ipx, const double ipy, const double ipw, const double iph, const uint8_t thickness, const char* const alphaString, const size_t fontTexture, const size_t texture, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const double lineSpacePerc, const bool scroll, const size_t lineAmount, const bool offset, const size_t errorline, const char* const errorfile)
{
	const int64_t ix = x + ipx * w;
	const int64_t iy = y + ipy * h;
	const int64_t iw = ipw * w;
	const int64_t ih = iph * h;

	char word[1024];
	size_t wordLen = 0;

	int64_t px = ix;
	int64_t py = iy;
	int64_t apy = iy;

	size_t it = 0;
	size_t wordEnd = 0;

	uint64_t next = 0;

	size_t pageScroll = lineAmount;
	size_t page = 0;

	bool scrollForward = false;

	if(texture) PLEX_printTextureInt(order, texture, tx, ty, tw, th, x, y, w, h, offset, errorline, errorfile);
	else PLEX_fillRectInt(order, x, y, w, h, br, bg, bb, ba, offset, errorline, errorfile);

	word[0] = '\0';
	next = message[0];
	++it;

	while(next != '\0' || wordLen)
	{
		if(next != ' ' && next != '\r' && next != '\t' && next != '\n' && next != '\0')
		{
			if(wordLen < 1023)
			{
				word[wordLen] = next;
				word[wordLen + 1] = '\0';
				++wordLen;
			}else break;
		}else{
			if(wordLen)
			{
				const uint32_t wordWidth = wordLen * fw;

				const bool onPage = page >= pageScroll;

				if(px + wordWidth > x + iw)
				{
					const double lh = fh * (1 + lineSpacePerc);

					px = ix;
					py += lh;
					if(onPage) apy += lh;

					if(py + lh > iy + ih)
					{
						if(onPage)
						{
							scrollForward = true;
							wordLen = 0;
							it = wordEnd;
							break;
						}else{
							py = apy;
							++page;
							continue;
						}
					}
				}

				if(onPage) PLEX_printFontStringTintedInt(order, wPerc, hPerc, px, apy, fw, fh, fr, fg, fb, (char*)word, alphaString, fontTexture, offset, errorline, errorfile);

				px += wordWidth + fw;
				wordEnd = it;

				word[0] = '\0';
				wordLen = 0;
			}
		}

		if(next != '\0')
		{
			next = message[it];

			++it;
		}
	}

	PLEX_drawRectInt(order, x, y, w, h, fr, fg, fb, fa, thickness, offset, errorline, errorfile);

	if(PLEX_mouseInside(x, y, w, h, offset))
	{
		if(scroll)
		{
			if(scrollForward)
			{
				if(PLEX_drawArrowButtonInt(order, x + w - fh, y + h - fh, fh, fh, fr, fg, fb, fa, br, bg, bb, ba, thickness, false, offset, errorline, errorfile)) ++pageScroll;
			}

			if(pageScroll >= page && pageScroll)
			{
				if(PLEX_drawArrowButtonInt(order, x + w - (fh * 2), y + h - fh, fh, fh, fr, fg, fb, fa, br, bg, bb, ba, thickness, true, offset, errorline, errorfile)) --pageScroll;
			}
		}
	}

	return pageScroll;
}

// ---------------------------- //
// PUBLIC FUNCTIONS:
// ---------------------------- //

bool PLEX_startGraphics()
{
	if(p_drawList != NULL)
	{
		PLEX_errorNoDesc(PLEX_ERROR_CATEGORY_GRAPHICS, PLEX_ERROR_DOUBLESTART, PLEX_ERROR_SEVERITY_MINOR);
		return true;
	}

	p_drawList = PLEX_genExpandArray(PLEX_getMemProfileDrawlistPreload(), sizeof(PLEX_GRAPHICS_INSTRUCT));

	PLEX_low_setBlendTarget(SDL_BLENDMODE_BLEND);

	p_screenPlate = PLEX_createGeneratedTexture(PLEX_WW(false), PLEX_WH(false), 0, 0, 0, 255);

	PLEX_low_setRenderTarget(PLEX_innerTexture(p_screenPlate));

	PLEX_low_setBlendTarget(SDL_BLENDMODE_BLEND);

	p_regularCursor = PLEX_loadTextureFromFile(64, 64, 255, 255, 255, 255, 0, PLEX_RECDIRECTORY "plex/cursor.png");

	if(!p_regularCursor)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GRAPHICS, PLEX_ERROR_ZEROVALUE, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_GRAPHICS_ONGRAPHICSSTART, PLEX_ERRORMESS_WINDOW_CURSORGRAPHIC);
		return false;
	}

	p_regularCursorClick = PLEX_loadTextureFromFile(64, 64, 255, 255, 255, 255, 0, PLEX_RECDIRECTORY "plex/cursorclick.png");

	if(!p_regularCursorClick)
	{
		PLEX_destroyTexture(p_regularCursor);

		p_regularCursor = 0;

		PLEX_error2(PLEX_ERROR_CATEGORY_GRAPHICS, PLEX_ERROR_ZEROVALUE, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_GRAPHICS_ONGRAPHICSSTART, PLEX_ERRORMESS_WINDOW_CURSORGRAPHICCLICK);
		return false;
	}

	p_regularCursorDown = PLEX_loadTextureFromFile(64, 64, 255, 255, 255, 255, 0, PLEX_RECDIRECTORY "plex/cursordown.png");

	if(!p_regularCursorDown)
	{
		PLEX_destroyTexture(p_regularCursor);
		PLEX_destroyTexture(p_regularCursorClick);

		p_regularCursor = 0;
		p_regularCursorClick = 0;

		PLEX_error2(PLEX_ERROR_CATEGORY_GRAPHICS, PLEX_ERROR_ZEROVALUE, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_GRAPHICS_ONGRAPHICSSTART, PLEX_ERRORMESS_WINDOW_CURSORGRAPHICDOWN);
		return false;
	}

	return true;
}

bool PLEX_updateGraphics(SDL_Window* window, const uint32_t w, const uint32_t h)
{
	PLEX_LOW_TEXTURE innerPlate = PLEX_innerTexture(p_screenPlate);

	uint32_t pw = 0;
	uint32_t ph = 0;

	if(p_drawList == NULL)
	{
		PLEX_errorNoDesc(PLEX_ERROR_CATEGORY_GRAPHICS, PLEX_ERROR_UPDATEBEFORESTART, PLEX_ERROR_SEVERITY_MINOR);
		return false;
	}

	if(window == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GRAPHICS, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_GRAPHICS_ONGRAPHICSUPDATE, PLEX_ERRORMESS_WINDOW_WINDOW);
		return false;
	}

	if(!PLEX_isSizing())
	{
		innerPlate = PLEX_innerTexture(p_screenPlate);

		PLEX_low_setRenderTarget(innerPlate);

		PLEX_low_fillRect(p_refreshR, p_refreshG, p_refreshB, 255, NULL, SDL_BLENDMODE_NONE);
		PLEX_low_setBlendTarget(SDL_BLENDMODE_BLEND);

		p_drawDrawList(__LINE__, __FILE__);

		PLEX_low_setRenderTarget(PLEX_LOW_NOTEXTURE);

		PLEX_low_textureWH(innerPlate, &pw, &ph);

		if(pw != PLEX_WW(false) || ph != PLEX_WH(false))
		{
			PLEX_destroyTexture(p_screenPlate);
			p_screenPlate = PLEX_createGeneratedTexture(PLEX_WW(false), PLEX_WH(false), 0, 0, 0, 255);
			innerPlate = PLEX_innerTexture(p_screenPlate);
		}

		PLEX_low_updateRenderer(window, w, h);
		PLEX_low_copyTexture(innerPlate, PLEX_LOW_NOTEXTURE, PLEX_LOW_NOTEXTURE, SDL_BLENDMODE_BLEND);

		if(PLEX_usingCustomCursor())
		{
			const size_t tex = PLEX_customCursor();

			uint32_t w = 0;
			uint32_t h = 0;

			uint32_t fx = 0;
			uint32_t fy = 0;
			uint32_t fw = 0;
			uint32_t fh = 0;

			PLEX_cursorDimens(&w, &h, &fx, &fy, &fw, &fh);

			PLEX_printTexture(0, tex, fx, fy, fw, fh, PLEX_MX(), PLEX_MY(), w, h, false);
		}else{
			if(!PLEX_cursorIsHidden())
			{
				size_t regularCursor = 0;

				if(PLEX_leftClick()) regularCursor = p_regularCursorClick;
				else if(PLEX_left()) regularCursor = p_regularCursorDown;
				else regularCursor = p_regularCursor;

				PLEX_printTextureTinted(0, regularCursor, 0, 0, 64, 64, PLEX_MX(), PLEX_MY(), PLEX_UNIT(1), PLEX_UNIT(1), PLEX_DEF_cursorR(), PLEX_DEF_cursorG(), PLEX_DEF_cursorB(), false);
			}
		}

		PLEX_low_setRenderTarget(innerPlate);
	}else{
		PLEX_destroyTexture(p_screenPlate);
		p_screenPlate = PLEX_createGeneratedTexture(PLEX_WW(false), PLEX_WH(false), 0, 0, 0, 255);
		return true;
	}

	PLEX_updateTextures();

	p_showClickables = false;
	p_showTooltips = false;

	return true;
}

void PLEX_endGraphics()
{
	PLEX_destroyTexture(p_regularCursor);
	PLEX_destroyTexture(p_regularCursorClick);
	PLEX_destroyTexture(p_regularCursorDown);

	p_regularCursor = 0;
	p_regularCursorClick = 0;
	p_regularCursorDown = 0;

	if(p_drawList == NULL) PLEX_errorNoDesc(PLEX_ERROR_CATEGORY_GRAPHICS, PLEX_ERROR_ENDBEFORESTART, PLEX_ERROR_SEVERITY_MINOR);

	PLEX_destroyExpandArray(&p_drawList);

	PLEX_endTextures();
}

void PLEX_setTextureTarget(const size_t texture)
{
	PLEX_LOW_TEXTURE target = PLEX_innerTexture(texture);

	PLEX_low_setRenderTarget(target);
}

void PLEX_unsetTextureTarget()
{
	PLEX_setTextureTarget(p_screenPlate);
}

void PLEX_drawRectInt(const size_t order, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const uint8_t thickness, const bool offset, const size_t errorline, const char* const errorfile)
{
	PLEX_GRAPHICS_INSTRUCT_CONTENTS contents;

	contents.rect.x = x;
	contents.rect.y = y;
	contents.rect.w = w;
	contents.rect.h = h;

	contents.rect.r = r;
	contents.rect.g = g;
	contents.rect.b = b;
	contents.rect.a = a;

	contents.rect.thickness = thickness;

	p_addDrawList(PLEX_GRAPHICS_INSTRUCT_TYPE_RECT, contents, order, offset, errorline, errorfile);
}

void PLEX_drawLineInt(const size_t order, const int64_t x, const int64_t y, const int64_t x2, const int64_t y2, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const uint8_t thickness, const bool offset, const size_t errorline, const char* const errorfile)
{
	PLEX_GRAPHICS_INSTRUCT_CONTENTS contents;

	contents.line.x = x > 0 ? x : 0;
	contents.line.y = y > 0 ? y : 0;
	contents.line.x2 = x2 > 0 ? x2 : 0;
	contents.line.y2 = y2 > 0 ? y2 : 0;

	contents.line.r = r;
	contents.line.g = g;
	contents.line.b = b;
	contents.line.a = a;

	contents.line.thickness = thickness;

	p_addDrawList(PLEX_GRAPHICS_INSTRUCT_TYPE_LINE, contents, order, offset, errorline, errorfile);
}

void PLEX_drawLine2Int(const size_t order, const int64_t x, int64_t y, const int64_t length, const double direction, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const uint8_t thickness, const bool offset, const size_t errorline, const char* const errorfile)
{
	PLEX_GRAPHICS_INSTRUCT_CONTENTS contents;

	contents.line2.x = x;
	contents.line2.y = y;
	contents.line2.length = length;
	contents.line2.direction = direction;

	contents.line2.r = r;
	contents.line2.g = g;
	contents.line2.b = b;
	contents.line2.a = a;

	contents.line2.thickness = thickness;

	p_addDrawList(PLEX_GRAPHICS_INSTRUCT_TYPE_LINE2, contents, order, offset, errorline, errorfile);
}

void PLEX_drawLine3Int(const size_t order, const PLEX_LINE line, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const uint8_t thickness, const bool offset, const size_t errorline, const char* const errorfile)
{
	const PLEX_MPREC rads = PLEX_slopeToRads2(line.rise, line.run);

	PLEX_drawLine2Int(order, line.origin.x, line.origin.y, line.length, PLEX_degrees(rads), r, g, b, a, thickness, offset, errorline, errorfile);
}

void PLEX_fillRectInt(const size_t order, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const bool offset, const size_t errorline, const char* const errorfile)
{
	PLEX_GRAPHICS_INSTRUCT_CONTENTS contents;

	contents.fillRect.x = x;
	contents.fillRect.y = y;
	contents.fillRect.w = w;
	contents.fillRect.h = h;

	contents.fillRect.r = r;
	contents.fillRect.g = g;
	contents.fillRect.b = b;
	contents.fillRect.a = a;

	p_addDrawList(PLEX_GRAPHICS_INSTRUCT_TYPE_FILL_RECT, contents, order, offset, errorline, errorfile);
}

void PLEX_printTextureInt(const size_t order, const size_t texture, const int64_t fx, const int64_t fy, const int64_t fw, const int64_t fh, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const bool offset, const size_t errorline, const char* const errorfile)
{
	PLEX_printTextureTintedInt(order, texture, fx, fy, fw, fh, tx, ty, tw, th, 255, 255, 255, offset, errorline, errorfile);
}

void PLEX_printTextureTintedInt(const size_t order, const size_t texture, const int64_t fx, const int64_t fy, const int64_t fw, const int64_t fh, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const uint8_t r, const uint8_t g, const uint8_t b, const bool offset, const size_t errorline, const char* const errorfile)
{
	PLEX_GRAPHICS_INSTRUCT_CONTENTS contents;

	contents.texture.texture = texture;

	contents.texture.fx = fx;
	contents.texture.fy = fy;
	contents.texture.fw = fw;
	contents.texture.fh = fh;

	contents.texture.tx = tx;
	contents.texture.ty = ty;
	contents.texture.tw = tw;
	contents.texture.th = th;

	contents.texture.r = r;
	contents.texture.g = g;
	contents.texture.b = b;

	contents.texture.textureBlendMode = PLEX_low_getTextureBlendMode(PLEX_innerTexture(texture));

	p_addDrawList(PLEX_GRAPHICS_INSTRUCT_TYPE_TEXTURE, contents, order, offset, errorline, errorfile);
}

void PLEX_printTextureRotatedInt(const size_t order, const size_t texture, const int64_t fx, const int64_t fy, const int64_t fw, const int64_t fh, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const double angle, const bool offset, const size_t errorline, const char* const errorfile)
{
	PLEX_GRAPHICS_INSTRUCT_CONTENTS contents;

	contents.textureRotated.texture = texture;

	contents.textureRotated.fx = fx;
	contents.textureRotated.fy = fy;
	contents.textureRotated.fw = fw;
	contents.textureRotated.fh = fh;

	contents.textureRotated.tx = tx;
	contents.textureRotated.ty = ty;
	contents.textureRotated.tw = tw;
	contents.textureRotated.th = th;

	contents.textureRotated.r = 255;
	contents.textureRotated.g = 255;
	contents.textureRotated.b = 255;

	contents.textureRotated.angle = angle;

	contents.textureRotated.textureBlendMode = PLEX_low_getTextureBlendMode(PLEX_innerTexture(texture));

	p_addDrawList(PLEX_GRAPHICS_INSTRUCT_TYPE_TEXTURE_ROTATE, contents, order, offset, errorline, errorfile);
}

void PLEX_printFontLetterInt(const size_t order, const double wPerc, const double hPerc, const int64_t ox, const int64_t oy, const int64_t ow, const int64_t oh, const uint8_t symbol, const char* const alphaString, const size_t texture, const bool offset, const size_t errorline, const char* const errorfile)
{
	if(!hPerc)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GRAPHICS, PLEX_ERROR_ZEROVALUE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_GRAPHICS_ONLETTERPRINTING, PLEX_ERRORMESS_GENERIC_HPERC);
		return;
	}

	if(!wPerc)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GRAPHICS, PLEX_ERROR_ZEROVALUE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_GRAPHICS_ONLETTERPRINTING, PLEX_ERRORMESS_GENERIC_WPERC);
		return;
	}

	if(alphaString == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GRAPHICS, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_GRAPHICS_ONLETTERPRINTING, PLEX_ERRORMESS_GENERIC_ALPHASTRING);
		return;
	}

	if(!texture)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GRAPHICS, PLEX_ERROR_ISNONTEXTURE, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_GRAPHICS_ONLETTERPRINTING, PLEX_ERRORMESS_TEXTURE_TEXTURE);
		return;
	}

	PLEX_printFontLetterTintedInt(order, wPerc, hPerc, ox, oy, ow, oh, 255, 255, 255, symbol, alphaString, texture, offset, errorline, errorfile);
}

void PLEX_printFontLetterTintedInt(const size_t order, const double wPerc, const double hPerc, const int64_t ox, const int64_t oy, const int64_t ow, const int64_t oh, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t symbol, const char* const alphaString, const size_t texture, const bool offset, const size_t errorline, const char* const errorfile)
{
	PLEX_GRAPHICS_INSTRUCT_CONTENTS contents;

	if(alphaString == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GRAPHICS, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_GRAPHICS_ONLETTERPRINTINGTINTED,  PLEX_ERRORMESS_GENERIC_ALPHASTRING);
		return;
	}

	contents.letter.wPerc = wPerc;
	contents.letter.hPerc = hPerc;

	contents.letter.ox = ox;
	contents.letter.oy = oy;
	contents.letter.ow = ow;
	contents.letter.oh = oh;

	contents.letter.r = r;
	contents.letter.g = g;
	contents.letter.b = b;

	contents.letter.symbol = symbol;

	contents.letter.alphaString = alphaString;

	contents.letter.texture = texture;

	contents.letter.textureBlendMode = PLEX_low_getTextureBlendMode(PLEX_innerTexture(texture));

	if(contents.letter.ox + contents.letter.ow > PLEX_WW(false)) return;
	if(contents.letter.oy + contents.letter.oh > PLEX_WH(false)) return;

	p_addDrawList(PLEX_GRAPHICS_INSTRUCT_TYPE_FONT_LETTER, contents, order, offset, errorline, errorfile);
}

void PLEX_printFontStringInt(const size_t order, const double wPerc, const double hPerc, const int64_t ox, const int64_t oy, const int64_t ow, const int64_t oh, const char* const string, const char* const alphaString, const size_t texture, const bool offset, const size_t errorline, const char* const errorfile)
{
	if(string == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GRAPHICS, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_GRAPHICS_ONSTRINGPRINTING, PLEX_ERRORMESS_GENERIC_STRING);
		return;
	}

	if(alphaString == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GRAPHICS, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_GRAPHICS_ONSTRINGPRINTING, PLEX_ERRORMESS_GENERIC_ALPHASTRING);
		return;
	}

	PLEX_printFontStringTintedInt(order, wPerc, hPerc, ox, oy, ow, oh, 255, 255, 255, string, alphaString, texture, offset, errorline, errorfile);
}

void PLEX_printFontStringTintedInt(const size_t order, const double wPerc, const double hPerc, const int64_t ox, const int64_t oy, const int64_t ow, const int64_t oh, const uint8_t r, const uint8_t g, const uint8_t b, const char* const string, const char* const alphaString, const size_t texture, const bool offset, const size_t errorline, const char* const errorfile)
{
	int64_t aoX = ox;

	size_t it = 0;

	char next = 0;

	if(string == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GRAPHICS, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_GRAPHICS_ONSTRINGPRINTINGTINTED, PLEX_ERRORMESS_GENERIC_STRING);
		return;
	}

	if(alphaString == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GRAPHICS, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_GRAPHICS_ONSTRINGPRINTINGTINTED, PLEX_ERRORMESS_GENERIC_ALPHASTRING);
		return;
	}

	next = string[it];

	while(next != '\0')
	{
		PLEX_printFontLetterTintedInt(order, wPerc, hPerc, aoX, oy, ow, oh, r, g, b, next, alphaString, texture, offset, errorline, errorfile);

		aoX += ow;
		it++;
		next = string[it];
	}
}

bool PLEX_drawArrowButtonInt(const size_t order, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const uint8_t fr, const uint8_t fg, const uint8_t fb, const uint8_t fa, const uint8_t br, const uint8_t bg, const uint8_t bb, const uint8_t ba, const uint8_t thickness, const bool flip, const bool offset, const size_t errorline, const char* const errorfile)
{
	const bool inside = PLEX_mouseInside(x, y, w, h, offset);

	if(inside)
	{
		PLEX_fillRectInt(order, x, y, w, h, fr, fg, fb, fa, offset, errorline, errorfile);

		if(!flip)
		{
			PLEX_fillRectInt(order, x + w * 0.2, y + h * 0.2,  w * 0.2, h - h * 0.4, br, bg, bb, ba, offset, errorline, errorfile);
			PLEX_fillRectInt(order, x + w * 0.3, y + h * 0.25, w * 0.2, h - h * 0.5, br, bg, bb, ba, offset, errorline, errorfile);
			PLEX_fillRectInt(order, x + w * 0.4, y + h * 0.3,  w * 0.2, h - h * 0.6, br, bg, bb, ba, offset, errorline, errorfile);
			PLEX_fillRectInt(order, x + w * 0.5, y + h * 0.35, w * 0.2, h - h * 0.7, br, bg, bb, ba, offset, errorline, errorfile);
			PLEX_fillRectInt(order, x + w * 0.6, y + h * 0.4,  w * 0.2, h - h * 0.8, br, bg, bb, ba, offset, errorline, errorfile);
		}else{
			PLEX_fillRectInt(order, x + w * 0.2, y + h * 0.4,  w * 0.2, h - h * 0.8, br, bg, bb, ba, offset, errorline, errorfile);
			PLEX_fillRectInt(order, x + w * 0.3, y + h * 0.35, w * 0.2, h - h * 0.7, br, bg, bb, ba, offset, errorline, errorfile);
			PLEX_fillRectInt(order, x + w * 0.4, y + h * 0.3,  w * 0.2, h - h * 0.6, br, bg, bb, ba, offset, errorline, errorfile);
			PLEX_fillRectInt(order, x + w * 0.5, y + h * 0.25, w * 0.2, h - h * 0.5, br, bg, bb, ba, offset, errorline, errorfile);
			PLEX_fillRectInt(order, x + w * 0.6, y + h * 0.2,  w * 0.2, h - h * 0.4, br, bg, bb, ba, offset, errorline, errorfile);
		}

		PLEX_drawRectInt(order, x, y, w, h, br, bg, bb, ba, thickness, offset, errorline, errorfile);
		return PLEX_leftClick();
	}else{
		PLEX_fillRectInt(order, x, y, w, h, br, bg, bb, ba, offset, errorline, errorfile);

		if(!flip)
		{
			PLEX_fillRectInt(order, x + w * 0.2, y + h * 0.2,  w * 0.2, h - h * 0.4, fr, fg, fb, fa, offset, errorline, errorfile);
			PLEX_fillRectInt(order, x + w * 0.3, y + h * 0.25, w * 0.2, h - h * 0.5, fr, fg, fb, fa, offset, errorline, errorfile);
			PLEX_fillRectInt(order, x + w * 0.4, y + h * 0.3,  w * 0.2, h - h * 0.6, fr, fg, fb, fa, offset, errorline, errorfile);
			PLEX_fillRectInt(order, x + w * 0.5, y + h * 0.35, w * 0.2, h - h * 0.7, fr, fg, fb, fa, offset, errorline, errorfile);
			PLEX_fillRectInt(order, x + w * 0.6, y + h * 0.4,  w * 0.2, h - h * 0.8, fr, fg, fb, fa, offset, errorline, errorfile);
		}else{
			PLEX_fillRectInt(order, x + w * 0.2, y + h * 0.4,  w * 0.2, h - h * 0.8, fr, fg, fb, fa, offset, errorline, errorfile);
			PLEX_fillRectInt(order, x + w * 0.3, y + h * 0.35, w * 0.2, h - h * 0.7, fr, fg, fb, fa, offset, errorline, errorfile);
			PLEX_fillRectInt(order, x + w * 0.4, y + h * 0.3,  w * 0.2, h - h * 0.6, fr, fg, fb, fa, offset, errorline, errorfile);
			PLEX_fillRectInt(order, x + w * 0.5, y + h * 0.25, w * 0.2, h - h * 0.5, fr, fg, fb, fa, offset, errorline, errorfile);
			PLEX_fillRectInt(order, x + w * 0.6, y + h * 0.2,  w * 0.2, h - h * 0.4, fr, fg, fb, fa, offset, errorline, errorfile);
		}

		PLEX_drawRectInt(order, x, y, w, h, fr, fg, fb, fa, thickness, offset, errorline, errorfile);
	}

	if(p_showClickables) PLEX_fillRectInt(0, x, y, w, h, fr, fg, fb, fa * 0.5, offset, errorline, errorfile);

	return false;
}

void PLEX_drawIconInt(const size_t order, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const uint8_t br, const uint8_t bg, const uint8_t bb, const uint8_t ba, const size_t texture, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const uint8_t thickness, const bool offset, const size_t errorline, const char* const errorfile)
{
	PLEX_printTextureInt(order, texture, tx, ty, tw, th, x, y, w, h, offset, errorline, errorfile);
	PLEX_drawRectInt(order, x, y, w, h, br, bg, bb, ba, thickness, offset, errorline, errorfile);
}

void PLEX_drawIconTintedInt(const size_t order, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t br, const uint8_t bg, const uint8_t bb, const uint8_t ba, const size_t texture, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const uint8_t thickness, const bool offset, const size_t errorline, const char* const errorfile)
{
	PLEX_printTextureTintedInt(order, texture, tx, ty, tw, th, x, y, w, h, r, g, b, offset, errorline, errorfile);
	PLEX_drawRectInt(order, x, y, w, h, br, bg, bb, ba, thickness, offset, errorline, errorfile);
}

void PLEX_drawLabelInt(const size_t order, const char* const message, const double wPerc, const double hPerc, const uint8_t fr, const uint8_t fg, const uint8_t fb, const uint8_t fa, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const double ipx, const double ipy, const double ipw, const double iph, const uint8_t thickness, const char* const alphaString, const size_t fontTexture, const size_t texture, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const bool offset, const size_t errorline, const char* const errorfile)
{
	const int64_t ix = x + ipx * w;
	const int64_t iy = y + ipy * h;
	const int64_t iw = ipw * w;
	const int64_t ih = iph * h;

	size_t messLen = 0;

	int64_t letW = 0;

	if(message == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GRAPHICS, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_GRAPHICS_ONLABELDRAWING, PLEX_ERRORMESS_GENERIC_MESSAGE);
		return;
	}

	if(alphaString == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GRAPHICS, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_GRAPHICS_ONLABELDRAWING, PLEX_ERRORMESS_GENERIC_ALPHASTRING);
		return;
	}

	messLen = strlen(message);

	if(messLen < 8) messLen = 8;

	letW = iw/messLen;

	if(texture) PLEX_printTextureInt(order, texture, tx, ty, tw, th, x, y, w, h, offset, errorline, errorfile);
	else
	{
		const uint8_t tint = p_getReverseTintAmount(fr, fg, fb);

		PLEX_fillRectInt(order, x, y, w, h, tint, tint, tint, 255, offset, errorline, errorfile);
	}

	PLEX_printFontStringTintedInt(order, wPerc, hPerc, ix, iy, letW, ih, fr, fg, fb, message, alphaString, fontTexture, offset, errorline, errorfile);
	PLEX_drawRectInt(order, x, y, w, h, fr, fg, fb, fa, thickness, offset, errorline, errorfile);
}

void PLEX_drawTooltipInt(const size_t order, const char* const message, const double wPerc, const double hPerc, const uint8_t br, const uint8_t bg, const uint8_t bb, const uint8_t ba, const int64_t mx, const int64_t my, const int64_t mw, const int64_t mh, const int64_t dx, const int64_t dy, const int64_t lw, const int64_t lh, const double ipx, const double ipy, const double ipw, const double iph, const uint8_t thickness, const char* const alphaString, const size_t fontTexture, const size_t texture, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const bool offset, const size_t errorline, const char* const errorfile)
{
	const bool inside = PLEX_mouseInside(mx, my, mw, mh, offset) || p_showTooltips || p_toggleTooltips;

	if(message == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GRAPHICS, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_GRAPHICS_ONTOOLTIPDRAWING, PLEX_ERRORMESS_GENERIC_MESSAGE);
		return;
	}

	if(alphaString == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GRAPHICS, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_GRAPHICS_ONTOOLTIPDRAWING, PLEX_ERRORMESS_GENERIC_ALPHASTRING);
		return;
	}

	if(inside)
	{
		int64_t lx = PLEX_MX() - (PLEX_WOFFSET() * offset) + dx;
		int64_t ly = PLEX_MY() - (PLEX_HOFFSET() * offset) - dy;

		PLEX_drawLabelInt(order, message, wPerc, hPerc, br, bg, bb, ba, lx, ly, lw, lh, ipx, ipy, ipw, iph, thickness, alphaString, fontTexture, texture, tx, ty, tw, th, offset, errorline, errorfile);
	}
}

void PLEX_drawTextBoxInt(const size_t order, const char* const message, const double wPerc, const double hPerc, const uint8_t fr, const uint8_t fg, const uint8_t fb, const uint8_t fa, const int64_t hx, const int64_t hy, const int64_t hw, const int64_t hh, const uint32_t fw, const uint32_t fh, const double ipx, const double ipy, const double ipw, const double iph, const uint8_t thickness, const char* const alphaString, const size_t fontTexture, const size_t texture, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const double lineSpacePerc, const bool offset, const size_t errorline, const char* const errorfile)
{
	uint8_t tint = 0;

	if(message == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GRAPHICS, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_GRAPHICS_ONTEXTBOXDRAWING, PLEX_ERRORMESS_GENERIC_MESSAGE);
		return;
	}

	if(alphaString == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GRAPHICS, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_GRAPHICS_ONTEXTBOXDRAWING, PLEX_ERRORMESS_GENERIC_MESSAGE);
		return;
	}

	tint = p_getReverseTintAmount(fr, fg, fb)/2.0;

	PLEX_drawTextBoxTintedInt(order, message, wPerc, hPerc, fr, fg, fb, fa, tint, tint, tint, 255, hx, hy, hw, hh, fw, fh, ipx, ipy, ipw, iph, thickness, alphaString, fontTexture, texture, tx, ty, tw, th, lineSpacePerc, offset, errorline, errorfile);
}

void PLEX_drawTextBoxTintedInt(const size_t order, const char* const message, const double wPerc, const double hPerc, const uint8_t fr, const uint8_t fg, const uint8_t fb, const uint8_t fa, const uint8_t br, const uint8_t bg, const uint8_t bb, const uint8_t ba, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const uint32_t fw, const uint32_t fh, const double ipx, const double ipy, const double ipw, const double iph, const uint8_t thickness, const char* const alphaString, const size_t fontTexture, const size_t texture, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const double lineSpacePerc, const bool offset, const size_t errorline, const char* const errorfile)
{
	if(message == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GRAPHICS, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_GRAPHICS_ONTEXTBOXTINTEDDRAWING, PLEX_ERRORMESS_GENERIC_MESSAGE);
		return;
	}

	if(alphaString == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GRAPHICS, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_GRAPHICS_ONTEXTBOXTINTEDDRAWING, PLEX_ERRORMESS_GENERIC_ALPHASTRING);
		return;
	}

	p_drawTextBoxTintedBase(order, message, wPerc, hPerc, fr, fg, fb, fa, br, bg, bb, ba, x, y, w, h, fw, fh, ipx, ipy, ipw, iph, thickness, alphaString, fontTexture, texture, tx, ty, tw, th, lineSpacePerc, false, 0, offset, errorline, errorfile);
}

size_t PLEX_drawTextBoxScrollingInt(const size_t order, const char* const message, const double wPerc, const double hPerc, const uint8_t fr, const uint8_t fg, const uint8_t fb, const uint8_t fa, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const uint32_t fw, const uint32_t fh, const double ipx, const double ipy, const double ipw, const double iph, const uint8_t thickness, const char* const alphaString, const size_t fontTexture, const size_t texture, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const double lineSpacePerc, const size_t pageScroll, const bool offset, const size_t errorline, const char* const errorfile)
{
	uint8_t tint = 0;

	if(message == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GRAPHICS, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_GRAPHICS_ONSCROLLINGTEXTBOXDRAWING, PLEX_ERRORMESS_GENERIC_MESSAGE);
		return pageScroll;
	}

	if(alphaString == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GRAPHICS, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_GRAPHICS_ONSCROLLINGTEXTBOXDRAWING, PLEX_ERRORMESS_GENERIC_ALPHASTRING);
		return pageScroll;
	}

	tint = p_getReverseTintAmount(fr, fg, fb)/2.0;

	return PLEX_drawTextBoxScrollingTintedInt(order, message, wPerc, hPerc, fr, fg, fb, fa, tint, tint, tint, 255, x, y, w, h, fw, fh, ipx, ipy, ipw, iph, thickness, alphaString, fontTexture, texture, tx, ty, tw, th, lineSpacePerc, pageScroll, offset, errorline, errorfile);
}

size_t PLEX_drawTextBoxScrollingTintedInt(const size_t order, const char* const message, const double wPerc, const double hPerc, const uint8_t fr, const uint8_t fg, const uint8_t fb, const uint8_t fa, const uint8_t br, const uint8_t bg, const uint8_t bb, const uint8_t ba, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const uint32_t fw, const uint32_t fh, const double ipx, const double ipy, const double ipw, const double iph, const uint8_t thickness, const char* const alphaString, const size_t fontTexture, const size_t texture, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const double lineSpacePerc, const size_t pageScroll, const bool offset, const size_t errorline, const char* const errorfile)
{
	if(message == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GRAPHICS, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_GRAPHICS_ONSCROLLINGTEXTBOXTINTEDDRAWING, PLEX_ERRORMESS_GENERIC_MESSAGE);
		return pageScroll;
	}

	if(alphaString == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GRAPHICS, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_GRAPHICS_ONSCROLLINGTEXTBOXTINTEDDRAWING, PLEX_ERRORMESS_GENERIC_ALPHASTRING);
		return pageScroll;
	}

	return p_drawTextBoxTintedBase(order, message, wPerc, hPerc, fr, fg, fb, fa, br, bg, bb, ba, x, y, w, h, fw, fh, ipx, ipy, ipw, iph, thickness, alphaString, fontTexture, texture, tx, ty, tw, th, lineSpacePerc, true, pageScroll, offset, errorline, errorfile);
}

bool PLEX_drawButtonInt(const size_t order, const char* const message, const double wPerc, const double hPerc, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const uint8_t fr, const uint8_t fg, const uint8_t fb, const uint8_t fa, const uint8_t br, const uint8_t bg, const uint8_t bb, const uint8_t ba, const uint8_t thickness, const char* const alphaString, const size_t font, const size_t texture, const bool offset, const size_t errorline, const char* const errorfile)
{
	const uint32_t fh = (h - thickness) * 0.8;
	const uint32_t fw = fh * 0.5;
	const uint32_t hi = (h - fh)/2;

	const bool inside = PLEX_mouseInside(x, y, w, h, offset);

	size_t mlen = message == NULL ? 0 : strlen(message);
	uint32_t bw = 0;

	if(mlen && mlen < 8) mlen = 8;

	bw = mlen * fw;

	if(message == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GRAPHICS, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_GRAPHICS_ONBUTTONDDRAWING, PLEX_ERRORMESS_GENERIC_MESSAGE);
		return false;
	}

	if(alphaString == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GRAPHICS, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_GRAPHICS_ONBUTTONDDRAWING, PLEX_ERRORMESS_GENERIC_ALPHASTRING);
		return false;
	}

	if(texture)
	{
		PLEX_printTextureInt(order, texture, tx, ty, tw, th, x, y, w, h, offset, errorline, errorfile);

		if(inside) PLEX_fillRectInt(order, x, y, w, h, fr, fg, fb, fa * 0.5, offset, errorline, errorfile);
	}else{
		if(inside) PLEX_fillRectInt(order, x, y, w, h, fr, fg, fb, fa, offset, errorline, errorfile);
		else PLEX_fillRectInt(order, x, y, w, h, br, bg, bb, ba, offset, errorline, errorfile);
	}

	if(inside)
	{
		PLEX_drawRectInt(order, x, y, w, h, br, bg, bb, ba, thickness, offset, errorline, errorfile);

		PLEX_printFontStringTintedInt(order, wPerc, hPerc, x + (w - bw)/2, y + hi, fw, fh, br, bg, bb, message, alphaString, font, offset, errorline, errorfile);

		return PLEX_leftClick();
	}else{
		PLEX_drawRectInt(order, x, y, w, h, fr, fg, fb, fa, thickness, offset, errorline, errorfile);

		PLEX_printFontStringTintedInt(order, wPerc, hPerc, x + (w - bw)/2, y + hi, fw, fh, fr, fg, fb, message, alphaString, font, offset, errorline, errorfile);
	}

	if(p_showClickables) PLEX_fillRectInt(0, x, y, w, h, fr, fg, fb, fa * 0.5, offset, errorline, errorfile);

	return false;
}

bool PLEX_drawIconButtonInt(const size_t order, const bool selected, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const uint8_t sr, const uint8_t sg, const uint8_t sb, const uint8_t sa, const uint8_t ovr, const int64_t ovb, const int64_t ovg, const int64_t ova, const uint8_t sovr, const int64_t sovb, const int64_t sovg, const int64_t sova, const uint8_t thickness, const size_t texture, const bool offset, const size_t errorline, const char* const errorfile)
{
	const bool inside = PLEX_mouseInside(x, y, w, h, offset);

	uint8_t dr = 0;
	uint8_t dg = 0;
	uint8_t db = 0;
	uint8_t da = 0;

	if(texture)
	{
		if(inside) PLEX_printTextureTintedInt(order, texture, tx, ty, tw, th, x, y, w, h, r, g, b, offset, errorline, errorfile);
		else PLEX_printTextureInt(order, texture, tx, ty, tw, th, x, y, w, h, offset, errorline, errorfile);
	}

	if(selected)
	{
		if(inside){ dr = sovr; dg = sovg; db = sovb; da = sova; }
		else { dr = sr; dg = sg; db = sb; da = sa; };
	}else{
		if(inside){ dr = ovr; dg = ovg; db = ovb; da = ova; }
		else{ dr = r; dg = g; db = b; da = a; };
	}

	PLEX_drawRectInt(order, x, y, w, h, dr, dg, db, da, thickness, offset, errorline, errorfile);

	if(p_showClickables) PLEX_fillRectInt(0, x, y, w, h, dr, dg, db, da * 0.5, offset, errorline, errorfile);

	return PLEX_leftClick() && inside;
}

bool PLEX_twoImageButtonInt(const size_t order, const bool selected, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const int64_t tx, const int64_t ty, const int64_t tw, const int64_t th, const int64_t tx2, const int64_t ty2, const int64_t tw2, const int64_t th2, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const uint8_t sr, const uint8_t sg, const uint8_t sb, const uint8_t sa, const uint8_t thickness, const size_t texture1, const size_t texture2, const bool offset, const size_t errorline, const char* const errorfile)
{
	const bool inside = PLEX_mouseInside(x, y, w, h, offset);

	uint8_t dr = 0;
	uint8_t dg = 0;
	uint8_t db = 0;
	uint8_t da = 0;

	if(!texture1)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GRAPHICS, PLEX_ERROR_ISNONTEXTURE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_GRAPHICS_ONTWOIMAGEBUTTONDDRAWING, PLEX_ERRORMESS_TEXTURE_TEXTURE);
		return false;
	}

	if(!texture2)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GRAPHICS, PLEX_ERROR_ISNONTEXTURE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_GRAPHICS_ONTWOIMAGEBUTTONDDRAWING,  PLEX_ERRORMESS_TEXTURE_TEXTURE2);
		return false;
	}

	if(inside) PLEX_printTextureInt(order, texture2, tx2, ty2, tw2, th2, x, y, w, h, offset, errorline, errorfile);
	else PLEX_printTextureInt(order, texture1, tx, ty, tw, th, x, y, w, h, offset, errorline, errorfile);

	if(selected)
	{
		dr = sr;
		dg = sg;
		db = sb;
		da = sa;
	}else{
		dr = r;
		dg = g;
		db = b;
		da = a;
	}

	PLEX_drawRectInt(order, x, y, w, h, dr, dg, db, da, thickness, offset, errorline, errorfile);

	if(p_showClickables) PLEX_fillRectInt(0, x, y, w, h, dr, dg, db, da * 0.5, offset, errorline, errorfile);

	return PLEX_leftClick() && inside;
}

void PLEX_drawChargeBarInt(const size_t order, const double fullness, const int64_t x, const int64_t y, const int64_t w, const int64_t h, const uint8_t fr, const uint8_t fg, const uint8_t fb, const uint8_t fa, const uint8_t br, const uint8_t bg, const uint8_t bb, const uint8_t ba, const uint8_t thickness, const bool vertical, const bool offset, const size_t errorline, const char* const errorfile)
{
	const double trueFullness = fullness < 1 ? fullness : 1;

	if(fullness > 0)
	{
		if(vertical)
		{
			const double revFullness = 1 - trueFullness;

			PLEX_fillRectInt(order, x, y + (h * revFullness), w, h - (h * revFullness), br, bg, bb, ba, offset, errorline, errorfile);
			PLEX_drawRectInt(order, x, y + (h * revFullness), w, h - (h * revFullness), fr, fg, fb, fa, thickness, offset, errorline, errorfile);
		}else{
			PLEX_fillRectInt(order, x, y, w * trueFullness, h, br, bg, bb, ba, offset, errorline, errorfile);
			PLEX_drawRectInt(order, x, y, w * trueFullness, h, fr, fg, fb, fa, thickness, offset, errorline, errorfile);
		}
	}

	PLEX_drawRectInt(order, x, y, w, h, fr, fg, fb, fa, thickness, offset, errorline, errorfile);
}

void PLEX_drawPanoramaInt(const size_t order, const size_t texture, const size_t errorline, const char* const errorfile)
{
	const double panjust = 1.3333;

	uint32_t aw = 0, ow = 0;

	uint32_t tw = 0, th = 0, atw = 0, otw = 0;

	if(!texture)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_GRAPHICS, PLEX_ERROR_ISNONTEXTURE, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_GRAPHICS_ONPANORAMADRAWING, PLEX_ERRORMESS_TEXTURE_TEXTURE);
		return;
	}

	PLEX_textureWH(texture, &tw, &th);

	atw = th * panjust;
	aw = PLEX_WH(true) * panjust;

	if(atw > tw)
	{
		atw = tw;
		aw = PLEX_WW(true);
	}

	ow = (PLEX_WW(true) - aw)/2 + PLEX_WOFFSET();
	otw = (tw - atw)/2 + PLEX_WOFFSETPERC();

	if(ow > PLEX_WW(true))
	{
		ow = PLEX_WW(true);
		otw = tw;
	}

	PLEX_printTextureInt(order, texture, tw/2 - atw/2, 0, atw, th, PLEX_WW(true)/2  - aw/2, 0, aw, PLEX_WH(true), true, errorline, errorfile);

	PLEX_printTextureInt(order, texture, 0, 0, otw, th, 0, 0, ow + 1, PLEX_WH(true), false, errorline, errorfile);

	PLEX_printTextureInt(order, texture, tw - otw, 0, otw, th, PLEX_WW(false) - (ow + 1), 0, ow + 1, PLEX_WH(true), false, errorline, errorfile);
}

void PLEX_clearInt(const size_t order, const uint8_t r, const uint8_t g, const uint8_t b, const size_t errorline, const char* const errorfile)
{
	PLEX_clearAlphaInt(order, r, g, b, 255, errorline, errorfile);
}

void PLEX_clearAlphaInt(const size_t order, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const size_t errorline, const char* const errorfile)
{
	PLEX_GRAPHICS_INSTRUCT_CONTENTS contents;

	contents.clear.r = r;
	contents.clear.g = g;
	contents.clear.b = b;
	contents.clear.a = a;

	p_addDrawList(PLEX_GRAPHICS_INSTRUCT_TYPE_CLEAR, contents, order, true, errorline, errorfile); // offset is irrelevant
}

void PLEX_clearOffsetAreaInt(const size_t order, const uint8_t r, const uint8_t g, const uint8_t b, const size_t errorline, const char* const errorfile)
{
	PLEX_clearOffsetAreaAlphaInt(order, r, g, b, 255, errorline, errorfile);
}

void PLEX_clearOffsetAreaAlphaInt(const size_t order, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const size_t errorline, const char* const errorfile)
{
	PLEX_fillRectInt(order, 0, 0, PLEX_WOFFSET(), PLEX_WH(false), r, g, b, a, false, errorline, errorfile);
	PLEX_fillRectInt(order, PLEX_WW(false) - PLEX_WOFFSET(), 0, PLEX_WOFFSET(), PLEX_WH(false), r, g, b, a, false, errorline, errorfile);
	PLEX_fillRectInt(order, 0, 0, PLEX_WW(false), PLEX_HOFFSET(), r, g, b, a, false, errorline, errorfile);
	PLEX_fillRectInt(order, 0, PLEX_WH(false) - PLEX_HOFFSET(), PLEX_WW(false), PLEX_HOFFSET(), r, g, b, a, false, errorline, errorfile);
}

void PLEX_drawBlendMode(const SDL_BlendMode mode)
{
	PLEX_low_setBlendTarget(mode);
}

void PLEX_setRefreshColor(const uint8_t r, const uint8_t g, const uint8_t b)
{
	p_refreshR = r;
	p_refreshG = g;
	p_refreshB = b;
}

void PLEX_showAllTooltips(){ p_showTooltips = true; }
void PLEX_showTogglesTooltips(){ p_toggleTooltips = !p_toggleTooltips; }
void PLEX_showAllClickables(){ p_showClickables = true; }

size_t PLEX_takeScreencapInt(const bool offset, const size_t errorline, const char* const errorfile)
{
	return PLEX_copyTextureInt(p_screenPlate, offset * PLEX_WOFFSET(), offset * PLEX_HOFFSET(), PLEX_WW(offset), PLEX_WH(offset), errorline, errorfile);
}
