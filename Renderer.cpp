#include "renderer.h"

const DWORD CRenderer::DEFAULT_FVF = D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1;

CRenderer::~CRenderer()
{
	m_pD3DDevice = NULL;

	CBase::SafeRelease(m_pVBuffer, "Releasing default VBuffer");
}

void CRenderer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	m_pD3DDevice = pDevice;
	assert(m_pD3DDevice);

	// Create a default vertex buffer
	GetDevice()->CreateVertexBuffer(1024*sizeof(DEFAULT_VERTEX), D3DUSAGE_WRITEONLY, DEFAULT_FVF, D3DPOOL_MANAGED, &m_pVBuffer, NULL);
}

void CRenderer::ClearAll(const DWORD ClearColour)
{
	GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, ClearColour, 1.0f, 0);
}

void CRenderer::Clear(const DWORD ClearColour)
{
	GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, ClearColour, 1.0f, 0);
}

void CRenderer::BeginScene()
{
	GetDevice()->BeginScene();
}

void CRenderer::EndScene()
{
	GetDevice()->EndScene();
}

void CRenderer::Present()
{
	GetDevice()->Present(NULL, NULL, NULL, NULL);
}

void CRenderer::SetFilterMode(const DWORD Stage, const FILTERMODE FilterMode)
{
	GetDevice()->SetSamplerState(Stage, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	GetDevice()->SetSamplerState(Stage, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	if(FilterMode == BILINEAR)
		// Bi-Linear
		GetDevice()->SetSamplerState(Stage, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
	else
		// Tri_Linear
		GetDevice()->SetSamplerState(Stage, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
}

void CRenderer::SetCullMode(const CULLMODE CullMode)
{
	GetDevice()->SetRenderState(D3DRS_CULLMODE, CullMode);
}

void CRenderer::SetTextureBlendMode(const TEXTUREBLENDMODE BlendMode)
{
	GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	
	switch(BlendMode)
	{
	case BLEND_NONE:
		GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
		break;

	case BLEND_MODULATE:
		GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
		GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
		break;

	case BLEND_ADD:
		GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;

	case BLEND_ALPHA:
		GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		break;

	case BLEND_INVALPHA:
		GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		break;
		
	case BLEND_ALPHA_MODULATE:
		GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
		GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
		break;
	}
}

void CRenderer::SetTextureColourBlend(const int Stage, const TEXTUREOP TexOp)
{
	switch(TexOp)
	{
	case TEXOP_NONE:
		GetDevice()->SetTextureStageState(Stage, D3DTSS_COLOROP, D3DTOP_DISABLE);
		break;

	case TEXOP_MODULATE:
		GetDevice()->SetTextureStageState(Stage, D3DTSS_COLOROP, D3DTOP_MODULATE);
		GetDevice()->SetTextureStageState(Stage, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		GetDevice()->SetTextureStageState(Stage, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		break;

	case TEXOP_TEXTURE:
		GetDevice()->SetTextureStageState(Stage, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		GetDevice()->SetTextureStageState(Stage, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		break;

	case TEXOP_DIFFUSE:
		GetDevice()->SetTextureStageState(Stage, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		GetDevice()->SetTextureStageState(Stage, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
		break;

	case TEXOP_CURRENT_DIFF:
		GetDevice()->SetTextureStageState(Stage, D3DTSS_COLOROP, D3DTOP_MODULATE);
		GetDevice()->SetTextureStageState(Stage, D3DTSS_COLORARG1, D3DTA_CURRENT);
		GetDevice()->SetTextureStageState(Stage, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		break;

	case TEXOP_CURRENT_TEX:
		GetDevice()->SetTextureStageState(Stage, D3DTSS_COLOROP, D3DTOP_MODULATE);
		GetDevice()->SetTextureStageState(Stage, D3DTSS_COLORARG1, D3DTA_CURRENT);
		GetDevice()->SetTextureStageState(Stage, D3DTSS_COLORARG2, D3DTA_TEXTURE);
		break;

	case TEXOP_DOT3:
		GetDevice()->SetTextureStageState(Stage, D3DTSS_COLOROP, D3DTOP_DOTPRODUCT3);
		GetDevice()->SetTextureStageState(Stage, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		GetDevice()->SetTextureStageState(Stage, D3DTSS_COLORARG2, D3DTA_TFACTOR);
		break;
	}
}

void CRenderer::SetTextureStage(const int Stage, const D3DTEXTURESTAGESTATETYPE Type, int Value)
{
	GetDevice()->SetTextureStageState(Stage, Type, Value);
}

void CRenderer::SetTextureBlendFactor(const int Stage, const BYTE Alpha)
{
	GetDevice()->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(Alpha, 255, 255, 255));
	GetDevice()->SetTextureStageState(Stage, D3DTSS_COLOROP, D3DTOP_BLENDFACTORALPHA);
}

void CRenderer::SetTextureAlphaBlend(const int Stage, const TEXTUREOP TexOp)
{
	switch(TexOp)
	{
	case TEXOP_NONE:
		GetDevice()->SetTextureStageState(Stage, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		break;
		
	case TEXOP_MODULATE:
		GetDevice()->SetTextureStageState(Stage, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		GetDevice()->SetTextureStageState(Stage, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		GetDevice()->SetTextureStageState(Stage, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		break;

	case TEXOP_TEXTURE:
		GetDevice()->SetTextureStageState(Stage, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		GetDevice()->SetTextureStageState(Stage, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		break;

	case TEXOP_DIFFUSE:
		GetDevice()->SetTextureStageState(Stage, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		GetDevice()->SetTextureStageState(Stage, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
		break;

	default:
		break;
	}
}

void CRenderer::SetTextureWrapMode(const int Stage, const TEXWRAPMODE Wrap)
{
	GetDevice()->SetSamplerState(Stage, D3DSAMP_ADDRESSU, Wrap);
	GetDevice()->SetSamplerState(Stage, D3DSAMP_ADDRESSV, Wrap);
}

void CRenderer::ClearTextureStage(const int Stage)
{
	GetDevice()->SetTexture(Stage, NULL);
}

void CRenderer::CopyTextureCoords(const int FromStage, const int ToStage)
{
	GetDevice()->SetTextureStageState(ToStage, D3DTSS_TEXCOORDINDEX, FromStage);
}

void CRenderer::SetTextureWrapMode(const int Stage, const TEXWRAPMODE WrapU, const TEXWRAPMODE WrapV)
{
	GetDevice()->SetSamplerState(Stage, D3DSAMP_ADDRESSU, WrapU);
	GetDevice()->SetSamplerState(Stage, D3DSAMP_ADDRESSV, WrapV);
}

void CRenderer::SetSpecularSource(const SPECSOURCE Source)
{
	GetDevice()->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, Source);
}

void CRenderer::EnableZBuffer()
{
	GetDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);
}

void CRenderer::DisableZBuffer()
{
	GetDevice()->SetRenderState(D3DRS_ZENABLE, FALSE);
}

void CRenderer::EnableLighting()
{
	GetDevice()->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void CRenderer::DisableLighting()
{
	GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
}

void CRenderer::EnableAlphaBlend()
{
	GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
}

void CRenderer::DisableAlphaBlend()
{
	GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CRenderer::EnableSpecular()
{
	GetDevice()->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
}

void CRenderer::DisableSpecular()
{
	GetDevice()->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
}

void CRenderer::EnableStencilBuffer()
{
	GetDevice()->SetRenderState(D3DRS_STENCILENABLE, TRUE);
}

void CRenderer::DisableStencilBuffer()
{
	GetDevice()->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

void CRenderer::SetStencilRef(const DWORD Ref)
{
	GetDevice()->SetRenderState(D3DRS_STENCILREF, Ref);
}

void CRenderer::SetStencilMask(const DWORD Mask)
{
	GetDevice()->SetRenderState(D3DRS_STENCILMASK, Mask);
}

void CRenderer::SetStencilFunc(const CMPFUNC Func)
{
	GetDevice()->SetRenderState(D3DRS_STENCILFUNC, Func);
}

void CRenderer::SetStencilOps(const STENCILOP Fail, const STENCILOP ZFail, const STENCILOP Pass)
{
	GetDevice()->SetRenderState(D3DRS_STENCILFAIL, Fail);
	GetDevice()->SetRenderState(D3DRS_STENCILZFAIL, ZFail);
	GetDevice()->SetRenderState(D3DRS_STENCILPASS, Pass);
}

void CRenderer::SetStreamSource(const UINT Stream, const LPDIRECT3DVERTEXBUFFER9 StreamData, const UINT Stride)
{
	GetDevice()->SetStreamSource(Stream, StreamData, 0, Stride);
}

void CRenderer::SetDefaultStream()
{
	GetDevice()->SetStreamSource(0, m_pVBuffer, 0, sizeof(DEFAULT_VERTEX));
	GetDevice()->SetFVF(DEFAULT_FVF);
}

void CRenderer::LockVBuffer()
{
	m_pVBuffer->Lock(0, 1024, (void**)&pVertices, 0);
}

void CRenderer::UnlockVBuffer()
{
	m_pVBuffer->Unlock();
}

void CRenderer::VertexCopy(const void *src, const size_t count)
{	
	CUtils::memcpy64(pVertices, src, count);
}

void CRenderer::SetFVF(const DWORD FVF)
{
	GetDevice()->SetFVF(FVF);
}

void CRenderer::SetMaterial()
{
	GetDevice()->SetMaterial(&m_Material);
}

void CRenderer::DrawPrimitive(PRIMTYPE Type, UINT Start, UINT Count)
{
	GetDevice()->DrawPrimitive((D3DPRIMITIVETYPE)Type, Start, Count);
}

void CRenderer::SetMaterialAmbient(const float r, const float g, const float b)
{
	m_Material.Ambient.a = 1.0f;
	m_Material.Ambient.r = r;
	m_Material.Ambient.g = g;
	m_Material.Ambient.b = b;
}

void CRenderer::SetMaterialDiffuse(const float r, const float g, const float b)
{
	m_Material.Diffuse.a = 1.0f;
	m_Material.Diffuse.r = r;
	m_Material.Diffuse.g = g;
	m_Material.Diffuse.b = b;
}

void CRenderer::SetMaterialSpecular(const float r, const float g, const float b)
{
	m_Material.Specular.a = 1.0f;
	m_Material.Specular.r = r;
	m_Material.Specular.g = g;
	m_Material.Specular.b = b;
}

void CRenderer::SetMaterialEmissive(const float r, const float g, const float b)
{
	m_Material.Emissive.a = 1.0f;
	m_Material.Emissive.r = r;
	m_Material.Emissive.g = g;
	m_Material.Emissive.b = b;
}

void CRenderer::SetMaterialPower(const float Power)
{
	m_Material.Power = Power;
}

void CRenderer::ShowCursor(const BOOL bShow)
{
	GetDevice()->ShowCursor(bShow);
}