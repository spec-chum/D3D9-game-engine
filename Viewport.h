#ifndef _cviewport_h
#define _cviewport_h

#include "matrix.h"

class CViewport
{
	LPDIRECT3DDEVICE9 m_pDevice;
	D3DVIEWPORT9 m_Viewport;
	CMatrix m_MatProj, m_MatView, m_MatWorld, m_MatWorldView;

public:
	CViewport() {};
	~CViewport();

	void Create(const LPDIRECT3DDEVICE9 pDevice, const int PosX, const int PosY, const int Width, 
			const int Height, const float MinZ, const float MaxZ);

	void Create(const LPDIRECT3DDEVICE9 pDevice);

	LPDIRECT3DDEVICE9 GetDevice() { assert(m_pDevice); return m_pDevice; };

	void SetViewport();
	void GetViewport();

	void GetProjection(CMatrix &Matrix) { Matrix = m_MatProj; };
	void SetProjection(const float fovy, const float aspect, const float zNear, const float zFar);
	void SetProjection(const CMatrix &ProjectionMatrix);
	void MultiplyProjection(const CMatrix &Matrix);

	void GetView(CMatrix &Matrix) { Matrix = m_MatView; };
	void SetView(const CVector &Eye, const CVector &At, const CVector &Up);	
	void SetView(const CMatrix &ViewMatrix);
	void MultiplyView(const CMatrix &Matrix);

	void SetWorld(const CMatrix &WorldMatrix);
	void MultiplyWorld(const CMatrix &Matrix);

	int GetWidth() const { return m_Viewport.Width; };
	int GetHeight() const { return m_Viewport.Height; };

};

#endif