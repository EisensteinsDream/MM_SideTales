#include "sound/sound.h"

typedef struct
{
	PaStream* stream;
	size_t needle, length, id, iterations, type, channelCount;
	int64_t buffer;
	bool on, loop;
	double volume, sampleRate;
	bool complexPattern[8];
	size_t complexPatternCount;
	void* playBuffer;
} PLEX_SOUNDCHANNEL;

static bool p_soundLock[PLEX_LIMIT_SOUNDCHANNELS];
static PLEX_SOUNDCHANNEL p_soundChannel[PLEX_LIMIT_SOUNDCHANNELS];

static PLEX_EXPANDARRAY* p_preloadBuffers[PLEX_LIMIT_PREBUFFERCOUNT];
static size_t p_preloadBufferFrameCount[PLEX_LIMIT_PREBUFFERCOUNT], p_preloadBufferSize[PLEX_LIMIT_PREBUFFERCOUNT], p_preloadBufferType[PLEX_LIMIT_PREBUFFERCOUNT], p_preloadBufferChannelCount[PLEX_LIMIT_PREBUFFERCOUNT];
static double p_preloadBufferSampleRate[PLEX_LIMIT_PREBUFFERCOUNT];

static size_t p_preloadBufferCount = 0;

static size_t p_universalThemeCueID = 0;
static size_t p_universalSoundEffect = 0;
static bool p_universalThemeStopSignal = false;

static bool p_running = false;

static size_t p_soundLockFail = 0;

static bool p_disabled = false;

static bool p_checkSoundLock(const size_t index)
{
	size_t breakoutAttempts = 0;

	while(p_soundLock[index])
	{
		++breakoutAttempts;

		if(breakoutAttempts >= PLEX_STANDNUM_SOUNDBREAKOUTATTEMPTS)
		{
			if(p_soundLockFail >= PLEX_STANDNUM_SOUNDLOCKFAIL) PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_BADOPERATION, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONSOUNDLOCKCHECK, PLEX_ERRORMESS_OP_BREAKOUTFAIL);

			++p_soundLockFail;

			return false;
		}

		Pa_Sleep(10);

		continue;
	}

	return true;
}

static bool p_checkError(const PLEX_ERROR_SEVERITY severity, const PaError error)
{
	if(error != paNoError)
	{
		PLEX_errorPortAudio(severity, error);
		return false;
	}

	return true;
}

static void p_restartChannelInfo(PLEX_SOUNDCHANNEL* channel)
{
	channel->needle = 0;
	channel->loop = false;
	channel->iterations = 0;

	// do not reset volume here

	memset(channel->complexPattern, true, sizeof(bool) * 8);
	channel->complexPatternCount = 0;

	channel->on = false;
}

static void p_killChannelStream(PLEX_SOUNDCHANNEL* channel)
{
	if(channel->stream != NULL)
	{
		#if(PLEX_CORE_OS == PLEX_CORE_WINDOWS)
			Pa_AbortStream(channel->stream);
		#else
			Pa_StopStream(channel->stream);
			Pa_CloseStream(channel->stream);
		#endif

		channel->stream = NULL;
	}
}

static size_t p_fileTypeSize(const size_t type) // < keep above hook
{
	switch(type)
	{
		case SF_FORMAT_PCM_S8: return 1;
		case SF_FORMAT_PCM_16: return 2;
		case SF_FORMAT_PCM_24: return 3;
		case SF_FORMAT_PCM_32: return 4;
		case SF_FORMAT_PCM_U8: return 1;

		case SF_FORMAT_FLOAT: return 4;
		case SF_FORMAT_DOUBLE: return 8;
	};

	return 0;
}

static int p_hook(const void* input, void* output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData)
{
	PLEX_SOUNDCHANNEL* const me = (PLEX_SOUNDCHANNEL*)userData;

	size_t typeSize = p_fileTypeSize(me->type);

	int8_t* iOut = (int8_t*)output;

	p_soundLock[me->id] = true;

	for(size_t ze = 0; ze < frameCount; ++ze)
	{
		const size_t index = ze * typeSize * me->channelCount;
		const size_t nindex = me->needle * typeSize * me->channelCount;

		#if(PLEX_CORE_OS == PLEX_CORE_WINDOWS)
			if(!me->on)
			{
                me->iterations = 0;
				memset(iOut + index, 0, typeSize);
				continue;
			}
		#endif

		p_soundLock[me->id] = true;

		if(me->buffer < 0)
		{
			PLEX_errorNoDesc(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_BADBUFFERSOUNDHOOK, PLEX_ERROR_SEVERITY_MAJOR);
			p_soundLock[me->id] = false;
			continue;
		}

		if(me->buffer >= PLEX_LIMIT_PREBUFFERCOUNT)
		{
			PLEX_errorNoDesc(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_BADBUFFERSOUNDHOOK, PLEX_ERROR_SEVERITY_MAJOR);
			p_soundLock[me->id] = false;
			continue;
		}

		if(me->needle >= me->length || !me->length)
		{
			if(me->length) ++me->iterations; // make sure this happens before it tries to continue

			if(me->loop){ me->needle = 0; }
			else
			{
				memset(iOut + index, 0, typeSize);
				me->on = false;
				continue;
			}
		}

		if((me->complexPatternCount && !me->complexPattern[me->iterations % me->complexPatternCount]))
		{
			memset(iOut + index, 0, typeSize);
			++me->needle;
			continue;
		}

		if(me->needle < me->length)
		{
			if(me->playBuffer == NULL) break;

			for(size_t ze = 0; ze < me->channelCount; ++ze)
			{
				const size_t frameAdjust = ze * typeSize;

				int8_t* frame = 0;

				int8_t mod8 = 0;
				int16_t mod16 = 0;
				int32_t mod24 = 0;
				int32_t mod32 = 0;
				uint8_t modu8 = 0;
				float modf = 0;
				double mod = 0;

				frame = (int8_t*)me->playBuffer + nindex + frameAdjust;

				switch(me->type)
				{
					case SF_FORMAT_PCM_S8: mod = *frame; break;
					case SF_FORMAT_PCM_16: mod = *(int16_t*)frame; break;
					case SF_FORMAT_PCM_24: memcpy(&mod24, frame, 3); mod = mod24; break;
					case SF_FORMAT_PCM_32: mod = *(int32_t*)frame; break;
					case SF_FORMAT_PCM_U8: mod = *frame; break;

					case SF_FORMAT_FLOAT: mod = *(float*)frame; break;
					case SF_FORMAT_DOUBLE: mod = *(double*)frame; break;
				};

				if(me->volume < 1) mod *= me->volume;

				// absolutely DO NOT use me->needle here
				switch(me->type)
				{
					case SF_FORMAT_PCM_S8: mod8 = mod; memcpy(iOut + index + frameAdjust, (char*)&mod8, sizeof(mod8)); break;
					case SF_FORMAT_PCM_16: mod16 = mod; memcpy(iOut + index + frameAdjust, (char*)&mod16, sizeof(mod16)); break;
					case SF_FORMAT_PCM_24: mod24 = mod; memcpy(iOut + index + frameAdjust, (char*)&mod24, 3); break;
					case SF_FORMAT_PCM_32: mod32 = mod; memcpy(iOut + index + frameAdjust, (char*)&mod32, sizeof(mod32)); break;
					case SF_FORMAT_PCM_U8: modu8 = mod; memcpy(iOut + index + frameAdjust, (char*)&modu8, sizeof(modu8)); break;

					case SF_FORMAT_FLOAT: modf = mod; memcpy(iOut + index, (char*)&modf, sizeof(modf)); break;
					case SF_FORMAT_DOUBLE: memcpy(iOut + index, (char*)&mod, sizeof(mod));  break;
				};
			}

			++me->needle;
		}
	}

	p_soundLock[me->id] = false;

	return 0;
}

