#include "sprite.h"

void CSprite::Create(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = pDevice;

	D3DXCreateSprite(GetDevice(), &m_pSprite);
}

void CSprite::CreateFromTexture(LPDIRECT3DDEVICE9 pDevice, CTexture *pTexture)
{
	m_pDevice = pDevice;

	D3DXCreateSprite(GetDevice(), &m_pSprite);

	m_pTexture = pTexture->GetTexture();
	m_Width = pTexture->GetWidth();
	m_Height = pTexture->GetHeight();
}

void CSprite::Draw(const float TransX, const float TransY, const float RotationAngle,
				   const float ScalingX, const float ScalingY)
{
	//m_pSprite->Draw(m_pTexture, NULL, &m_vRotCentre, &D3DXVECTOR2(TransX, TransY), m_Colour);
}

void CSprite::SetTexture(CTexture *pTexture)
{
	m_pTexture = pTexture->GetTexture();
	m_Width = pTexture->GetWidth();
	m_Height = pTexture->GetHeight();
}

void CSprite::SetRotationCentre(const int x, const int y)
{
	// Damn conversion warnings, have MS never heard of FIST :/
	m_vRotCentre = D3DXVECTOR2(static_cast<float>(x), static_cast<float>(y));
}

void CSprite::SetCentreAsRotationCentre()
{
	SetRotationCentre(GetWidth() >> 1, GetHeight() >> 1);
}


