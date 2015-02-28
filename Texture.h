#ifndef _ctexture_h
#define _ctexture_h

#include "base.h"

class CTexture
{
	LPDIRECT3DDEVICE9 m_pDevice;
	LPDIRECT3DTEXTURE9 m_pTexture;
	D3DLOCKED_RECT m_LockedRect;
    D3DSURFACE_DESC m_Desc;
	int m_Width, m_Height;

public:
	LPDIRECT3DDEVICE9 GetDevice() { assert(m_pDevice); return m_pDevice; };

	HRESULT CreateTextureFromFile(LPDIRECT3DDEVICE9 pDevice, const char *pTextureName, const int ColourKey = 0, 
							const int Width = 0, const int Height = 0);

	HRESULT CreateTexture(LPDIRECT3DDEVICE9 pDevice, const int Width, const int Height);

	void GenerateMipLevels();

	void* Lock();
	void Unlock();

	void SetTexture(const int Stage);
	LPDIRECT3DTEXTURE9 GetTexture();

	int GetWidth() { return m_Width; };
	int GetHeight() { return m_Height; };

	CTexture();
	~CTexture();
};

#endif