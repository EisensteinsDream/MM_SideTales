#include "windowing/texture.h"

typedef struct
{
	PLEX_FAKESTRING_100 fileloc;
	uint32_t w, h;
	uint8_t r, g, b, a, tint;
} PLEX_TEXTURE_KEY_DATA;

typedef enum
{
	PLEX_TEXTURE_KEY_TYPE_BLANK = 0, // << this should always be 0
	PLEX_TEXTURE_KEY_TYPE_GENERATED = 1,
	PLEX_TEXTURE_KEY_TYPE_FILE = 2,
	PLEX_TEXTURE_KEY_TYPE_HEADERLESS = 3
} PLEX_TEXTURE_KEY_TYPE;

typedef struct
{
	PLEX_TEXTURE_KEY_TYPE type;
	PLEX_TEXTURE_KEY_DATA data;
} PLEX_TEXTURE_KEY;

typedef struct
{
	PLEX_TEXTURE_KEY key;
	PLEX_LOW_TEXTURE contents;
	size_t count;
	bool cleared;
} PLEX_TEXTURE;

static PLEX_EXPANDARRAY* p_texBank = NULL;

static bool p_running = false;

static PLEX_TEXTURE p_genBlankTexture()
{
	PLEX_TEXTURE ret;

	memset(&ret, 0, sizeof(ret));

	return ret;
}

// PLEX only deals in 32-bit true color images. This function ensures that images that do not fit that description are converted.
static SDL_Surface* p_loadImage(const char* const fileloc, const bool onlyNew)
{
	SDL_Surface* loadSurf = IMG_Load(fileloc);
	SDL_Surface* copySurf = NULL;

	if(loadSurf == NULL)
	{
		PLEX_errorSDLImage(PLEX_ERROR_SEVERITY_MAJOR);
		return NULL;
	}

	copySurf = SDL_CreateRGBSurface(0, loadSurf->w, loadSurf->h, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);

	if(copySurf == NULL)
	{
		SDL_FreeSurface(loadSurf);
		PLEX_error2(PLEX_ERROR_CATEGORY_SDL, PLEX_ERROR_CREATE, PLEX_ERROR_SEVERITY_MAJOR, PLEX_ERRORMESS_TEXTURE_ONLOADTEXTUREFROMFILE, PLEX_ERRORMESS_LOWGRAPHICS_SDLSURFACE);
		return NULL;
	}

	SDL_BlitSurface(loadSurf, NULL, copySurf, NULL);

	SDL_FreeSurface(loadSurf);

	return copySurf;
}

static void p_tintSurface(SDL_Surface* toTint, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const double tint)
{
	uint8_t* pixels = (uint8_t*)toTint->pixels;

	size_t pixCount = toTint->pitch * toTint->h;

	size_t inc = toTint->pitch/toTint->w;

	SDL_LockSurface(toTint);

	for(size_t ze = 0; ze < pixCount; ze += inc)
	{
		Uint8 cr, cg, cb, ca, nr, ng, nb, na;

		Uint32 nextCol = 0;

		memcpy(&nextCol, pixels + ze, inc);

		SDL_GetRGBA(nextCol, toTint->format, &cr, &cg, &cb, &ca);

		if(ca)
		{
			nr = (r * tint) + (cr * (1 - tint));
			ng = (g * tint) + (cg * (1 - tint));
			nb = (b * tint) + (cb * (1 - tint));
			na = a;

			nextCol = SDL_MapRGBA(toTint->format, nr, ng, nb, na);
		}

		memcpy(pixels + ze, &nextCol, inc);
	}

	SDL_UnlockSurface(toTint);
}

static void p_dumpHeaderlessSurface(SDL_Surface* toDump, FILE* file)
{
	const uint32_t w = toDump->w;
	const uint32_t h = toDump->h;

	uint8_t* pixels = (uint8_t*)toDump->pixels;

	uint32_t pixCount = toDump->pitch * toDump->h;

	SDL_LockSurface(toDump);

	fwrite(&w, sizeof(uint32_t), 1, file);
	fwrite(&h, sizeof(uint32_t), 1, file);

	fwrite(pixels, pixCount, 1, file);

	SDL_UnlockSurface(toDump);
}