static SNDFILE* p_getFile(const char* const fileloc, SF_INFO* info)
{
	return sf_open(fileloc, SFM_READ, info);
}

static size_t p_fileBufferLen(const SF_INFO info, const size_t byteSize)
{
	return info.frames * byteSize * info.channels;
}

static size_t p_fileTypeWav(const size_t format)
{
	if((SF_FORMAT_PCM_S8 & format) == SF_FORMAT_PCM_S8) return SF_FORMAT_PCM_S8;
	if((SF_FORMAT_PCM_16 & format) == SF_FORMAT_PCM_16) return SF_FORMAT_PCM_16;
	if((SF_FORMAT_PCM_24 & format) == SF_FORMAT_PCM_24) return SF_FORMAT_PCM_24;
	if((SF_FORMAT_PCM_32 & format) == SF_FORMAT_PCM_32) return SF_FORMAT_PCM_32;

	if((SF_FORMAT_PCM_U8 & format) == SF_FORMAT_PCM_U8) return SF_FORMAT_PCM_U8;

	if((SF_FORMAT_FLOAT & format) == SF_FORMAT_FLOAT) return SF_FORMAT_FLOAT;
	if((SF_FORMAT_DOUBLE & format) == SF_FORMAT_DOUBLE) return SF_FORMAT_DOUBLE;

	return 0;
}

static size_t p_fileTypeFormat(const size_t format)
{
	if((format & SF_FORMAT_WAV) == SF_FORMAT_WAV) return SF_FORMAT_WAV;
	if((format & SF_FORMAT_AIFF) == SF_FORMAT_AIFF) return SF_FORMAT_AIFF;
	if((format & SF_FORMAT_AU) == SF_FORMAT_AU) return SF_FORMAT_AU;
	if((format & SF_FORMAT_RAW) == SF_FORMAT_RAW) return SF_FORMAT_RAW;
	if((format & SF_FORMAT_PAF) == SF_FORMAT_PAF) return SF_FORMAT_PAF;
	if((format & SF_FORMAT_SVX) == SF_FORMAT_SVX) return SF_FORMAT_SVX;
	if((format & SF_FORMAT_NIST) == SF_FORMAT_NIST) return SF_FORMAT_NIST;
	if((format & SF_FORMAT_VOC) == SF_FORMAT_VOC) return SF_FORMAT_VOC;
	if((format & SF_FORMAT_IRCAM) == SF_FORMAT_IRCAM) return SF_FORMAT_IRCAM;
	if((format & SF_FORMAT_W64) == SF_FORMAT_W64) return SF_FORMAT_W64;
	if((format & SF_FORMAT_MAT4) == SF_FORMAT_MAT4) return SF_FORMAT_MAT4;
	if((format & SF_FORMAT_MAT5) == SF_FORMAT_MAT5) return SF_FORMAT_MAT5;
	if((format & SF_FORMAT_PVF) == SF_FORMAT_PVF) return SF_FORMAT_PVF;
	if((format & SF_FORMAT_XI) == SF_FORMAT_XI) return SF_FORMAT_XI;
	if((format & SF_FORMAT_HTK) == SF_FORMAT_HTK) return SF_FORMAT_HTK;
	if((format & SF_FORMAT_SDS) == SF_FORMAT_SDS) return SF_FORMAT_SDS;
	if((format & SF_FORMAT_AVR) == SF_FORMAT_AVR) return SF_FORMAT_AVR;
	if((format & SF_FORMAT_WAVEX) == SF_FORMAT_WAVEX) return SF_FORMAT_WAVEX;
	if((format & SF_FORMAT_SD2) == SF_FORMAT_SD2) return SF_FORMAT_SD2;
	if((format & SF_FORMAT_FLAC) == SF_FORMAT_FLAC) return SF_FORMAT_FLAC;
	if((format & SF_FORMAT_CAF) == SF_FORMAT_CAF) return SF_FORMAT_CAF;
	if((format & SF_FORMAT_WVE) == SF_FORMAT_WVE) return SF_FORMAT_WVE;
	if((format & SF_FORMAT_OGG) == SF_FORMAT_OGG) return SF_FORMAT_OGG;
	if((format & SF_FORMAT_MPC2K) == SF_FORMAT_MPC2K) return SF_FORMAT_MPC2K;
	if((format & SF_FORMAT_RF64) == SF_FORMAT_RF64) return SF_FORMAT_RF64;

	return 0;
}

