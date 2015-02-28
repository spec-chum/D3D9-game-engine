#include "base.h"

CPixelShader::CPixelShader()
{
	m_pDevice = NULL;
	m_pCode = NULL;
}

CPixelShader::~CPixelShader()
{
	SafeRelease(m_pCode, "Releasing Pixel Shader Code");
}
HRESULT CPixelShader::CreateFromFile(LPDIRECT3DDEVICE9 pDevice, const char* FileName)
{
    m_pDevice = pDevice;

	D3DXAssembleShaderFromFile(FileName, 0, NULL, &m_pCode, NULL);

    m_pDevice->CreatePixelShader((DWORD*)m_pCode->GetBufferPointer(), &m_hPShader);

	if(m_pCode != NULL)
    {
        m_pCode->Release(); 
        m_pCode = NULL;

		return E_FAIL;
    }

	return S_OK;
}

void CPixelShader::SetConstant(DWORD Reg, const void *pData, DWORD Count)
{
	m_pDevice->SetPixelShaderConstant(Reg, pData, Count);
}

void CPixelShader::Set()
{
	m_pDevice->SetPixelShader(m_hPShader);
}