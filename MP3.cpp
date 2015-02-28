#include "mp3.h"

// Initialise static ref counter to 0
int CMP3::Num = 0;

// Default Constructor.  Checks ref counter to see if this is the first instance, if so initialises BASS.
CMP3::CMP3()
{
	Log.LogFile << "Constructing MP3 object...OK\n\n";

	b_IsPaused = false;
	
	// Check if this is first instance
	if(!Num++)
	{
		// First instance, so initialise BASS
		BASS_Init(-1, 44100, 0, 0);
		BASS_Start();
	}
}

// Overloaded constructor.  Automagically calls Load() and optionally plays file when a filename is specified.
CMP3::CMP3(const char* filename, const int play, const int loop)
{
	Log.LogFile << "Constructing MP3 object...OK\n\n";
	
	b_IsPaused = false;
	
	// Check if this is first instance
	if(!Num++)
	{
		// First instance, so initialise BASS
		BASS_Init(-1, 44100, 0, 0);
		BASS_Start();
	}
	
	// Filename was specified, so call Load().
	Load(filename);
	
	// Check if file should be played now
	if(play)
		Play(loop);
}


// Default Destructor.  Checks ref counter and frees all BASS resources if no instances remaining.
CMP3::~CMP3()
{
	Log.LogFile << "Releasing MP3 object...OK\n\n";

	// Check if any more instances
	if(!--Num)
	{	
		// No more instances so free BASS
		BASS_Free();
	}
}

// Creates a BASS stream from the specified file.  Also calculates length of MP3.  Returns 0 on failure, 1 on success.
int CMP3::Load(const char *filename)
{
	m_Stream = BASS_StreamCreateFile(FALSE, (void *)filename, 0, 0, 0);
	
	// Check if stream was created successfully
	if(!m_Stream)
		return 0;	// Nope :(
	
	// Get length of stream (MP3)
	m_Length.Bytes = (DWORD)BASS_StreamGetLength(m_Stream);
	m_Length.Seconds = (DWORD)BASS_ChannelBytes2Seconds(m_Stream, m_Length.Bytes);
	
	return 1;	// Yup :)
}

// Frees the BASS stream.
void CMP3::Unload() const
{
	BASS_StreamFree(m_Stream);
}

// Plays the MP3 with optional looping (loop = 0 for no looping (default)).  Checks if stream is paused and call appropriate BASS function.
void CMP3::Play(const int loop)
{
	// If stream is paused ChannelResume needs to be called
	if(b_IsPaused)
	{
		b_IsPaused = false;
		BASS_ChannelResume(m_Stream);
		return;
	}
	
	// Play the stream
	BASS_StreamPlay(m_Stream, false, loop);
}

// Pauses the stream
void CMP3::Pause()
{
	b_IsPaused = true;
	BASS_ChannelPause(m_Stream);
}

// Stops the stream
void CMP3::Stop() const
{
	BASS_ChannelStop(m_Stream);
}

// Get length of MP3.  Returns either bytes or seconds (default bytes).
DWORD CMP3::GetLength(const int format) const
{	
	return (format == MP3_BYTES ? m_Length.Bytes : m_Length.Seconds);
}

// Get level of audio output.  Fills MP3_Level struct with stereo Left and Right levels.
void CMP3::GetLevel(CMP3::MP3_Level &Level) const
{
	DWORD Temp_Level = BASS_ChannelGetLevel(m_Stream);
	Level.Left = LOWORD(Temp_Level);
	Level.Right = HIWORD(Temp_Level);
}

//  Get level of audio output.  Returns Left or Right levels.  If LR is unspecified returns DWORD, see func below.
int CMP3::GetLevel(const int LR) const
{
	DWORD Temp_Level = BASS_ChannelGetLevel(m_Stream);
	
	if(LR == MP3_LEFT_CHAN)
		return(LOWORD(Temp_Level));	// Return Left Channel Level
	
	return(HIWORD(Temp_Level));		// Return Right Channel Level
}

//  Get level of audio output.  Returns DWORD containing stereo levels, Left in LOWORD Right in HIWORD.
DWORD CMP3::GetLevel() const
{
	return(BASS_ChannelGetLevel(m_Stream));
}