static bool p_genProcess(PLEX_TEXTURE* target, const PLEX_TEXTURE_KEY_TYPE type, const char* const fileloc, const uint32_t w, const uint32_t h, const uint8_t tint, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a)
{
	uint32_t aw = w;
	uint32_t ah = h;

	const SDL_BlendMode startBlendMode = PLEX_low_getRenderBlendMode();

	const uint32_t limit = PLEX_DEF_textureDimensLimit();

	if(target == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_ISNONTEXTURE, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_LOWTEXTURE_ONGENTEXTURE, PLEX_ERRORMESS_TEXTURE_TEXTURE);
		return false;
	}

	if(aw > ah && aw > limit)
	{
		const double ratio = ah/(double)aw;

		aw = limit;
		ah = limit * ratio;
	}

	if(ah > limit)
	{
		const double ratio = aw/(double)ah;

		aw = limit * ratio;
		ah = limit;
	}

	target->contents = PLEX_low_generateTexture(SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, aw, ah);

	if(type != PLEX_TEXTURE_KEY_TYPE_HEADERLESS && PLEX_low_isTextureNull(target->contents))
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_CREATE, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_LOWTEXTURE_ONGENTEXTURE, PLEX_ERRORMESS_LOWTEXTURE_INNERTEXTURE);
		return false;
	}

	if(type == PLEX_TEXTURE_KEY_TYPE_GENERATED)
	{
		PLEX_low_setGeneratedTexture(target->contents, r, g, b, a);
	}else{
		if(type == PLEX_TEXTURE_KEY_TYPE_FILE)
		{
			SDL_Surface* copySurf = p_loadImage(fileloc, false);

			if(copySurf == NULL)
			{
				const PLEX_FAKESTRING_100 targMessage = PLEX_fileLoadErrorPath(fileloc, PLEX_ERRORMESS_LOWGRAPHICS_SDLSURFACE);

				PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_FILELOAD, PLEX_ERROR_SEVERITY_MAJOR, PLEX_ERRORMESS_LOWTEXTURE_ONPROCESSGENTEXTURE, PLEX_fts(targMessage));
				return false;
			}

			if(tint) p_tintSurface(copySurf, r, g, b, a, tint/255.0);

			PLEX_low_textureFromSurface(copySurf, &target->contents, a);

			SDL_FreeSurface(copySurf);
		}else{
			uint32_t w = 0;
			uint32_t h = 0;

			uint64_t size = 0;

			uint8_t* pixels = NULL;

			FILE* file = fopen(fileloc, "rb");

			SDL_Surface* copySurf = NULL;

			if(file == NULL)
			{
				const PLEX_FAKESTRING_100 targMessage = PLEX_fileLoadErrorPath(fileloc, PLEX_ERRORMESS_LOWGRAPHICS_SDLSURFACE);

				PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_FILELOAD, PLEX_ERROR_SEVERITY_MAJOR, PLEX_ERRORMESS_LOWTEXTURE_ONPROCESSGENTEXTURE, PLEX_fts(targMessage));
				return false;
			}

			fread(&w, sizeof(uint32_t), 1, file);
			fread(&h, sizeof(uint32_t), 1, file);

			copySurf = SDL_CreateRGBSurface(0, w, h, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);

			size = w * h * sizeof(uint32_t);

			if(copySurf == NULL)
			{
				PLEX_error2(PLEX_ERROR_CATEGORY_SDL, PLEX_ERROR_CREATE, PLEX_ERROR_SEVERITY_MAJOR, PLEX_ERRORMESS_LOWTEXTURE_ONPROCESSGENTEXTURE, PLEX_ERRORMESS_LOWGRAPHICS_SDLSURFACE);
				fclose(file);
				return false;
			}

			pixels = (uint8_t*)copySurf->pixels;

			SDL_LockSurface(copySurf);

			fread(pixels, size, 1, file);

			target->contents = PLEX_low_generateTexture(SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, w, h);

			SDL_UnlockSurface(copySurf);

			fclose(file);

			PLEX_low_textureFromSurface(copySurf, &target->contents, a);

			SDL_FreeSurface(copySurf);
		}
	}

	PLEX_low_setBlendTarget(startBlendMode);

	return true;
}