// NOTE: THE PLEX FOR MMS ONLY TAKES IN WAV FILES. FUTURE VERSIONS OF PLEX WILL TAKE IN MORE FORMATS

static size_t p_fileType(const SF_INFO info)
{
	const size_t format = info.format;

	const size_t typeFormat = p_fileTypeFormat(format);

	if(typeFormat == SF_FORMAT_WAV) return p_fileTypeWav(format);

	return 0;
}

static void p_fileBufferLoad(SNDFILE* source, void* buffer, size_t* length, const size_t readLen, const SF_INFO info)
{
	const size_t typeFormat = p_fileTypeFormat(info.format);

	memset(buffer, 0, readLen);

	if(typeFormat != SF_FORMAT_WAV) return;

	sf_read_raw(source, buffer, readLen);

	*length = info.frames;
}

static bool p_openChannelStream(PLEX_SOUNDCHANNEL* channel, const size_t index)
{
	const size_t type = channel->type;

	size_t paType = 0;

	switch(type)
	{
		case SF_FORMAT_PCM_S8: paType = paInt8; break;
		case SF_FORMAT_PCM_16: paType = paInt16; break;
		case SF_FORMAT_PCM_24: paType = paInt24; break;
		case SF_FORMAT_PCM_32: paType = paInt32; break;

		case SF_FORMAT_PCM_U8: paType = paUInt8; break;

		case SF_FORMAT_FLOAT: paType = paFloat32; break;

		case SF_FORMAT_DOUBLE:
			PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_BADOPERATION, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONOPENSOUNDSTREAM, PLEX_ERRORMESS_OP_DOUBLESAMPLERATE);
		return false;

		default: return false;
	}

	if(!p_checkError(PLEX_ERROR_SEVERITY_MAJOR, Pa_OpenDefaultStream(&channel->stream, 0, channel->channelCount, paType, channel->sampleRate, 256, p_hook, p_soundChannel + index)))
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_PORTAUDIO, PLEX_ERROR_EXTERNALMODULE, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONOPENSOUNDSTREAM, PLEX_ERRORMESS_FAILEDTOOPENSOUNDSTREAM);
		return false;
	}

	return true;
}

static bool p_playChannelStream(PLEX_SOUNDCHANNEL* channel)
{
	if(Pa_IsStreamActive(channel->stream) && !Pa_IsStreamStopped(channel->stream)) return true;

	return p_checkError(PLEX_ERROR_SEVERITY_MAJOR, Pa_StartStream(channel->stream));
}

static void p_stopChannelStream(PLEX_SOUNDCHANNEL* channel)
{
	if(!Pa_IsStreamActive(channel->stream))
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_STREAMSTATE, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONSTOPSOUNDSTREAM, PLEX_ERRORMESS_STREAMISINACTIVE);
		return;
	}

	if(Pa_IsStreamStopped(channel->stream)) return;

	// the windows build is unusually slow with stopping streams
	#if (PLEX_CORE_OS != PLEX_CORE_WINDOWS)
		p_checkError(PLEX_ERROR_SEVERITY_MAJOR, Pa_StopStream(channel->stream));
	#endif

	channel->needle = 0;
}

// ---------------------------- //
// PUBLIC FUNCTIONS:
// ---------------------------- //

bool PLEX_preloadSoundFileToSpecificBufferInt(const char* const fileloc, const size_t buffer, const size_t line, const char* const filename)
{
	SF_INFO info;

	size_t bufferLen = 0;
	size_t frameLen = 0;
	size_t byteSize = 0;

	size_t type = 0;

	SNDFILE* file = NULL;

	if(PLEX_DEF_disableAudio()) return true;

	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONPRELOADINTOSPECIFICBUFFER);
		return false;
	}

	if(buffer >= PLEX_LIMIT_PREBUFFERCOUNT)
	{
		// sound over buffer here
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONPRELOADINTOSPECIFICBUFFER, PLEX_ERRORMESS_SOUNDBUFFER);
		return false;
	}

	file = p_getFile(fileloc, &info);

	if(file == NULL)
	{
		const PLEX_FAKESTRING_100 targMessage = PLEX_fileLoadErrorPath(fileloc, PLEX_ERRORMESS_SOUNDBUFFERFILE);

		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_FILELOAD, PLEX_ERROR_SEVERITY_MAJOR, PLEX_ERRORMESS_ONPRELOADINTOSPECIFICBUFFER, PLEX_fts(targMessage));
		return false;
	}

	type = p_fileType(info);

	if(!type)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_ZEROVALUE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONPRELOADINTOSPECIFICBUFFER, PLEX_ERRORMESS_SOUNDTYPE);
		sf_close(file);
		return false;
	}

	p_preloadBufferType[buffer] = type;
	byteSize = p_fileTypeSize(p_preloadBufferType[buffer]);
	bufferLen = p_fileBufferLen(info, byteSize);
	frameLen = info.frames * info.channels;

	if(p_preloadBuffers[buffer] == NULL || PLEX_expandArrayCount(p_preloadBuffers[buffer]) <= frameLen)
	{
		if(p_preloadBuffers[buffer] != NULL) PLEX_destroyExpandArray(p_preloadBuffers + buffer);
		p_preloadBuffers[buffer] = PLEX_genExpandArrayInt(frameLen, byteSize, line, filename);
	}

	if(p_preloadBuffers[buffer] == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_GETNULL, PLEX_ERROR_SEVERITY_MAJOR, PLEX_ERRORMESS_ONPRELOADINTOSPECIFICBUFFER, PLEX_ERRORMESS_PRELOADBUFFER);
		sf_close(file);
		return false;
	}

	p_preloadBufferSampleRate[buffer] = info.samplerate;
	p_preloadBufferChannelCount[buffer] = info.channels;

	p_fileBufferLoad(file, PLEX_expandArrayContents(p_preloadBuffers[buffer]), p_preloadBufferFrameCount + buffer, bufferLen, info);
	p_preloadBufferSize[buffer] = bufferLen;
	p_preloadBufferFrameCount[buffer] = frameLen;
	PLEX_clearExpandArray(p_preloadBuffers[buffer]);
	PLEX_multiIncrementExpandArrayInt(frameLen, p_preloadBuffers[buffer], line, filename);
	sf_close(file);

	return true;
}

