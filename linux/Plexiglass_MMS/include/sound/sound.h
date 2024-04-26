#ifndef PLEX_SOUND_H
#define PLEX_SOUND_H

#include "utility/defaults.h"
#include "utility/error.h"
#include "utility/math.h"
#include "utility/fakestrings.h"

#include "mem/expandarray.h"

// this unit manages PLEX's sound. PLEX's sound engine is very low-level and very powerful. It allows for fully programmable algorithmic sound based on portaudio.

// NOTE: At the moment this can only load samples in the '.wav' format at 44100 32bit-signed sample rate. This is mostly just done for conveinence because that's what I am using mostly at the moment. It's not an incredibly difficult modificaiton to fix this I just haven't gotten to it as of yet. It's definitely something I will cover when I aim to make this more moddable.

#define PLEX_soundChannelOn(loop, index) PLEX_soundChannelOnInt(loop, index, __LINE__, __FILE__)
#define PLEX_preloadSoundFileToSpecificBuffer(fileloc, buffer) PLEX_preloadSoundFileToSpecificBufferInt(fileloc, buffer, __LINE__, __FILE__)
#define PLEX_soundBufferRearrange(buffer, perc, perc2, lenPerc) PLEX_soundBufferRearrangeInt(buffer, perc, perc2, lenPerc, __LINE__, __FILE__)
#define PLEX_soundReverse(buffer, perc, lenPerc) PLEX_soundReverseInt(buffer, perc, lenPerc, __LINE__, __FILE__)
#define PLEX_soundBufferRandomSwap(buffer1, buffer2, perc, perc2, lenPerc) PLEX_soundBufferRandomSwapInt(buffer1, buffer2, perc, perc2, lenPerc, __LINE__, __FILE__)

#define PLEX_preloadSoundFile(fileloc) PLEX_preloadSoundFileInt(fileloc, __LINE__, __FILE__)

#ifdef __cplusplus
extern "C"
{
#endif
	// preloads sound file into index'd buffer. returns false if anything goes wrong.
	extern bool PLEX_preloadSoundFileToSpecificBufferInt(const char* const fileloc, const size_t buffer, const size_t line, const char* const filename);

	// preloads sound file into the last unused buffer
	extern int64_t PLEX_preloadSoundFileInt(const char* const fileloc, const size_t line, const char* const filename);

	// assigns buffer 'buffer' into channel 'index'. Returns false if anything goes wrong.
	extern bool PLEX_assignSoundBuffer(const int64_t buffer, const size_t index);

	// clears out a particular buffer by index
	extern void PLEX_clearSoundBuffer(const int64_t buffer);

	// clears out a particular channel's buffer
	extern void PLEX_clearSoundBufferByChannel(const size_t index);

	// loads buffer of fileloc directly into channel index
	extern bool PLEX_loadSoundFileIntoBuffer(const char* const fileloc, const size_t index);

	// sets the sound buffer at index's length. This is poorly tested and, therefore, experimental.
	extern bool PLEX_setSoundBufferLen(const size_t length, const size_t index);

	// sets a sound channels play/not play patterns. Holds up to 8. Essentially how this works, is that it creates a pattern the length of 'count' up to 8. The sequence repeats at the end of count. So every repetetion of the sound it basically plays on 'true's and does not play on 'false''s. This is also an experimental poorly tested feature that has shown problems.
	extern void PLEX_setComplexSoundSequence(const bool seq0, const bool seq1, const bool seq2, const bool seq3, const bool seq4, const bool seq5, const bool seq6, const bool seq7, const size_t count, const size_t index);

	// swaps a piece of one sound buffer to another by percentage length of said buffer. Is safe because the buffer playing is copied from the buffer this is editing rather than directly streamed. However, it also means that this doesn't effect the currently playing buffer.
	extern void PLEX_soundBufferRandomSwapInt(const int64_t buffer1, const int64_t buffer2, const double perc, const double perc2, const double lenPerc, const size_t linenum, const char* const filename);

	// takes one piece of a buffer and moves it to another piece by the percentage. Is safe because the buffer playing is copied from the buffer this is editing rather than directly streamed. However, it also means that this doesn't effect the currently playing buffer.
	extern void PLEX_soundBufferRearrangeInt(const int64_t buffer, const double perc, const double perc2, const double lenPerc, const size_t linenum, const char* const filename);

	// cuts out a piece of the buffer by percentage, shortening the length. Is safe because the buffer playing is copied from the buffer this is editing rather than directly streamed. However, it also means that this doesn't effect the currently playing buffer.
	extern void PLEX_soundBufferCut(const int64_t buffer, const double perc, const double lenPerc);

	// reverses (backmasks) a piece of the buffer by percentage, shortening the length. Is safe because the buffer playing is copied from the buffer this is editing rather than directly streamed. However, it also means that this doesn't effect the currently playing buffer.
	extern void PLEX_soundReverseInt(const int64_t buffer, const double perc, const double lenPerc, const size_t linenum, const char* const filename);

	// sets the volume of a current index for playback by percentage.
	extern void PLEX_setVolume(const double volume, const size_t index);

	// turns the channel on (copying and playing buffer linked to channel) or off (stops playing). If the 'loop' bool is true the channel will not stop playing until it's told to.
	extern void PLEX_soundChannelOnInt(const bool loop, const size_t index, const size_t line, const char* const filename);
	extern void PLEX_soundChannelOff(const size_t index);

	// clears out all preloaded buffer
	extern void PLEX_clearSoundPreloads();

	// these are signals for external music systems. They don't specifically mean anything but can be used as on/off switches in custom systems.
	extern void PLEX_setUniversalSoundEffect(const size_t id);
	extern void PLEX_setUniversalThemeID(const size_t id);
	extern void PLEX_signalUniversalThemeStop();

	// the start, update, and end of the sound engine. Typically, you never need to call these as they are called in PLEX_start, PLEX_update, and PLEX_end
	extern bool PLEX_startSound();
	extern bool PLEX_updateSound();
	extern void PLEX_endSound();

	// returns the buffer index assigned to channel 'index'
	extern int64_t PLEX_getBuffer(const size_t index);

	// returns the duration of the buffer at 'index' in microseconds
	extern uint64_t PLEX_channelDuration(const size_t index);

	// returns the amount of times that a channel has looped over.
	extern size_t PLEX_channelIteration(const size_t index);

	// returns the set cue signals that can be read outside.
	extern size_t PLEX_getUniversalThemeCueID();
	extern size_t PLEX_getUniversalSoundEffect();
	extern bool PLEX_getUniversalThemeStopSignal();

	// returns whether or not sound is playing at index
	extern bool PLEX_isSoundPlaying(const size_t index);
#ifdef __cplusplus
}
#endif

#endif
