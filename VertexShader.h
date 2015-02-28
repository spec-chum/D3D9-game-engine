#ifndef _cvertexshader_h
#define _cvertexshader_h

#include "base.h"

class CVertexShader : public CAppResource<CVertexShader>
{
	LPDIRECT3DDEVICE9 m_pDevice;
	LPD3DXBUFFER m_pCode;
	LPD3DXBUFFER m_pErrors;
	LPDIRECT3DVERTEXSHADER9 m_hVShader;
	LPDIRECT3DVERTEXDECLARATION9 m_pVDeclaration;

public:
	enum VS_TYPE
	{
		VS_POSITION = 1,
		VS_NORMAL = 2,
		VS_DIFFUSE = 4,
		VS_SPECULAR = 8,
		VS_TEXCOORD0 = 16,
		VS_TEXCOORD1 = 32
	};

	CVertexShader();

	HRESULT CreateFromFile(LPDIRECT3DDEVICE9 pDevice, const char* FileName, int Stream, DWORD Type, int NumTypes);
	void SetConstant(DWORD Reg, const void *pData, DWORD Count);
	void Set();
	LPDIRECT3DVERTEXSHADER9 GetHandle() { return m_hVShader; };
};
#endif