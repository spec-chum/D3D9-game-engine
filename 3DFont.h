#ifndef _cdxfont_h
#define _cdxfont_h

#include "base.h"

#define MESH_FVF (D3DFVF_DIFFUSE|D3DFVF_XYZ|D3DFVF_NORMAL)

class C3DFont
{	
	LPDIRECT3DDEVICE9 m_pDevice;
	LPD3DXFONT m_pFont;
	LPD3DXMESH m_pMesh;
	HDC m_wHdc;
	HFONT m_HFont;

public:
	HRESULT CreateNewFont(LPDIRECT3DDEVICE9 pDevice, const char *pFontName, int Height, BOOL Bold, BOOL Underlined, BOOL Italic);
	void SetText(const DWORD Colour, const char *pText, ...);
	void DrawText() const;
	C3DFont();
	~C3DFont();
};

#endif

