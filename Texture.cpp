#include "texture.h"

CTexture::CTexture()
{
	Log.LogFile << "Constructing Texture object...OK\n\n";

	m_pTexture = NULL;
	m_pDevice = NULL;

	ZeroMemory(&m_LockedRect, sizeof(D3DLOCKED_RECT));
    ZeroMemory(&m_Desc, sizeof(D3DSURFACE_DESC));

	m_Width = 0;
	m_Height = 0;
}

CTexture::~CTexture()
{
	CBase::SafeRelease(m_pTexture, "Releasing Texture");
}

HRESULT CTexture::CreateTextureFromFile(LPDIRECT3DDEVICE9 pDevice, const char *pTextureName, const int ColourKey, 
										const int Width, const int Height)
{	
	m_pDevice = pDevice;

	Log.LogFile << "Loading Texture " << pTextureName << "...";

	HRESULT hr = D3DXCreateTextureFromFileEx(pDevice, pTextureName, Width, Height, 0, 0, D3DFMT_A8R8G8B8, 
											D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, ColourKey,
											NULL, NULL, &m_pTexture);

	if(hr == S_OK)
		Log.LogFile << "OK\n\n";
	else
		throw("\nError, file not found...\n\n");

	// Get width and height data here cos they may have come from the file, only returns top level
    m_pTexture->GetLevelDesc(0, &m_Desc);
	m_Width = m_Desc.Width;
	m_Height = m_Desc.Height;

	return hr;	
}

HRESULT CTexture::CreateTexture(LPDIRECT3DDEVICE9 pDevice, const int Width, const int Height)
{
	m_pDevice = pDevice;

	D3DXCreateTexture(pDevice, Width, Height, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pTexture);

	m_Width = Width;
	m_Height = Height;

	return S_OK;
}

void CTexture::GenerateMipLevels()
{
	assert(m_pTexture != NULL);
	D3DXFilterTexture(m_pTexture, NULL, 0, D3DX_DEFAULT);
}

void CTexture::SetTexture(const int Stage)
{
	assert(m_pTexture != NULL);
	GetDevice()->SetTexture(Stage, m_pTexture);
}

LPDIRECT3DTEXTURE9 CTexture::GetTexture()
{
	return m_pTexture;
}

// Lock the rect and point ptr to the pixel data
void* CTexture::Lock()
{
	m_pTexture->LockRect(0, &m_LockedRect, 0, 0);

	return m_LockedRect.pBits;
}

void CTexture::Unlock()
{
	m_pTexture->UnlockRect(0);
}