int64_t PLEX_preloadSoundFileInt(const char* const fileloc, const size_t linenum, const char* const filename)
{
	size_t index = p_preloadBufferCount;

	if(PLEX_DEF_disableAudio()) return 0; // 0 instead of -1 to not indicate something is wrong

	if(!PLEX_preloadSoundFileToSpecificBufferInt(fileloc, index, linenum, filename))
	{
		PLEX_FAKESTRING_25 errMess2 = PLEX_createString25(PLEX_ERRORMESS_GENERIC_INDEX);
		PLEX_FAKESTRING_25 errMess2IndexNum = PLEX_createStringFromNumber25(index);
		PLEX_FAKESTRING_25 errMess2Filepath = PLEX_createString25(PLEX_ERRORMESS_GENERIC_FILEPATH);
		PLEX_FAKESTRING_25 errMess2Filename = PLEX_createString25(fileloc);

		errMess2 = PLEX_concatString25(errMess2, ':', 1);
		errMess2 = PLEX_concatString25(errMess2, ' ', 1);
		errMess2 = PLEX_mergeString25(errMess2, errMess2IndexNum);
		errMess2 = PLEX_concatString25(errMess2, ' ', 1);
		errMess2 = PLEX_mergeString25(errMess2, errMess2Filepath);
		errMess2 = PLEX_concatString25(errMess2, ':', 1);
		errMess2 = PLEX_concatString25(errMess2, ' ', 1);
		errMess2 = PLEX_mergeString25(errMess2, errMess2Filename);

		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_CREATE, PLEX_ERROR_SEVERITY_MAJOR, PLEX_ERRORMESS_ONPRELOADINTOBUFFER, PLEX_fts(errMess2));
		return -1;
	}

	++p_preloadBufferCount;

	return index;
}

bool PLEX_assignSoundBuffer(const int64_t buffer, const size_t index)
{
	PLEX_SOUNDCHANNEL* channel = NULL;

	if(PLEX_DEF_disableAudio()) return true;

	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONASSIGNBUFFER);
		return false;
	}

	if(index >= PLEX_LIMIT_SOUNDCHANNELS)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONASSIGNBUFFER, PLEX_ERRORMESS_SOUNDCHANNEL);
		return false;
	}

	if(buffer < 0)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_UNDERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONASSIGNBUFFER, PLEX_ERRORMESS_SOUNDBUFFER);
		return false;
	}

	if(buffer >= PLEX_LIMIT_PREBUFFERCOUNT)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONASSIGNBUFFER, PLEX_ERRORMESS_SOUNDBUFFER);
		return false;
	}

	if(!p_checkSoundLock(index))
	{
		PLEX_error(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_SOUNDLOCK, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONASSIGNBUFFER);
		return false;
	}

	if(PLEX_isSoundPlaying(index))
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_SOUNDCHANNELSTATE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONASSIGNBUFFER, PLEX_ERRORMESS_SOUNDCHANNELON);
		return false;
	}

	channel = p_soundChannel + index;

	p_restartChannelInfo(channel);

	channel->buffer = buffer;
	channel->iterations = 0;
	channel->needle = 0;

	if(channel->playBuffer != NULL) PLEX_dealloc(&channel->playBuffer);

	return true;
}

void PLEX_clearSoundBuffer(const int64_t buffer)
{
	if(PLEX_DEF_disableAudio()) return;

	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONCLEARBUFFER);
		return;
	}

	if(buffer >= PLEX_LIMIT_PREBUFFERCOUNT)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONCLEARBUFFER, PLEX_ERRORMESS_SOUNDBUFFER);
		return;
	}

	// sometimes buffers of channels are automatically cleared without being set, so this should remain a bad style as to not flood the output with pointless error messages.
	if(buffer < 0)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_UNDERLIMIT, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONCLEARBUFFER, PLEX_ERRORMESS_SOUNDBUFFER);
		return;
	}

	p_preloadBufferFrameCount[buffer] = 0;
	p_preloadBufferSize[buffer] = 0;
	PLEX_destroyExpandArray(p_preloadBuffers + buffer);

	if(buffer == p_preloadBufferCount - 1)
	{
		--p_preloadBufferCount;
		return;
	}
}

void PLEX_clearSoundBufferByChannel(const size_t index)
{
	PLEX_SOUNDCHANNEL* channel = NULL;

	if(PLEX_DEF_disableAudio()) return;

	if(index >= PLEX_LIMIT_SOUNDCHANNELS)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONCLEARBUFFERBYCHANNEL, PLEX_ERRORMESS_SOUNDCHANNEL);
		return;
	}

	channel = p_soundChannel + index;

	PLEX_clearSoundBuffer(channel->buffer);

	channel->buffer = -1;
}

