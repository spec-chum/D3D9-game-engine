#ifndef _cpixelshader_
#define _cpixelshader_

#include "base.h"

class CPixelShader : public CAppResource<CPixelShader>
{
	LPDIRECT3DDEVICE9 m_pDevice;
	ID3DXBuffer *m_pCode;
	DWORD m_hPShader;

public:
	CPixelShader();
	~CPixelShader();

	HRESULT CreateFromFile(LPDIRECT3DDEVICE9 pDevice, const char* FileName);
	void SetConstant(DWORD Reg, const void *pData, DWORD Count);
	void Set();
	DWORD GetHandle() { return m_hPShader; };
};

#endif