static size_t p_genTexture(const PLEX_TEXTURE_KEY_TYPE type, const char* const fileloc, const uint32_t w, const uint32_t h, const uint8_t tint, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const size_t linenum, const char* const filename)
{
	const size_t texBankCount = PLEX_expandArrayCount(p_texBank);

	PLEX_TEXTURE base;

	PLEX_TEXTURE* target = NULL;

	size_t index = PLEX_expandArrayCount(p_texBank);

	index = texBankCount;

	memset(&base, 0, sizeof(base));

	if(type == PLEX_TEXTURE_KEY_TYPE_BLANK) return 0;

	base.key.type = type;
	base.key.data.fileloc = PLEX_createString100(fileloc);
	base.key.data.w = w;
	base.key.data.h = h;
	base.key.data.tint = tint;
	base.key.data.r = r;
	base.key.data.g = g;
	base.key.data.b = b;
	base.key.data.a = a;
	base.cleared = false;

	// searches to see if identical texture already exists, returns that if so. Does not do this with generated textures.

	if(type != PLEX_TEXTURE_KEY_TYPE_GENERATED)
	{
		for(size_t ze = 1; ze < texBankCount; ++ze)
		{
			target = PLEX_getExpandArrayEntry(ze, p_texBank);

			if(target->cleared) continue;

			if(target->count)
			{
				if(!memcmp(&base.key, &target->key, sizeof(PLEX_TEXTURE_KEY)))
				{
					++(target->count);
					return ze; // < remember 0 == true with this function
				}
			}else{
				PLEX_low_destroyTexture(&target->contents);
				target->contents = PLEX_LOW_NOTEXTURE;
				index = ze;
				continue;
			}
		}
	}

	if(index == texBankCount)
	{
		if(!PLEX_incrementExpandArrayInt(p_texBank, linenum, filename))
		{
			PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_INCREMENT, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_TEXTURE_ONCREATEGENTEXTURE, PLEX_ERRORMESS_TEXTURE_TEXTUREBANK);
			return 0;
		}
	}

	target = PLEX_getExpandArrayEntry(index, p_texBank);
	*target = base;

	target->count = 1;

	if(!p_genProcess(target, type, fileloc, w, h, tint, r, g, b, a)) return 0;

	return index;
}

// ---------------------------- //
// PUBLIC FUNCTIONS:
// ---------------------------- //

size_t PLEX_createGeneratedTextureInt(const uint32_t w, const uint32_t h, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const size_t linenum, const char* const filename)
{
	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_TEXTURE_ONCREATEGENTEXTURE);
		return 0;
	}

	if(p_texBank == NULL)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_TEXTURE_ONCREATEGENTEXTURE);
		return 0;
	}

	return p_genTexture(PLEX_TEXTURE_KEY_TYPE_GENERATED, "\0", w, h, a, r, g, b, a, linenum, filename);
}

size_t PLEX_loadTextureFromFileInt(const uint32_t w, const uint32_t h, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const double tint, const char* const fileloc, const size_t linenum, const char* const filename)
{
	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_TEXTURE_ONLOADTEXTUREFROMFILE);
		return 0;
	}

	if(p_texBank == NULL)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_TEXTURE_ONLOADTEXTUREFROMFILE);
		return 0;
	}

	if(fileloc == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_LOWTEXTURE_ONGENTEXTURE, PLEX_ERRORMESS_GENERIC_FILEPATH);
		return 0;
	}

	return p_genTexture(PLEX_TEXTURE_KEY_TYPE_FILE, fileloc, w, h, tint * 255, r, g, b, a, linenum, filename);
}