bool PLEX_loadSoundFileIntoBufferInt(const char* const fileloc, const size_t index, const size_t line, const char* const filename)
{
	int64_t buffer = -1;

	if(PLEX_DEF_disableAudio()) return true;

	buffer = PLEX_preloadSoundFileInt(fileloc, line, filename);

	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONLOADSOUNDFILEINTOBUFFER);
		return false;
	}

	if(buffer < 0)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_UNDERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONLOADSOUNDFILEINTOBUFFER, PLEX_ERRORMESS_SOUNDBUFFER);
		return false;
	}

	PLEX_assignSoundBuffer(buffer, index);

	return true;
}

bool PLEX_setSoundBufferLen(const size_t length, const size_t index)
{
	PLEX_SOUNDCHANNEL* channel = NULL;

	if(PLEX_DEF_disableAudio()) return true;

	size_t byteSize = 0;

	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONSETTINGSOUNDBUFFERLEN);
		return false;
	}

	if(index >= PLEX_LIMIT_SOUNDCHANNELS)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONSETTINGSOUNDBUFFERLEN, PLEX_ERRORMESS_SOUNDCHANNEL);
		return false;
	}

	if(!p_checkSoundLock(index))
	{
		PLEX_error(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_SOUNDLOCK, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONSETTINGSOUNDBUFFERLEN);
		return false;
	}

	channel = p_soundChannel + index;

	p_restartChannelInfo(channel);

	byteSize = p_fileTypeSize(channel->type);

	channel->length = byteSize ? length/byteSize : 0;

	return true;
}

void PLEX_setComplexSoundSequence(const bool seq0, const bool seq1, const bool seq2, const bool seq3, const bool seq4, const bool seq5, const bool seq6, const bool seq7, const size_t count, const size_t index)
{
	PLEX_SOUNDCHANNEL* channel = NULL;

	if(PLEX_DEF_disableAudio()) return;

	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONSETTINGSOUNDCOMPLEXSEQUENCE);
		return;
	}

	if(index >= PLEX_LIMIT_SOUNDCHANNELS)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONSETTINGSOUNDCOMPLEXSEQUENCE, PLEX_ERRORMESS_SOUNDCHANNEL);
		return;
	}

	if(!count)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_ZEROVALUE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONSETTINGSOUNDCOMPLEXSEQUENCE, PLEX_ERRORMESS_SOUNDCHANNELSEQUENCE);
		return;
	}

	if(count > 8)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONSETTINGSOUNDCOMPLEXSEQUENCE, PLEX_ERRORMESS_SOUNDCHANNELSEQUENCE);
		return;
	}

	if(!p_checkSoundLock(index))
	{
		PLEX_error(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_SOUNDLOCK, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONSETTINGSOUNDCOMPLEXSEQUENCE);
		return;
	}

	channel = p_soundChannel + index;

	channel->complexPattern[0] = seq0;
	if(count > 1) channel->complexPattern[1] = seq1; else channel->complexPattern[1] = false;
	if(count > 2) channel->complexPattern[2] = seq2; else channel->complexPattern[2] = false;
	if(count > 3) channel->complexPattern[3] = seq3; else channel->complexPattern[3] = false;
	if(count > 4) channel->complexPattern[4] = seq4; else channel->complexPattern[4] = false;
	if(count > 5) channel->complexPattern[5] = seq5; else channel->complexPattern[5] = false;
	if(count > 6) channel->complexPattern[6] = seq6; else channel->complexPattern[6] = false;
	if(count > 7) channel->complexPattern[7] = seq7; else channel->complexPattern[7] = false;

	channel->complexPatternCount = count;
}

void PLEX_soundBufferRandomSwapInt(const int64_t buffer1, const int64_t buffer2, const double perc, const double perc2, const double lenPerc, const size_t linenum, const char* const filename)
{
	PLEX_EXPANDARRAY* preBuff1 = NULL;
	PLEX_EXPANDARRAY* preBuff2 = NULL;

	if(PLEX_DEF_disableAudio()) return;

	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONSOUNDBUFFERRANDOMSWAP);
		return;
	}

	if(buffer1 >= PLEX_LIMIT_PREBUFFERCOUNT)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONSOUNDBUFFERRANDOMSWAP, PLEX_ERRORMESS_SOUNDBUFFER);
		return;
	}

	if(buffer2 >= PLEX_LIMIT_PREBUFFERCOUNT)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONSOUNDBUFFERRANDOMSWAP, PLEX_ERRORMESS_SOUNDBUFFER2);
		return;
	}

	if(buffer1 < 0)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_UNDERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONSOUNDBUFFERRANDOMSWAP, PLEX_ERRORMESS_SOUNDBUFFER);
		return;
	}

	if(buffer2 < 0)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_UNDERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONSOUNDBUFFERRANDOMSWAP, PLEX_ERRORMESS_SOUNDBUFFER2);
		return;
	}

	preBuff1 = p_preloadBuffers[buffer1];

	if(preBuff1 == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONSOUNDBUFFERRANDOMSWAP, PLEX_ERRORMESS_PRELOADBUFFER);
		return;
	}

	preBuff2 = p_preloadBuffers[buffer2];

	if(preBuff2 == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONSOUNDBUFFERRANDOMSWAP, PLEX_ERRORMESS_PRELOADBUFFER2);
		return;
	}

	PLEX_swapExpandArrayPercInt(perc, perc2, lenPerc, preBuff1, preBuff2, linenum, filename);
}

