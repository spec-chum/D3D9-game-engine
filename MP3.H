#ifndef _cmp3_h
#define _cmp3_h

#include "base.h"

class CMP3 : public CAppResource<CMP3>
{
	struct 
	{
		DWORD Seconds;
		DWORD Bytes;
	} m_Length;

	HSTREAM m_Stream;
	BOOL b_IsPaused;
	static int Num;

public:
	enum MP3
	{
		MP3_NULL,
		MP3_NO_PLAY,
		MP3_PLAY,
		MP3_NO_LOOP,
		MP3_LOOP = BASS_SAMPLE_LOOP,
		MP3_BYTES = 5,
		MP3_SECONDS,
		MP3_LEFT_CHAN,
		MP3_RIGHT_CHAN	
	};
	struct MP3_Level
	{
		DWORD Left;
		DWORD Right;
	};

	CMP3();
	CMP3(const char *, const int = MP3_NO_PLAY, const int = MP3_NO_LOOP);
	~CMP3();
	int Load(const char *);
	void Unload(void) const;
	void Play(const int = MP3_NO_LOOP);
	void Pause(void);
	void Stop(void) const;
	DWORD GetLength(const int = MP3_BYTES) const;
	void GetLevel(MP3_Level&) const;
	int GetLevel(const int) const;
	DWORD GetLevel(void) const;
};
#endif