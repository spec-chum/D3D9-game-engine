#ifndef _csprite_h
#define _csprite_h

#include "texture.h"

class CSprite
{
	LPDIRECT3DDEVICE9 m_pDevice;
	LPD3DXSPRITE m_pSprite;
	LPDIRECT3DTEXTURE9 m_pTexture;
	D3DXVECTOR2 m_vRotCentre;
	DWORD m_Colour;
	int m_Width, m_Height;

public:
	CSprite::CSprite() { m_pDevice = NULL; m_pSprite = NULL; };
	CSprite::~CSprite() { m_pSprite->Release(); };

	LPDIRECT3DDEVICE9 GetDevice() { assert(m_pDevice); return m_pDevice; };
	void Create(LPDIRECT3DDEVICE9 pDevice);
	void CreateFromTexture(LPDIRECT3DDEVICE9 pDevice, CTexture *pTexture);

	// Draw calls Begin and End internally, but call these if using draw multiple times to avoid overhead
	void Begin() { m_pSprite->Begin(NULL); };
	void End() { m_pSprite->End(); };

	void Draw(const float TransX, const float TransY, const float RotationAngle = 0.0f,
			const float ScalingX = 1.0f, const float ScalingY = 1.0f);

	void SetTexture(CTexture *pTexture);
	void SetColour(const DWORD Colour) { m_Colour = Colour; };
	void SetRotationCentre(const int x, const int y);
	void SetCentreAsRotationCentre();

	int GetWidth() { return m_Width; };
	int GetHeight() { return m_Height; };

};

#endif