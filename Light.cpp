#include "light.h"

CLight::CLight()
{
	m_pDevice = NULL;
	m_Index = 0;

	ZeroMemory(&m_Light, sizeof(D3DLIGHT9));
}

CLight::~CLight()
{
}

void CLight::Create(LPDIRECT3DDEVICE9 pDevice, const int Index, const LIGHT_TYPE Type)
{
	m_pDevice = pDevice;
	m_Index = Index;

	ZeroMemory(&m_Light, sizeof(D3DLIGHT9));
	
	m_Light.Type = (D3DLIGHTTYPE)Type;
}

void CLight::Set()
{
	m_pDevice->SetLight(m_Index, &m_Light);
}

void CLight::Enable()
{
	m_pDevice->LightEnable(m_Index, TRUE);
}

void CLight::Disable()
{
	m_pDevice->LightEnable(m_Index, FALSE);
}

void CLight::SetAttenuation(const float Atten0, const float Atten1, const float Atten2)
{
	m_Light.Attenuation0 = Atten0;
	m_Light.Attenuation1 = Atten1;
	m_Light.Attenuation2 = Atten2;
}

void CLight::SetDiffuse(const float r, const float g, const float b)
{
	m_Light.Diffuse.a = 1.0f;
	m_Light.Diffuse.r = r;
	m_Light.Diffuse.g = g;
	m_Light.Diffuse.b = b;
}

void CLight::SetSpecular(const float r, const float g, const float b)
{
	m_Light.Specular.a = 1.0f;
	m_Light.Specular.r = r;
	m_Light.Specular.g = g;
	m_Light.Specular.b = b;
}

void CLight::SetDirection(const CVector &vDir)
{
	m_Light.Direction = (D3DXVECTOR3)vDir;
}

void CLight::SetGlobalAmbient(DWORD Ambient)
{
	m_pDevice->SetRenderState(D3DRS_AMBIENT, Ambient);
}

void CLight::SetAmbient(const float r, const float g, const float b)
{
	m_Light.Ambient.a = 1.0f;
	m_Light.Ambient.r = r;
	m_Light.Ambient.g = g;
	m_Light.Ambient.b = b;
}

void CLight::SetPhi(const float Phi)
{
	m_Light.Phi = Phi;
}

void CLight::SetTheta(const float Theta)
{
	m_Light.Theta = Theta;
}

void CLight::SetRange(const float Range)
{
	m_Light.Range = Range;
}

void CLight::SetPosition(const CVector &vPos)
{
	m_Light.Position = (D3DXVECTOR3)vPos;
}