bool PLEX_convToHeaderlessImage(const char* const filelocFrom, const char* const filelocTo, const size_t linenum, const char* const filename)
{
	FILE* to = NULL;

	SDL_Surface* from = NULL;

	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_TEXTURE_ONCONVHEADERLESSIMAGE);
		return false;
	}

	if(filelocFrom == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_TEXTURE_ONCONVHEADERLESSIMAGE, PLEX_ERRORMESS_GENERIC_FILEPATH);
		return false;
	}

	if(filelocTo == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_TEXTURE_ONCONVHEADERLESSIMAGE, PLEX_ERRORMESS_GENERIC_FILEPATH2);
		return false;
	}

	from = p_loadImage(filelocFrom, true);

	if(from == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_CREATE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_TEXTURE_ONCONVHEADERLESSIMAGE, PLEX_ERRORMESS_TEXTURE_TEXTURE);
		return false;
	}

	to = fopen(filelocTo, "wb+");

	p_dumpHeaderlessSurface(from, to);

	SDL_FreeSurface(from);

	if(to == NULL)
	{
		SDL_FreeSurface(from);
		PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_CREATE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_TEXTURE_ONCONVHEADERLESSIMAGE, PLEX_ERRORMESS_TEXTURE_TEXTURE2);
		return false;
	}

	fclose(to);

	return true;
}

size_t PLEX_loadTextureFromHeaderlessInt(const char* const fileloc, const size_t linenum, const char* const filename)
{
	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_TEXTURE_LOADTEXTUREFROMHEADERLESS);
		return 0;
	}

	if(fileloc == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_TEXTURE_LOADTEXTUREFROMHEADERLESS, PLEX_ERRORMESS_GENERIC_FILEPATH);
		return 0;
	}

	return p_genTexture(PLEX_TEXTURE_KEY_TYPE_HEADERLESS, fileloc, 0, 0, 0, 0, 0, 0, 255, linenum, filename);
}

size_t PLEX_loadTextureFromFileOrHeaderlessInt(const uint32_t w, const uint32_t h, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a, const double tint, const char* const fileloc, const char* const headerlessLoc, const size_t linenum, const char* const filename)
{
	FILE* headerlessFile = NULL;

	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_TEXTURE_LOADTEXTUREFROMFILEORHEADERLESS);
		return 0;
	}

	if(fileloc == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_TEXTURE_LOADTEXTUREFROMFILEORHEADERLESS, PLEX_ERRORMESS_GENERIC_FILEPATH);
		return 0;
	}

	if(headerlessLoc == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_TEXTURE_LOADTEXTUREFROMFILEORHEADERLESS, PLEX_ERRORMESS_GENERIC_FILEPATH2);
		return 0;
	}

	headerlessFile = fopen(headerlessLoc, "rb");

	if(headerlessFile != NULL)
	{
		fclose(headerlessFile);

		return PLEX_loadTextureFromHeaderless(headerlessLoc);
	}else{
		// check for default that enables/disables dumping headerless images

		SDL_Surface* toDump = NULL;

		if(!PLEX_DEF_heaaderlessCopy()) return PLEX_loadTextureFromFileInt(w, h, r, g, b, a, tint, fileloc, linenum, filename);

		headerlessFile = fopen(headerlessLoc, "wb+");

		if(headerlessFile != NULL)
		{
			toDump = p_loadImage(fileloc, true);

			if(tint) p_tintSurface(toDump, r, g, b, a, tint);

			p_dumpHeaderlessSurface(toDump, headerlessFile);

			fclose(headerlessFile);

			SDL_FreeSurface(toDump);

			return PLEX_loadTextureFromHeaderless(headerlessLoc);
		}else{

			const PLEX_FAKESTRING_100 targMessage = PLEX_fileLoadErrorPath(fileloc, PLEX_ERRORMESS_LOWGRAPHICS_SDLSURFACE);

			PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_FILELOAD, PLEX_ERROR_SEVERITY_MAJOR, PLEX_ERRORMESS_TEXTURE_LOADTEXTUREFROMFILEORHEADERLESS, PLEX_fts(targMessage));

			/* do not return */
		}
	}

	return PLEX_loadTextureFromFileInt(w, h, r, g, b, a, tint, fileloc, linenum, filename);
}

