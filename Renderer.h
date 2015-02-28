#ifndef _CRenderer_h
#define _CRenderer_h

#include "base.h"

class CRenderer
{
	LPDIRECT3DDEVICE9 m_pD3DDevice;
	D3DMATERIAL9 m_Material;
	LPDIRECT3DVERTEXBUFFER9 m_pVBuffer;
	void *pVertices;

public:
	static const DWORD DEFAULT_FVF;

	struct DEFAULT_VERTEX
	{
		float x, y, z;
		float nx, ny, nz;
		DWORD colour;
		float tu, tv;
	};

	enum FILTERMODE
	{
		BILINEAR,
		TRILINEAR
	};

	enum CULLMODE
	{
		NONE = 1,
		FRONT,
		BACK		
	};

	enum TEXTUREBLENDMODE
	{
		BLEND_NONE,
		BLEND_MODULATE,
		BLEND_ADD,
		BLEND_ALPHA,
		BLEND_INVALPHA,
		BLEND_ALPHA_MODULATE				
	};

	enum TEXTUREOP
	{
		TEXOP_NONE,
		TEXOP_MODULATE,
		TEXOP_TEXTURE,
		TEXOP_DIFFUSE,
		TEXOP_CURRENT_DIFF,
		TEXOP_CURRENT_TEX,
		TEXOP_DOT3
	};

	enum TEXWRAPMODE
	{
		WRAP = 1,
		MIRROR,
		CLAMP
	};

	enum PRIMTYPE
	{
		POINTLIST = 1,
		LINELIST,
		LINESTRIP,
		TRILIST,
		TRISTRIP,
		TRIFAN	
	};

	enum SPECSOURCE
	{
		MATERIAL,
		COLOR1, 
		COLOR2
	};

	enum CMPFUNC
	{
		NEVER = 1,
		LESS = 2,
		EQUAL = 3,
		LESSEQUAL = 4,
		GREATER = 5,
		NOTEQUAL = 6,
		GREATEREQUAL = 7,
		ALWAYS = 8,
	};

	enum STENCILOP
	{
	    KEEP           = 1,
		ZERO           = 2,
		REPLACE        = 3,
		INCRSAT        = 4,
		DECRSAT        = 5,
		INVERT         = 6,
		INCR           = 7,
		DECR           = 8,
	};

	CRenderer() { ZeroMemory(&m_Material, sizeof(D3DMATERIAL9)); };
	~CRenderer();
    
	LPDIRECT3DDEVICE9 GetDevice() { assert(m_pD3DDevice); return m_pD3DDevice;};
	void SetDevice(LPDIRECT3DDEVICE9 pD3DDevice) { m_pD3DDevice = pD3DDevice; };

	void Create(LPDIRECT3DDEVICE9 pDevice);

	// Clears the render target and z-buffer, but ignores the stencil buffer
	void Clear(const DWORD ClearColour);

	// Clears the stencil buffer too
	void ClearAll(const DWORD ClearColour);

	void BeginScene();
	void EndScene();
	void Present();

	void SetFilterMode(const DWORD Stage, const FILTERMODE FilterMode);

	void SetCullMode(const CULLMODE CullMode);
	void SetTextureBlendMode(const TEXTUREBLENDMODE BlendMode);
	void SetTextureColourBlend(const int Stage, const TEXTUREOP TexOp);
	void SetTextureStage(const int Stage, const D3DTEXTURESTAGESTATETYPE Type, int Value);
	void SetTextureBlendFactor(const int Stage, const BYTE Alpha);
	void SetTextureAlphaBlend(const int Stage, const TEXTUREOP TexOp);
	void SetTextureWrapMode(const int Stage, const TEXWRAPMODE Wrap);
	void SetTextureWrapMode(const int Stage, const TEXWRAPMODE WrapU, const TEXWRAPMODE WrapV);
	void SetSpecularSource(const SPECSOURCE Source); 
	void ClearTextureStage(const int Stage);

	void CopyTextureCoords(const int FromStage, const int ToStage);

	void EnableZBuffer();
	void DisableZBuffer();
	void EnableLighting();
	void DisableLighting();
	void EnableAlphaBlend();
	void DisableAlphaBlend();
	void EnableSpecular();
	void DisableSpecular();
	void EnableStencilBuffer();
	void DisableStencilBuffer();

	void SetStencilRef(const DWORD Ref);
	void SetStencilMask(const DWORD Mask);
	void SetStencilFunc(const CMPFUNC Func);
	void SetStencilOps(const STENCILOP Fail, const STENCILOP ZFail, const STENCILOP Pass);

	void SetStreamSource(const UINT Stream, const LPDIRECT3DVERTEXBUFFER9 StreamData, const UINT Stride);
	void SetDefaultStream();

	void LockVBuffer();
	void UnlockVBuffer();
	void VertexCopy(const void *src, const size_t count);

	void SetFVF(const DWORD FVF);

	void SetMaterial();
	void SetMaterialDiffuse(const float r, const float g, const float b);
	void SetMaterialSpecular(const float r, const float g, const float b);
	void SetMaterialAmbient(const float r, const float g, const float b);
	void SetMaterialEmissive(const float r, const float g, const float b);
	void SetMaterialPower(const float Power);

	void DrawPrimitive(PRIMTYPE Type, UINT Start, UINT Count);

	void ShowCursor(const BOOL bShow);
};

#endif