void PLEX_soundBufferRearrangeInt(const int64_t buffer, const double perc, const double perc2, const double lenPerc, const size_t linenum, const char* const filename)
{
	PLEX_EXPANDARRAY* preBuff = NULL;

	if(PLEX_DEF_disableAudio()) return;

	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONSOUNDBUFFERREARRANGE);
		return;
	}

	if(buffer >= PLEX_LIMIT_PREBUFFERCOUNT)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONSOUNDBUFFERREARRANGE, PLEX_ERRORMESS_SOUNDBUFFER);
		return;
	}

	if(buffer < 0)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_UNDERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONSOUNDBUFFERREARRANGE, PLEX_ERRORMESS_SOUNDBUFFER);
		return;
	}

	preBuff = p_preloadBuffers[buffer];

	if(preBuff == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONSOUNDBUFFERREARRANGE, PLEX_ERRORMESS_PRELOADBUFFER);
		return;
	}

	PLEX_rearrangeExpandArrayPercInt(perc, perc2, lenPerc, preBuff, linenum, filename);
}

void PLEX_soundBufferCut(const int64_t buffer, const double perc, const double lenPerc)
{
	PLEX_EXPANDARRAY* preBuff = NULL;

	if(PLEX_DEF_disableAudio()) return;

	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONSOUNDBUFFERCUT);
		return;
	}

	if(buffer >= PLEX_LIMIT_PREBUFFERCOUNT)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONSOUNDBUFFERCUT, PLEX_ERRORMESS_SOUNDBUFFER);
		return;
	}

	if(buffer < 0)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_UNDERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONSOUNDBUFFERCUT, PLEX_ERRORMESS_SOUNDBUFFER);
		return;
	}

	preBuff = p_preloadBuffers[buffer];

	if(preBuff == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONSOUNDBUFFERCUT, PLEX_ERRORMESS_PRELOADBUFFER);
		return;
	}

	if(PLEX_cutExpandArrayPerc(perc, lenPerc, preBuff))
	{
		p_preloadBufferFrameCount[buffer] = PLEX_expandArrayCount(preBuff);
		p_preloadBufferSize[buffer] = PLEX_protoExpandArraySize(preBuff);
	}else{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_BADOPERATION, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONSOUNDBUFFERCUT, PLEX_ERRORMESS_OP_CUTSOUNDBUFFER);
	}
}

void PLEX_soundReverseInt(const int64_t buffer, const double perc, const double lenPerc, const size_t linenum, const char* const filename)
{
	PLEX_EXPANDARRAY* preBuff = NULL;

	if(PLEX_DEF_disableAudio()) return;

	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONSOUNDBUFFERREVERSE);
		return;
	}

	if(buffer >= PLEX_LIMIT_PREBUFFERCOUNT)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONSOUNDBUFFERREVERSE, PLEX_ERRORMESS_SOUNDBUFFER);
		return;
	}

	if(buffer < 0)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_UNDERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONSOUNDBUFFERREVERSE, PLEX_ERRORMESS_SOUNDBUFFER);
		return;
	}

	preBuff = p_preloadBuffers[buffer];

	if(preBuff == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONSOUNDBUFFERREVERSE, PLEX_ERRORMESS_PRELOADBUFFER);
		return;
	}

	if(PLEX_reverseExpandArrayPercInt(perc, lenPerc, preBuff, linenum, filename))
	{
		p_preloadBufferFrameCount[buffer] = PLEX_expandArrayCount(preBuff);
		p_preloadBufferSize[buffer] = PLEX_protoExpandArraySize(preBuff);
	}else{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_BADOPERATION, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONSOUNDBUFFERREVERSE, PLEX_ERRORMESS_OP_REVERSESOUNDBUFFER);
	}
}

void PLEX_setVolume(const double volume, const size_t index)
{
	PLEX_SOUNDCHANNEL* channel = NULL;

	if(PLEX_DEF_disableAudio()) return;

	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONSETTINGCHANNELVOLUME);
		return;
	}

	if(index >= PLEX_LIMIT_SOUNDCHANNELS)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONSETTINGCHANNELVOLUME, PLEX_ERRORMESS_SOUNDCHANNEL);
		return;
	}

	channel = p_soundChannel + index;

	if(channel == NULL)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_ISNULL, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONSETTINGCHANNELVOLUME, PLEX_ERRORMESS_SOUNDCHANNEL);
		return;
	}

	channel->volume = volume;
}