size_t PLEX_copyTextureInt(const size_t from, const uint32_t cx, const uint32_t cy, const uint32_t cw, const uint32_t ch, const size_t linenum, const char* const filename)
{
	size_t ret = 0;

	size_t texBankCount = 0;

	SDL_Rect area;
	SDL_Rect toArea;

	PLEX_TEXTURE* copied = NULL;

	PLEX_LOW_TEXTURE copiedInnerTexture = PLEX_LOW_NOTEXTURE;
	PLEX_LOW_TEXTURE innerTexture = PLEX_LOW_NOTEXTURE;

	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_TEXTURE_ONCOPYTEXTURE);
		return 0;
	}

	if(p_texBank == NULL)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_TEXTURE_ONCOPYTEXTURE);
		return 0;
	}

	if(!from)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_ZEROVALUE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_TEXTURE_ONCOPYTEXTURE, PLEX_ERRORMESS_TEXTURE_FROMTEXTURE);
		return 0;
	}

	texBankCount = PLEX_expandArrayCount(p_texBank);

	if(from >= texBankCount)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_TEXTURE_ONCOPYTEXTURE, PLEX_ERRORMESS_TEXTURE_FROMTEXTURE);
		return false;
	}

	copied = PLEX_getExpandArrayEntry(from, p_texBank);

	ret = PLEX_createGeneratedTexture(cw, ch, 0, 0, 0, 255);

	area.x = cx;
	area.y = cy;
	area.w = cw;
	area.h = ch;

	toArea.x = 0;
	toArea.y = 0;
	toArea.w = cw;
	toArea.h = ch;

	copiedInnerTexture = copied->contents;
	innerTexture = PLEX_innerTexture(ret);

	PLEX_low_textureOverTexture(copiedInnerTexture, innerTexture, area, toArea); // < don't use PLEX_innerTexture for the target pointer

	return ret;
}

void PLEX_destroyTexture(const size_t index)
{
	PLEX_TEXTURE* target = NULL;

	size_t texBankCount = 0;

	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_TEXTURE_ONDESTROYTEXTURE);
		return;
	}

	if(p_texBank == NULL)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MAJOR, PLEX_ERRORMESS_TEXTURE_ONDESTROYTEXTURE);
		return;
	}

	texBankCount = PLEX_expandArrayCount(p_texBank);

	if(!index)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_ISNONTEXTURE, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_TEXTURE_ONDESTROYTEXTURE, PLEX_ERRORMESS_TEXTURE_TEXTURE);
		return;
	}

	if(index >= texBankCount)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_TEXTURE_ONDESTROYTEXTURE, PLEX_ERRORMESS_GENERIC_INDEX);
		return;
	}

	target = PLEX_getExpandArrayEntry(index, p_texBank);

	if(target == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_ACQUIRE, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_TEXTURE_LOADTEXTUREFROMHEADERLESS, PLEX_ERRORMESS_TEXTURE_TEXTUREBANK);
		return;
	}

	if(!target->count)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_FREENULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_TEXTURE_ONDESTROYTEXTURE, PLEX_ERRORMESS_TEXTURE_TEXTURE);
		return;
	}

	--(target->count);
}

void PLEX_clearTexture(const size_t index)
{
	PLEX_TEXTURE* target = NULL;

	size_t texBankCount = 0;

	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_TEXTURE_ONCLEARTEXTURE);
		return;
	}

	if(p_texBank == NULL)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_TEXTURE_ONCLEARTEXTURE);
		return;
	}

	texBankCount = PLEX_expandArrayCount(p_texBank);

	if(index >= texBankCount)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_TEXTURE_ONCLEARTEXTURE, PLEX_ERRORMESS_GENERIC_INDEX);
		return;
	}

	target = PLEX_getExpandArrayEntry(index, p_texBank);

	if(target->cleared)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_BADOPERATION, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_TEXTURE_ONCLEARTEXTURE, PLEX_ERRORMESS_OP_CLEARCLEAREDTEXTURE);

		return;
	}

	target->cleared = true;
}

