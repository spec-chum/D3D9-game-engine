#ifndef _cmesh_h
#define _cmesh_h

#include "matrix.h"
#include "viewport.h"
#include "frustum.h"

class CMesh
{
	LPDIRECT3DDEVICE9 m_pDevice;
	LPD3DXMESH m_pMesh;
	LPD3DXMESH m_pShadowMesh;
	LPDIRECT3DTEXTURE9* m_pMeshTextures;
	D3DMATERIAL9* m_pMeshMaterials;
	CMatrix m_MatTransform, m_MatRotate, m_MatScale, m_MatTrans;
	DWORD m_dwNumMaterials;
	CVector m_vSphereCentre;
	float m_fSphereRadius;
	static const DWORD MeshFVF;

	struct MESHVERTEX
	{
		float x, y, z;
		float nx, ny, nz;
		DWORD Colour;
		float tu, tv;
	}; 
	
public:
	CMesh();
	CMesh(const CMesh &Mesh);
	~CMesh();

	CVector m_Rotate;
	CVector m_Scale;
	CVector m_Trans;

	HRESULT CreateMeshFromX(LPDIRECT3DDEVICE9 pDevice, const char *FileName);
	CMatrix GetTransform()	{ return m_MatTransform; };

	void Transform(const CMatrix &pMat);
	void SetTransform(CViewport *pViewport) const;
	void SetTransformAll(CViewport *pViewport);
	void MultiplyTransform(CViewport *pViewport) const;
	void MultiplyTransformAll(CViewport *pViewport);
	void ResetTransform();

	void Rotate();
	void SetRotate(const float x, const float y, const float z);
	void Scale();
	void SetScale(const float x, const float y, const float z);
	CVector& GetScale() { return m_Scale; };
	void Translate();
	void SetTranslate(const float x, const float y, const float z);
	CVector& GetTranslate() { return m_Trans; };

	void ComputeBoundingSphere();
	const CVector& GetBoundingSphereCentre() const { return m_vSphereCentre; };
	const float GetBoundingSphereRadius() const { return m_fSphereRadius; };

	DWORD GetNumVerts() { return m_pMesh->GetNumVertices(); };
	LPD3DXMESH GetMesh() { return m_pMesh; };

	void RenderMesh(const CFrustum *pFrustum) const;
	void RenderMeshShadow(const CFrustum *pFrustum) const;

	CMesh& operator=(const CMesh &Mesh);
};

#endif