void PLEX_soundChannelOnInt(const bool loop, const size_t index, const size_t line, const char* const filename)
{
	PLEX_SOUNDCHANNEL* channel = NULL;

	size_t startLen = 0;
	size_t fullBufferLen = 0;

	if(PLEX_DEF_disableAudio()) return;

	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONTURNINGSOUNDCHANNELON);
		return;
	}

	if(index >= PLEX_LIMIT_SOUNDCHANNELS)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONTURNINGSOUNDCHANNELON, PLEX_ERRORMESS_SOUNDCHANNEL);
		return;
	}

	if(!p_checkSoundLock(index))
	{
		PLEX_error(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_SOUNDLOCK, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONTURNINGSOUNDCHANNELON);
		return;
	}

	channel = p_soundChannel + index;

	if(channel->on)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_SOUNDCHANNELSTATE, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONTURNINGSOUNDCHANNELON, PLEX_ERRORMESS_SOUNDCHANNELON);
		return;
	}

	if(channel->buffer < 0)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_UNDERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONTURNINGSOUNDCHANNELON, PLEX_ERRORMESS_SOUNDCHANNEL);
		return;
	}

	// DO NOT USE p_preloadBufferCount HERE!
	if(channel->buffer >= PLEX_LIMIT_PREBUFFERCOUNT)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONTURNINGSOUNDCHANNELON, PLEX_ERRORMESS_SOUNDCHANNEL);
		return;
	}

	startLen = channel->length;

	channel->on = true;
	channel->loop = loop;
	channel->needle = 0;
	channel->iterations = 0;
	channel->length = 0;

	// keep before play buffer stuff
	if(channel->stream == NULL || channel->type != p_preloadBufferType[channel->buffer] || channel->sampleRate != p_preloadBufferSampleRate[channel->buffer] || channel->channelCount != p_preloadBufferChannelCount[channel->buffer])
	{
		channel->type = p_preloadBufferType[channel->buffer];
		channel->sampleRate = p_preloadBufferSampleRate[channel->buffer];
		channel->channelCount = p_preloadBufferChannelCount[channel->buffer];

		p_killChannelStream(channel);
		p_openChannelStream(channel, index);
	}

	if(channel->buffer != -1 && p_preloadBuffers[channel->buffer] != NULL && PLEX_expandArrayContents(p_preloadBuffers[channel->buffer]) != NULL)
	{
		const size_t frameCount = p_preloadBufferFrameCount[channel->buffer];

		if(channel->playBuffer == NULL || (channel->playBuffer != NULL && startLen >= frameCount))
		{
			const size_t byteSize = p_fileTypeSize(channel->type);

			fullBufferLen = frameCount * byteSize * channel->channelCount;

			if(channel->playBuffer != NULL) PLEX_dealloc(&channel->playBuffer);
			channel->playBuffer = PLEX_allocInt(fullBufferLen + byteSize + 1, line, filename);

			if(channel->playBuffer == NULL)
			{
				PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_ALLOCFAIL, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONTURNINGSOUNDCHANNELON, PLEX_ERRORMESS_GENERIC_BUFFER);
				return;
			}
		}

		channel->length = frameCount; // < do not move to assignBuffer

		memset(channel->playBuffer, 0, fullBufferLen + 1);
		memcpy(channel->playBuffer, PLEX_expandArrayContents(p_preloadBuffers[channel->buffer]), fullBufferLen);

		if(!p_playChannelStream(channel))
		{
			PLEX_FAKESTRING_25 errMess2 = PLEX_createString25(PLEX_ERRORMESS_OP_PLAYSOUNDCHANNEL);
			PLEX_FAKESTRING_25 errMess2ChannelIndex = PLEX_createStringFromNumber25(index);

			errMess2 = PLEX_concatString25(errMess2, ' ', 1);
			errMess2 = PLEX_mergeString25(errMess2, errMess2ChannelIndex);

			PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_BADOPERATION, PLEX_ERROR_SEVERITY_MAJOR, PLEX_ERRORMESS_ONTURNINGSOUNDCHANNELON, PLEX_fts(errMess2));
			return;
		}
	}
}

void PLEX_soundChannelOff(const size_t index)
{
	PLEX_SOUNDCHANNEL* channel = NULL;

	if(PLEX_DEF_disableAudio()) return;

	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONTURNINGSOUNDCHANNELOFF);
		return;
	}

	if(index >= PLEX_LIMIT_SOUNDCHANNELS)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONTURNINGSOUNDCHANNELOFF, PLEX_ERRORMESS_SOUNDCHANNEL);
		return;
	}

	// do not check for sound lock, the hook should stop when the stream stops

	channel = p_soundChannel + index;

	channel->iterations = 0;
	channel->volume = 1;

	#if(PLEX_CORE_OS != PLEX_CORE_WINDOWS)
		// p_stopChannelStream(channel);
	#endif

	if(!channel->on)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_SOUNDCHANNELSTATE, PLEX_ERROR_SEVERITY_BADSTYLE, PLEX_ERRORMESS_ONTURNINGSOUNDCHANNELOFF, PLEX_ERRORMESS_SOUNDCHANNELOFF);
		return;
	}

	channel->on = false;
}

void PLEX_clearSoundPreloads()
{
	if(PLEX_DEF_disableAudio()) return;

	for(size_t ze = 0; ze < PLEX_LIMIT_SOUNDCHANNELS; ++ze)
	{
		if(PLEX_isSoundPlaying(ze)) PLEX_soundChannelOff(ze);
	}

	// DO NOT USE p_prelaodBufferCount HERE
	for(size_t ze = 0; ze < PLEX_LIMIT_PREBUFFERCOUNT; ++ze)
	{
		PLEX_clearSoundBuffer(ze);
	}

	p_preloadBufferCount = 0;
}

void PLEX_setUniversalSoundEffect(const size_t id)
{
	p_universalSoundEffect = id;
}

void PLEX_setUniversalThemeID(const size_t id)
{
	p_universalThemeCueID = id;
}

void PLEX_signalUniversalThemeStop()
{
	p_universalThemeStopSignal = true;
}

bool PLEX_startSound()
{
	if(PLEX_DEF_disableAudio())
	{
		p_disabled = true;
		return true;
	}

	p_disabled = false;

	if(p_running)
	{
		PLEX_errorNoDesc(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_DOUBLESTART, PLEX_ERROR_SEVERITY_BADSTYLE);
		return true;
	}

	if(!p_checkError(PLEX_ERROR_SEVERITY_FATAL, Pa_Initialize()))
	{
		PLEX_error(PLEX_ERROR_CATEGORY_PORTAUDIO, PLEX_ERROR_LOADEXTERNALMODULE, PLEX_ERROR_SEVERITY_FATAL, PLEX_ERRORMESS_ONSOUNDBUFFERREVERSE);
		return false;
	}

	for(size_t ze = 0; ze < PLEX_LIMIT_PREBUFFERCOUNT; ++ze) p_preloadBuffers[ze] = NULL;

	for(size_t ze = 0; ze < PLEX_LIMIT_SOUNDCHANNELS; ++ze)
	{
		PLEX_SOUNDCHANNEL* channel = p_soundChannel + ze;

		channel->on = false;
		channel->id = ze;
		channel->buffer = -1;
		channel->playBuffer = NULL;
		channel->stream = NULL;
		channel->volume = 1;

		p_restartChannelInfo(channel);

		p_soundLock[channel->id] = false;
	}

	p_preloadBufferCount = 0;

	p_soundLockFail = 0;

	p_running = true;

	return true;
}