void PLEX_unclearTexture(const size_t index)
{
	PLEX_TEXTURE* target = NULL;

	size_t texBankCount = 0;

	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_TEXTURE_ONUNCLEARTEXTURE);
		return;
	}

	if(p_texBank == NULL)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_TEXTURE_ONUNCLEARTEXTURE);
		return;
	}

	texBankCount = PLEX_expandArrayCount(p_texBank);

	if(index >= texBankCount)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_TEXTURE_ONUNCLEARTEXTURE, PLEX_ERRORMESS_GENERIC_INDEX);
		return;
	}

	target = PLEX_getExpandArrayEntry(index, p_texBank);

	if(!target->cleared)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_BADOPERATION, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_TEXTURE_ONUNCLEARTEXTURE, PLEX_ERRORMESS_OP_CLEARCLEAREDTEXTURE);
		return;
	}

	p_genProcess(target, target->key.type, PLEX_fts(target->key.data.fileloc), target->key.data.w, target->key.data.h, target->key.data.tint, target->key.data.r, target->key.data.g, target->key.data.b, target->key.data.a);

	target->cleared = false; // < must be at end of function
}

void PLEX_setTextureAlpha(const size_t index, const uint8_t a)
{
	PLEX_LOW_TEXTURE lowTex = PLEX_LOW_NOTEXTURE;

	size_t texBankCount = 0;

	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_TEXTURE_ONSETTEXTUREALPHA);
		return;
	}

	if(p_texBank == NULL)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_TEXTURE_ONSETTEXTUREALPHA);
		return;
	}

	texBankCount = PLEX_expandArrayCount(p_texBank);

	if(index >= texBankCount)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_TEXTURE_ONSETTEXTUREALPHA, PLEX_ERRORMESS_GENERIC_INDEX);
		return;
	}

	lowTex = PLEX_innerTexture(index);

	if(PLEX_low_isTextureNull(lowTex))
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_ISNONTEXTURE, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_TEXTURE_ONSETTEXTUREALPHA, PLEX_ERRORMESS_TEXTURE_TEXTURE);
		return;
	}

	PLEX_low_textureSetAlpha(&lowTex, a);
}

bool PLEX_startTextures()
{
	PLEX_TEXTURE* blankTexture = NULL;

	if(p_running)
	{
		PLEX_errorNoDesc(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_DOUBLESTART, PLEX_ERROR_SEVERITY_BADSTYLE);
		return true;
	}

	p_texBank = PLEX_genExpandArray(PLEX_getMemProfileTexturePreload(), sizeof(PLEX_TEXTURE));

	if(p_texBank == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_ALLOCFAIL, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_TEXTURE_ONSTARTTEXTURE, PLEX_ERRORMESS_TEXTURE_TEXTUREBANK);
		return false;
	}

	if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		SDL_QuitSubSystem(SDL_INIT_EVENTS);
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
		PLEX_error(PLEX_ERROR_CATEGORY_SDL, PLEX_ERROR_LOADEXTERNALMODULE, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_SDLMODULE_IMAGE);
		PLEX_errorSDL(PLEX_ERROR_SEVERITY_FATAL);
		return false;
	}

	if(!PLEX_incrementExpandArray(p_texBank))
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_INCREMENT, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_TEXTURE_ONSTARTTEXTURE, PLEX_ERRORMESS_TEXTURE_TEXTUREBANK);
		return false;
	}

	blankTexture = PLEX_getExpandArrayLast(p_texBank);

	*blankTexture = p_genBlankTexture();

	p_running = true;

	return true;
}

bool PLEX_updateTextures()
{
	if(!p_running)
	{
		PLEX_errorNoDesc(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_UPDATEBEFORESTART, PLEX_ERROR_SEVERITY_BADSTYLE);
		return false;
	}

	if(p_texBank == NULL)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_UPDATEBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_TEXTURE_ONUPDATETEXTURE);
		return false;
	}

	return true;
}

