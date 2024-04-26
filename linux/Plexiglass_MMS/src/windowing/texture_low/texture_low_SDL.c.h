// this should obviously already have the texture_low header loaded before this is compiled

// ---------------------------- //
// PUBLIC FUNCTIONS:
// ---------------------------- //

PLEX_LOW_TEXTURE PLEX_low_generateTexture(const Uint32 format, const int access, const int w, const int h)
{
	return SDL_CreateTexture(PLEX_low_getRenderer(), format, access, w, h);
}

void PLEX_low_clearTexture(PLEX_LOW_TEXTURE texture)
{
	SDL_Texture* originalTarget = SDL_GetRenderTarget(PLEX_low_getRenderer());

	SDL_SetRenderTarget(PLEX_low_getRenderer(), texture);
	SDL_SetRenderDrawBlendMode(PLEX_low_getRenderer(), SDL_BLENDMODE_NONE);
	SDL_SetRenderDrawColor(PLEX_low_getRenderer(), 0, 0, 0, 0); // < alpha clear
	SDL_RenderFillRect(PLEX_low_getRenderer(), NULL);
	SDL_SetRenderTarget(PLEX_low_getRenderer(), originalTarget);
	SDL_SetRenderDrawBlendMode(PLEX_low_getRenderer(), SDL_BLENDMODE_BLEND);
}

void PLEX_low_setGeneratedTexture(PLEX_LOW_TEXTURE texture, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a)
{
	SDL_Texture* originalTarget = SDL_GetRenderTarget(PLEX_low_getRenderer());

	SDL_SetRenderTarget(PLEX_low_getRenderer(), texture);
	SDL_SetRenderDrawBlendMode(PLEX_low_getRenderer(), SDL_BLENDMODE_NONE);
	SDL_SetRenderDrawColor(PLEX_low_getRenderer(), r, g, b, a);
	SDL_RenderFillRect(PLEX_low_getRenderer(), NULL);
	SDL_SetRenderTarget(PLEX_low_getRenderer(), originalTarget);
	SDL_SetRenderDrawBlendMode(PLEX_low_getRenderer(), SDL_BLENDMODE_BLEND);

	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
}

void PLEX_low_textureFromSurface(SDL_Surface* surface, PLEX_LOW_TEXTURE* texture, const uint8_t a)
{
	PLEX_LOW_TEXTURE copy = PLEX_LOW_NOTEXTURE;

	SDL_Texture* originalTarget = NULL;

	PLEX_LOW_TEXTURE lTexture = *texture;

	if(surface == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_LOWTEXTURE_ONGETFROMSURFACE, PLEX_ERRORMESS_LOWGRAPHICS_SDLSURFACE);
		return;
	}

	copy = SDL_CreateTextureFromSurface(PLEX_low_getRenderer(), surface);

	if(copy == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_CREATE, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_LOWTEXTURE_ONGETFROMSURFACE, PLEX_ERRORMESS_TEXTURE_TEXTURECOPY);
		return;
	}

	originalTarget = SDL_GetRenderTarget(PLEX_low_getRenderer());

	// clear the texture to pure invisible
	SDL_SetRenderTarget(PLEX_low_getRenderer(), lTexture);
	SDL_SetRenderDrawBlendMode(PLEX_low_getRenderer(), SDL_BLENDMODE_NONE);
	SDL_SetRenderDrawColor(PLEX_low_getRenderer(), 0, 0, 0, 0);
	SDL_RenderFillRect(PLEX_low_getRenderer(), NULL);

	// copy the copytex loaded from file to new texture
	SDL_RenderCopy(PLEX_low_getRenderer(), copy, NULL, NULL);

	SDL_SetRenderTarget(PLEX_low_getRenderer(), originalTarget);

	SDL_DestroyTexture(copy);

	SDL_SetTextureBlendMode(lTexture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(lTexture, a);
	SDL_SetRenderDrawBlendMode(PLEX_low_getRenderer(), SDL_BLENDMODE_BLEND);

	*texture = lTexture;
}

void PLEX_low_destroyTexture(PLEX_LOW_TEXTURE* texture)
{
	if(*texture == PLEX_LOW_NOTEXTURE)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_FREENULL, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_TEXTURE_ONDESTROYTEXTURE, PLEX_ERRORMESS_TEXTURE_TEXTURE);
		return;
	}

	SDL_DestroyTexture(*texture);
}

void PLEX_low_textureOverTexture(PLEX_LOW_TEXTURE from, PLEX_LOW_TEXTURE to, SDL_Rect area, SDL_Rect toArea)
{
	SDL_Texture* originalTarget = SDL_GetRenderTarget(PLEX_low_getRenderer());

	SDL_SetTextureBlendMode(to, SDL_BLENDMODE_NONE);

	SDL_SetRenderTarget(PLEX_low_getRenderer(), to);

	SDL_RenderCopy(PLEX_low_getRenderer(), from, &area, &toArea);

	SDL_SetRenderTarget(PLEX_low_getRenderer(), originalTarget);
}

void PLEX_low_textureSetAlpha(PLEX_LOW_TEXTURE* toSet, const uint8_t alpha)
{
	SDL_SetTextureAlphaMod(*toSet, alpha);
}

void PLEX_low_textureWH(PLEX_LOW_TEXTURE texture, uint32_t* w, uint32_t* h)
{
	int tw = 0;
	int th = 0;

	if(texture == PLEX_LOW_NOTEXTURE)
	{
		*w = 0;
		*h = 0;
		return;
	}

	SDL_QueryTexture(texture, NULL, NULL, &tw, &th);
	*w = tw;
	*h = th;
}

SDL_BlendMode PLEX_low_getTextureBlendMode(PLEX_LOW_TEXTURE texture)
{
	SDL_BlendMode ret;

	if(texture == PLEX_LOW_NOTEXTURE)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_TEXTURE, PLEX_ERROR_ISNONTEXTURE, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_TEXTURE_ONGETBLENDMODE, PLEX_ERRORMESS_TEXTURE_TEXTURE);
		return SDL_BLENDMODE_NONE;
	}

	SDL_GetTextureBlendMode(texture, &ret);

	return ret;
}

bool PLEX_low_compTextures(const PLEX_LOW_TEXTURE a, const PLEX_LOW_TEXTURE b)
{
	return a == b;
}

bool PLEX_low_lessTextures(const PLEX_LOW_TEXTURE a, const PLEX_LOW_TEXTURE b)
{
	return a < b;
}

bool PLEX_low_isTextureNull(const PLEX_LOW_TEXTURE texture)
{
	return texture == NULL;
}
