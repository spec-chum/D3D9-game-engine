#include "3dfont.h"

C3DFont::C3DFont()
{
	Log.LogFile << "Constructing 3DFont object...OK\n\n";
	
	m_pFont = NULL;
	m_pDevice = NULL;
	m_HFont = NULL;
	m_pMesh = NULL;
	m_wHdc = NULL;
}

C3DFont::~C3DFont()
{	 
	CBase::SafeRelease(m_pFont, "Releasing 3DFont interface");
}

HRESULT C3DFont::CreateNewFont(LPDIRECT3DDEVICE9 pDevice, const char *pFontName, int Height, BOOL Bold, BOOL Underlined, BOOL Italic)
{
	m_pDevice = pDevice;
	
	m_wHdc = GetDC(g_hWnd);

	int Weight = Bold ? FW_BOLD : FW_NORMAL; 	
	m_HFont = CreateFont(Height, 0, 0, 0, Weight, Italic, Underlined, 0, ANSI_CHARSET, 0, 0, 0, 0, pFontName);
	
	ReleaseDC(g_hWnd, m_wHdc);	

	return S_OK;
}

void C3DFont::SetText(const DWORD Colour, const char *pText, ...)
{
	struct my_vertex
	{
		float x, y, z;
		float nx, ny, nz;
		DWORD colour;
	};

	LPD3DXMESH pMesh = NULL;

	char buffer[1024];
	va_list args;
	
	va_start(args, pText);
	vsprintf(buffer, pText, args);
	va_end(args);
	
	SelectObject(m_wHdc, m_HFont);
	D3DXCreateText(m_pDevice, m_wHdc, buffer, 0.001f, 0.2f, &pMesh, NULL, NULL);
	pMesh->CloneMeshFVF(D3DXMESH_MANAGED, MESH_FVF, m_pDevice, &m_pMesh);	
	pMesh->Release();	
	D3DXComputeNormals(m_pMesh, NULL);
	
	DWORD vert_num, count;
	my_vertex *vertices;
		
	vert_num = m_pMesh->GetNumVertices();
		
	m_pMesh->LockVertexBuffer(0, (void**)&vertices);
		
	for(count = 0; count < vert_num; count++)
	{
		vertices[count].colour = Colour;
	}
		
	m_pMesh->UnlockVertexBuffer();		
}

void C3DFont::DrawText() const
{
	m_pMesh->DrawSubset(0);
}