void PLEX_endTextures()
{
	size_t texBankCount = 0;

	if(!p_running)
	{
		PLEX_errorNoDesc(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_ENDBEFORESTART, PLEX_ERROR_SEVERITY_BADSTYLE);
		return;
	}

	if(p_texBank == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_ALLOCFAIL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_TEXTURE_ONENDTEXTURE, PLEX_ERRORMESS_TEXTURE_TEXTUREBANK);
		return;
	}

	texBankCount = PLEX_expandArrayCount(p_texBank);

	for(size_t ze = 1; ze < texBankCount; ++ze)
	{
		PLEX_TEXTURE* target = PLEX_getExpandArrayEntry(ze, p_texBank);
		PLEX_LOW_TEXTURE lowTarg = target->contents;

		if(!PLEX_low_isTextureNull(lowTarg)) PLEX_low_destroyTexture(&lowTarg);
	}

	if(p_texBank != NULL)
	{
		PLEX_destroyExpandArray(&p_texBank);
	}else{
		PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_FREENULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_TEXTURE_ONENDTEXTURE, PLEX_ERRORMESS_TEXTURE_TEXTUREBANK);
	}

	IMG_Quit();

	p_running = false;
}

size_t PLEX_nullTexture()
{
	return 0;
}

bool PLEX_textureIsNull(const size_t index)
{
	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_TEXTURE_ONNULLCHECK);
		return true;
	}

	// make sure to check low level comparisons

	if(p_texBank == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_TEXTURE_ONNULLCHECK, PLEX_ERRORMESS_TEXTURE_TEXTUREBANK);
		return true;
	}

	return !index;
}

void PLEX_textureWH(const size_t index, uint32_t* w, uint32_t* h)
{
	size_t texBankCount = 0;

	PLEX_TEXTURE* target = NULL;

	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_TEXTURE_ONGETWIDTHHEIGHT);
		return;
	}

	if(p_texBank == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_TEXTURE_ONGETWIDTHHEIGHT, PLEX_ERRORMESS_TEXTURE_TEXTUREBANK);
		return;
	}

	texBankCount = PLEX_expandArrayCount(p_texBank);

	if(index >= texBankCount)
	{
		*w = 0;
		*h = 0;
		PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_TEXTURE_ONGETWIDTHHEIGHT, PLEX_ERRORMESS_GENERIC_INDEX);
		return;
	}

	if(!index)
	{
		*w = 0;
		*h = 0;
		return;
	}

	target = PLEX_getExpandArrayEntry(index, p_texBank);

	PLEX_low_textureWH(target->contents, w, h);
}

PLEX_LOW_TEXTURE PLEX_innerTexture(const size_t index)
{
	size_t texBankCount = 0;

	PLEX_TEXTURE* target = NULL;

	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_TEXTURE_ONGETINNERTEXTURE);
		return PLEX_LOW_NOTEXTURE;
	}

	if(p_texBank == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_TEXTURE_ONGETINNERTEXTURE, PLEX_ERRORMESS_TEXTURE_TEXTUREBANK);
		return PLEX_LOW_NOTEXTURE;
	}

	texBankCount = PLEX_expandArrayCount(p_texBank);

	if(index >= texBankCount)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_TEXTURE_ONGETINNERTEXTURE, PLEX_ERRORMESS_GENERIC_INDEX);
		return PLEX_LOW_NOTEXTURE;
	}

	if(!index) return PLEX_LOW_NOTEXTURE;

	target = PLEX_getExpandArrayEntry(index, p_texBank);

	return target->contents;
}

bool PLEX_textureIsCleared(const size_t index)
{
	size_t texBankCount = 0;

	PLEX_TEXTURE* target = NULL;

	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_TEXTURE_ONGETCLEAR);
		return false;
	}

	if(p_texBank == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_TEXTURE_ONGETCLEAR, PLEX_ERRORMESS_TEXTURE_TEXTUREBANK);
		return false;
	}

	texBankCount = PLEX_expandArrayCount(p_texBank);

	if(index >= texBankCount)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_TEXTURE_ONGETCLEAR, PLEX_ERRORMESS_GENERIC_INDEX);
		return false;
	}

	if(!index) return false;

	target = PLEX_getExpandArrayEntry(index, p_texBank);

	return target->cleared;
}
