#include "base.h"

CVertexShader::CVertexShader()
{
	m_pDevice = NULL;
	m_pCode = NULL;
}

HRESULT CVertexShader::CreateFromFile(LPDIRECT3DDEVICE9 pDevice, const char* FileName, int Stream, DWORD Type, int NumTypes)
{
    m_pDevice = pDevice;
	int offset = 0;
	int index = 0;
	D3DVERTEXELEMENT9 *decl = new D3DVERTEXELEMENT9[NumTypes+1];

	// Position
	if(Type & 1)
	{
		decl[index].Stream = Stream;
		decl[index].Offset = offset;
		decl[index].Method = D3DDECLMETHOD_DEFAULT;
		decl[index].Type = D3DDECLTYPE_FLOAT3;
		decl[index].Usage = D3DDECLUSAGE_POSITION;
		decl[index].UsageIndex = 0;
		offset += 12;
		index++;
	}

	// Normal
	if(Type & 2)
	{
		decl[index].Stream = Stream;
		decl[index].Offset = offset;
		decl[index].Method = D3DDECLMETHOD_DEFAULT;
		decl[index].Type = D3DDECLTYPE_FLOAT3;
		decl[index].Usage = D3DDECLUSAGE_NORMAL;
		decl[index].UsageIndex = 0;
		offset += 12;
		index++;
	}

	// Diffuse
	if(Type & 4)
	{
		decl[index].Stream = Stream;
		decl[index].Offset = offset;
		decl[index].Method = D3DDECLMETHOD_DEFAULT;
		decl[index].Type = D3DDECLTYPE_D3DCOLOR;
		decl[index].Usage = D3DDECLUSAGE_COLOR;
		decl[index].UsageIndex = 0;
		offset += 4;
		index++;
	}

	// Specular
	if(Type & 8)
	{
		decl[index].Stream = Stream;
		decl[index].Offset = offset;
		decl[index].Method = D3DDECLMETHOD_DEFAULT;
		decl[index].Type = D3DDECLTYPE_D3DCOLOR;
		decl[index].Usage = D3DDECLUSAGE_COLOR;
		decl[index].UsageIndex = 1;
		offset += 4;
		index++;
	}

	// Tex Co-ord 0
	if(Type & 16)
	{
		decl[index].Stream = Stream;
		decl[index].Offset = offset;
		decl[index].Method = D3DDECLMETHOD_DEFAULT;
		decl[index].Type = D3DDECLTYPE_FLOAT2;
		decl[index].Usage = D3DDECLUSAGE_TEXCOORD;
		decl[index].UsageIndex = 0;
		offset += 8;
		index++;
	}

	// Tex Co-ord 1
	if(Type & 32)
	{
		decl[index].Stream = Stream;
		decl[index].Offset = offset;
		decl[index].Method = D3DDECLMETHOD_DEFAULT;
		decl[index].Type = D3DDECLTYPE_FLOAT2;
		decl[index].Usage = D3DDECLUSAGE_TEXCOORD;
		decl[index].UsageIndex = 1;
		offset += 8;
		index++;
	}

	// Terminate declaration
	decl[index].Stream = 255;
	decl[index].Offset = 0;
	decl[index].Type = D3DDECLTYPE_UNUSED;
	decl[index].Method = 0;
	decl[index].Usage = 0;
	decl[index].UsageIndex = 0;

	m_pDevice->CreateVertexDeclaration(decl, &m_pVDeclaration);
	SafeDelete(decl, "Deleting Vertex Shader declaration");

	Log.LogFile << "Assembling Vertex Shader from " << FileName << "...\n";
	if(D3DXAssembleShaderFromFile(FileName, NULL, NULL, 0, &m_pCode, &m_pErrors) != D3D_OK)
	{
		Log.LogFile << "Errors:\n" << (char*)m_pErrors->GetBufferPointer() << "\n";
		SafeRelease(m_pErrors, "Releasing Vertex Shader Error Buffer");

		return E_FAIL;
	}
	else
		Log.LogFile << "All Assembled OK.\n\n";
	
    m_pDevice->CreateVertexShader((DWORD*)m_pCode->GetBufferPointer(), &m_hVShader);

    SafeRelease(m_pCode, "Releasing Vertex Shader Code");
	SafeRelease(m_pErrors, "Releasing Vertex Shader Error Buffer");

	return S_OK;
}

void CVertexShader::SetConstant(DWORD Reg, const void *pData, DWORD Count)
{
	m_pDevice->SetVertexShaderConstantF(Reg, (float*)pData, Count);
}

void CVertexShader::Set()
{
	m_pDevice->SetVertexDeclaration(m_pVDeclaration);
	m_pDevice->SetVertexShader(m_hVShader);
}