bool PLEX_updateSound()
{
	if(PLEX_DEF_disableAudio()) return true;

	if(!p_running)
	{
		PLEX_errorNoDesc(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_UPDATEBEFORESTART, PLEX_ERROR_SEVERITY_MINOR);
		return false;
	}

	for(size_t ze = 0; ze < PLEX_LIMIT_SOUNDCHANNELS; ++ze)
	{
		PLEX_SOUNDCHANNEL* channel = p_soundChannel + ze;

		if(!channel->on) p_stopChannelStream(channel);
	}

	p_universalThemeCueID = 0;
	p_universalSoundEffect = 0;
	p_universalThemeStopSignal = false;

	Pa_Sleep(10);

	return true;
}

void PLEX_endSound()
{
	if(PLEX_DEF_disableAudio()) return;

	if(!p_running)
	{
		PLEX_errorNoDesc(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_ENDBEFORESTART, PLEX_ERROR_SEVERITY_BADSTYLE);
		return;
	}

	for(size_t ze = 0; ze < PLEX_LIMIT_SOUNDCHANNELS; ++ze)
	{
		PLEX_SOUNDCHANNEL* channel = p_soundChannel + ze;

		if(channel == NULL) continue;

		p_killChannelStream(channel);

		if(channel->playBuffer != NULL) PLEX_dealloc(&channel->playBuffer);
	}

	Pa_Terminate();

	for(size_t ze = 0; ze < PLEX_LIMIT_PREBUFFERCOUNT; ++ze)
	{
		PLEX_EXPANDARRAY* expandArray = p_preloadBuffers[ze];

		if(expandArray != NULL) PLEX_destroyExpandArray(&expandArray);

		p_preloadBufferFrameCount[ze] = 0;
		p_preloadBufferSize[ze] = 0;
	}

	p_running = false;
}

int64_t PLEX_getBuffer(const size_t index)
{
	PLEX_SOUNDCHANNEL* channel = NULL;

	if(PLEX_DEF_disableAudio()) return 0; // 0 as to indicate nothing is wrong instead of -1

	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONGETTINGSOUNDBUFFER);
		return -1;
	}

	if(index >= PLEX_LIMIT_SOUNDCHANNELS)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONGETTINGSOUNDBUFFER, PLEX_ERRORMESS_SOUNDCHANNEL);
		return -1;
	}

	if(!p_checkSoundLock(index))
	{
		PLEX_error(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_SOUNDLOCK, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONGETTINGSOUNDBUFFER);
		return -1;
	}

	channel = p_soundChannel + index;

	return channel->buffer;
}

uint64_t PLEX_channelDuration(const size_t index)
{
	PLEX_SOUNDCHANNEL* channel = NULL;

	if(PLEX_DEF_disableAudio()) return 0;

	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONGETTINGSOUNDDURATION);
		return false;
	}

	if(index >= PLEX_LIMIT_SOUNDCHANNELS)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONGETTINGSOUNDDURATION, PLEX_ERRORMESS_SOUNDCHANNEL);
		return false;
	}

	if(!p_checkSoundLock(index))
	{
		PLEX_error(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_SOUNDLOCK, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONGETTINGSOUNDDURATION);
		return -1;
	}

	channel = p_soundChannel + index;

	return channel->sampleRate ? channel->length/(channel->sampleRate/1000.0) : 0;
}

size_t PLEX_channelIteration(const size_t index)
{
	PLEX_SOUNDCHANNEL* channel = NULL;

	if(PLEX_DEF_disableAudio()) return 0;

	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONGETTINGCHANNELITERATION);
		return 0;
	}

	if(index >= PLEX_LIMIT_SOUNDCHANNELS)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONGETTINGCHANNELITERATION, PLEX_ERRORMESS_SOUNDCHANNEL);
		return 0;
	}

	if(!p_checkSoundLock(index))
	{
		PLEX_error(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_SOUNDLOCK, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONGETTINGCHANNELITERATION);
		return 0;
	}

	channel = p_soundChannel + index;

	// if(!channel->on) return 0; << do not do

	return channel->iterations;
}

size_t PLEX_getUniversalThemeCueID(){ return p_universalThemeCueID; }
size_t PLEX_getUniversalSoundEffect(){ return p_universalSoundEffect; }
bool PLEX_getUniversalThemeStopSignal(){ return p_universalThemeStopSignal; }

bool PLEX_isSoundPlaying(const size_t index)
{
	PLEX_SOUNDCHANNEL* channel = NULL;

	if(PLEX_DEF_disableAudio()) return true;

	if(!p_running)
	{
		PLEX_error(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_ACTIONBEFORESTART, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONCHECKSOUNDPLAYING);
		return false;
	}

	if(index >= PLEX_LIMIT_SOUNDCHANNELS)
	{
		PLEX_error2(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_OVERLIMIT, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONCHECKSOUNDPLAYING, PLEX_ERRORMESS_SOUNDCHANNEL);
		return false;
	}

	if(!p_checkSoundLock(index))
	{
		PLEX_error(PLEX_ERROR_CATEGORY_SOUND, PLEX_ERROR_SOUNDLOCK, PLEX_ERROR_SEVERITY_MINOR, PLEX_ERRORMESS_ONTURNINGSOUNDCHANNELON);
		return true;
	}

	channel = p_soundChannel + index;

	return channel->on;
}
