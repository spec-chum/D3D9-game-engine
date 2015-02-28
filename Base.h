#ifndef _cbase_h
#define _cbase_h

#pragma comment(lib, "bass")
#pragma comment(lib, "d3d9")

#ifndef _DEBUG
	#pragma comment(lib, "d3dx9")
#else
	#pragma comment(lib, "d3dx9d")
#endif

#pragma warning(disable : 4251)
#pragma warning(disable : 4275)

#ifdef _DEBUG
	#pragma warning(disable : 4786)	// Disable debug names truncated warning
#endif

extern "C" int g_USE_SSE;

#include <d3d9.h>
#include <d3dx9.h>

#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
#include <assert.h>
#include <cmath>
#include <float.h>

#include <bass\bass.h>
#include "appresource.h"
#include "log.h"
#include "utils.h"

extern HWND g_hWnd;

class CBase
{	
	LPDIRECT3D9 m_pD3D;
	LPDIRECT3DDEVICE9 m_pd3dDevice;

	void CheckForSSE();
	void AssignPointers();
	
protected:
	D3DPRESENT_PARAMETERS m_d3dpp;
	HWND m_hWnd;
	BOOL m_IsFullScreen;
	int m_Width;
	int m_Height;
	int m_Bpp;
	const char *m_Title;

	virtual HRESULT Render() { return S_OK; };
	virtual HRESULT InitScene() { return S_OK; };
	virtual HRESULT MoveFrame() { return S_OK; };
	virtual HRESULT InitD3D9();

public:
	LPDIRECT3DDEVICE9 GetDevice() { assert(m_pd3dDevice); return m_pd3dDevice; };
	virtual HRESULT OneTimeInit() { return S_OK; };
	virtual LRESULT MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual ~CBase();
	int Run();
	int GetHeight() const { return m_Height; };
	int GetWidth() const { return m_Width; };
	HRESULT Create();
	BOOL IsFullScreen() const { return m_IsFullScreen; };
	void SetHeight(const int Height) { m_Height = Height; };
	void SetWidth(const int Width) { m_Width = Width; };
	void SetDimensions(const int Width, const int Height)	{ m_Width = Width; m_Height = Height; };
	void SetTitle(const char *Title);
	void SetFullScreen(const BOOL FullScreen) { m_IsFullScreen = FullScreen; };
	void CentreWindow() const;
	template<class T> static void SafeRelease(T &IObject, const char *message = "Releasing Unknown");
	template<class T> static void SafeDelete(T &Ptr, const char *message = "Deleting Unknown");
	template<class T> static void SafeDeleteArray(T &Ptr, const char *message = "Deleting Unknown");
	CBase(BOOL FullScreen = false, int Width = 1024, int Height = 768, const char *Title = "SpecGen");
};

template<class T> void CBase::SafeRelease(T &IObject, const char *message)
{
	Log.LogFile << message << "...";
	
	if(IObject)
	{
		IObject->Release();
		IObject = NULL;
		Log.LogFile << "OK.\n\n";
	}
	else
		Log.LogFile << "Nothing to release.\n\n";
}

template<class T> void CBase::SafeDelete(T &Ptr, const char *message)
{
	Log.LogFile << message << "...";
	
	if(Ptr)
	{
		delete Ptr;
		Ptr = NULL;
		Log.LogFile << "OK.\n\n";
	}
	else
		Log.LogFile << "Nothing to delete.\n\n";
}

template<class T> void CBase::SafeDeleteArray(T &Ptr, const char *message)
{
	Log.LogFile << message << "...";
	
	if(Ptr)
	{
		delete [] Ptr;
		Ptr = NULL;
		Log.LogFile << "OK.\n\n";
	}
	else
		Log.LogFile << "Nothing to delete.\n\n";
}

#endif