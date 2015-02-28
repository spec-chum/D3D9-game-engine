#include "viewport.h"

void CViewport::Create(const LPDIRECT3DDEVICE9 pDevice, const int PosX, const int PosY, const int Width, const int Height, const float MinZ, const float MaxZ)
{
	m_pDevice = pDevice;

	m_Viewport.X = PosX;
	m_Viewport.Y = PosY;
	m_Viewport.Width = Width;
	m_Viewport.Height = Height;
	m_Viewport.MinZ = MinZ;
	m_Viewport.MaxZ = MaxZ;

	m_MatProj = CMatrix::Identity();
	m_MatView = CMatrix::Identity();
	m_MatWorld = CMatrix::Identity();
	m_MatWorldView = CMatrix::Identity();

	// Set view matrix to identity
	GetDevice()->SetTransform(D3DTS_VIEW, m_MatView);
}

void CViewport::Create(const LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = pDevice;
	GetDevice()->GetViewport(&m_Viewport);
	
	m_MatProj = CMatrix::Identity();
	m_MatView = CMatrix::Identity();
	m_MatWorld = CMatrix::Identity();
	m_MatWorldView = CMatrix::Identity();

	// Set view matrix to identity
	GetDevice()->SetTransform(D3DTS_VIEW, m_MatView);
}

CViewport::~CViewport()
{
}

void CViewport::SetViewport()
{
	GetDevice()->SetViewport(&m_Viewport);
}

void CViewport::GetViewport()
{
	GetDevice()->GetViewport(&m_Viewport);
}

void CViewport::SetProjection(const float fovy, const float aspect, const float zNear, const float zFar)
{
	m_MatProj = CMatrix::PerspectiveFovLH(fovy, aspect, zNear, zFar);
	GetDevice()->SetTransform(D3DTS_PROJECTION, m_MatProj);
}

void CViewport::SetProjection(const CMatrix &ProjectionMatrix)
{
	m_MatProj = ProjectionMatrix;
	GetDevice()->SetTransform(D3DTS_PROJECTION, ProjectionMatrix);
}

void CViewport::MultiplyProjection(const CMatrix &Matrix)
{
	m_MatProj *= Matrix;
	GetDevice()->MultiplyTransform(D3DTS_PROJECTION, Matrix);
}

void CViewport::SetView(const CVector &Eye, const CVector &At, const CVector &Up)
{
	m_MatView = CMatrix::LookAtLH(Eye, At, Up);
	GetDevice()->SetTransform(D3DTS_VIEW, m_MatView);
}

void CViewport::SetView(const CMatrix &ViewMatrix)
{
	m_MatView = ViewMatrix;
	GetDevice()->SetTransform(D3DTS_VIEW, ViewMatrix);
}

void CViewport::MultiplyView(const CMatrix &Matrix)
{
	m_MatView *= Matrix;
	GetDevice()->MultiplyTransform(D3DTS_VIEW, Matrix);
}

void CViewport::SetWorld(const CMatrix &WorldMatrix)
{
	m_MatWorld = WorldMatrix;
	GetDevice()->SetTransform(D3DTS_WORLD, m_MatWorld);
}

void CViewport::MultiplyWorld(const CMatrix &Matrix)
{
	// Keep track of the world matrix
	m_MatWorld *= Matrix;
	GetDevice()->MultiplyTransform(D3DTS_WORLD, Matrix);
}