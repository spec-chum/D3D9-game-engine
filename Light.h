#ifndef _clight_h
#define _clight_h

#include "vector3.h"

class CLight
{
	LPDIRECT3DDEVICE9 m_pDevice;
	D3DLIGHT9 m_Light;
	CVector m_vPosition, m_vDirection;
	int m_Index;
	
public:
	enum LIGHT_TYPE
	{
		POINT = 1,
		SPOT,
		DIRECTIONAL,
	};

	CLight();
	~CLight();

	void Create(LPDIRECT3DDEVICE9 pDevice, const int Index, const LIGHT_TYPE Type);

	void SetRange(const float Range);
	void SetDiffuse(const float r, const float g, const float b);
	void SetAttenuation(const float Atten0, const float Atten1 = 0, const float Atten2 = 0);
	void SetSpecular(const float r, const float g, const float b);

	void SetPosition(const CVector &vPos);
	CVector GetPosition(CVector &vec) { return vec = m_vPosition; };

	void SetDirection(const CVector &vDir);
	CVector GetDirection(CVector &vec) { return vec = m_vDirection;};

	void SetAmbient(const float r, const float g, const float b);
	void SetTheta(const float Theta);
	void SetPhi(const float Phi);
	void SetGlobalAmbient(DWORD Ambient);

	void Enable();
	void Disable();

	void Set();
};

#endif