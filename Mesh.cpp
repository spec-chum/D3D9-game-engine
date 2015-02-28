#include "mesh.h"

const DWORD CMesh::MeshFVF = D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1;

CMesh::CMesh()
{
	m_pDevice = NULL;
	m_dwNumMaterials = 0;
	m_pMeshMaterials = NULL;
	m_pMeshTextures = NULL;
	m_pMesh = NULL;
	m_MatTrans = CMatrix::Identity();
	m_MatScale = CMatrix::Identity();
	m_MatRotate = CMatrix::Identity();
	m_MatTransform = CMatrix::Identity();
}

CMesh::CMesh(const CMesh &Mesh)
{
	m_pDevice = Mesh.m_pDevice;
	m_dwNumMaterials = Mesh.m_dwNumMaterials;
	m_pMeshMaterials = new D3DMATERIAL9[m_dwNumMaterials];
	m_pMeshTextures  = new LPDIRECT3DTEXTURE9[m_dwNumMaterials];

	for(DWORD i = 0; i < m_dwNumMaterials; ++i)
	{
		m_pMeshMaterials[i] = Mesh.m_pMeshMaterials[i];
		m_pMeshTextures[i] = Mesh.m_pMeshTextures[i];
	}

	m_pMesh = Mesh.m_pMesh;

	// Add reference to the COM object
	m_pMesh->AddRef();
}

CMesh& CMesh::operator=(const CMesh &Mesh)
{
	m_pDevice = Mesh.m_pDevice;
	m_dwNumMaterials = Mesh.m_dwNumMaterials;
	m_pMeshMaterials = new D3DMATERIAL9[m_dwNumMaterials];
	m_pMeshTextures  = new LPDIRECT3DTEXTURE9[m_dwNumMaterials];

	for(DWORD i = 0; i < m_dwNumMaterials; ++i)
	{
		m_pMeshMaterials[i] = Mesh.m_pMeshMaterials[i];
		m_pMeshTextures[i] = Mesh.m_pMeshTextures[i];
	}

	m_pMesh = Mesh.m_pMesh;

	// Add reference to the ID3DXBaseMesh COM object since we're not explicitly creating one
	m_pMesh->AddRef();

	return *this;
}

CMesh::~CMesh()
{
	if(m_dwNumMaterials > 1)
	{
		for(size_t i = 0; i < m_dwNumMaterials; i++)
			CBase::SafeRelease(m_pMeshTextures[i], "Releasing Mesh Textures");
		
		CBase::SafeDeleteArray(m_pMeshMaterials);
		CBase::SafeDeleteArray(m_pMeshTextures);
	}
	else
	{
		CBase::SafeDelete(m_pMeshTextures, "Releasing Mesh Texture");
		CBase::SafeDelete(m_pMeshMaterials, "Releasing Mesh Material");
	}

	CBase::SafeRelease(m_pMesh, "Releasing Mesh Object");
}

HRESULT CMesh::CreateMeshFromX(LPDIRECT3DDEVICE9 pDevice, const char *FileName)
{
	LPD3DXBUFFER pD3DXMtrlBuffer;

	m_pDevice = pDevice;

    if(FAILED(D3DXLoadMeshFromX((char*)FileName, D3DXMESH_MANAGED, m_pDevice, NULL, 
                                   &pD3DXMtrlBuffer, NULL, &m_dwNumMaterials, &m_pMesh)))
    {
        return E_FAIL;
    }

	// Make sure normals are there, just in case
	m_pMesh->CloneMeshFVF(D3DXMESH_MANAGED, MeshFVF, m_pDevice, &m_pMesh);
	D3DXComputeNormals(m_pMesh, NULL);

	// Clone again for the shadow maesh
	m_pMesh->CloneMeshFVF(D3DXMESH_MANAGED, MeshFVF, m_pDevice, &m_pShadowMesh);
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
	MESHVERTEX *ptr;
	void *pVerts;

	// Get the verts
	m_pMesh->GetVertexBuffer(&pVB);
	pVB->Lock(0, 0, &pVerts, 0);

	ptr = (MESHVERTEX*)pVerts;
	for(DWORD i = 0; i < m_pShadowMesh->GetNumVertices(); i++)
        ptr[i].Colour = 0x80808080;
	
	pVB->Unlock();

    D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
    m_pMeshMaterials = new D3DMATERIAL9[m_dwNumMaterials];
    m_pMeshTextures  = new LPDIRECT3DTEXTURE9[m_dwNumMaterials];

    for(size_t i = 0; i < m_dwNumMaterials; i++)
    {
        m_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;
        m_pMeshMaterials[i].Ambient = m_pMeshMaterials[i].Diffuse;

        if(FAILED(D3DXCreateTextureFromFileEx(m_pDevice, d3dxMaterials[i].pTextureFilename, 
                                               0, 0, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, 
											   D3DX_DEFAULT, D3DX_FILTER_LINEAR, 0, NULL, NULL,
											   &m_pMeshTextures[i])))
		{
            m_pMeshTextures[i] = NULL;
        }
    }

    pD3DXMtrlBuffer->Release();

	return S_OK;
}

void CMesh::Transform(const CMatrix &pMat)
{
	m_MatTransform *= pMat;
}

void CMesh::SetTransform(CViewport *pViewport) const
{
	pViewport->SetWorld(m_MatTransform);
}

void CMesh::SetTransformAll(CViewport *pViewport)
{
	m_MatTransform *= m_MatScale * m_MatRotate * m_MatTrans;
	pViewport->SetWorld(m_MatTransform);
}

void CMesh::MultiplyTransform(CViewport *pViewport) const
{
	pViewport->MultiplyWorld(m_MatTransform);
}

void CMesh::MultiplyTransformAll(CViewport *pViewport)
{
	m_MatTransform *= m_MatScale * m_MatRotate * m_MatTrans;
	pViewport->MultiplyWorld(m_MatTransform);
}

void CMesh::ResetTransform()
{
	m_MatTransform = CMatrix::Identity();
}

void CMesh::Rotate()
{
	m_MatRotate = CMatrix::YawPitchRoll(m_Rotate.y, m_Rotate.x, m_Rotate.z);
	m_MatTransform *= m_MatRotate;
}

void CMesh::SetRotate(const float x, const float y, const float z)
{
	m_Rotate.x = x;
	m_Rotate.y = y;
	m_Rotate.z = z;

	m_MatRotate = CMatrix::YawPitchRoll(y, x, z);
}

void CMesh::Scale()
{
	m_MatScale = CMatrix::Scaling(m_Scale.x, m_Scale.y, m_Scale.z);
	m_MatTransform *= m_MatScale;
}

void CMesh::SetScale(const float x, const float y, const float z)
{
	m_Scale.x = x;
	m_Scale.y = y;
	m_Scale.z = z;

	m_vSphereCentre *= m_Scale.x;
	m_fSphereRadius *= m_Scale.x;

	m_MatScale = CMatrix::Scaling(x, y, z);
}

void CMesh::Translate()
{
	m_MatTrans = CMatrix::Translation(m_Trans.x, m_Trans.y, m_Trans.z);
	m_MatTransform *= m_MatTrans;
}

void CMesh::SetTranslate(const float x, const float y, const float z)
{
	m_Trans.x = x;
	m_Trans.y = y;
	m_Trans.z = z;

	// Update the bounding sphere centre
	m_vSphereCentre += m_Trans;

	m_MatTrans = CMatrix::Translation(x, y, z);
}

void CMesh::ComputeBoundingSphere()
{	
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
	void *pVerts;

	// Get the verts
	m_pMesh->GetVertexBuffer(&pVB);
	pVB->Lock(0, 0, &pVerts, 0);

	// Calculate bounding sphere
	D3DXComputeBoundingSphere((D3DXVECTOR3*)pVerts, m_pMesh->GetNumVertices(),
								D3DXGetFVFVertexSize(m_pMesh->GetFVF()), m_vSphereCentre, &m_fSphereRadius);

	pVB->Unlock();
}

void CMesh::RenderMesh(const CFrustum *pFrustum) const
{
	// Don't draw if not in frustum
	if(pFrustum->SphereIsInside(GetBoundingSphereCentre(), GetBoundingSphereRadius()))
	{
		for(DWORD i = 0; i < m_dwNumMaterials; i++)
		{
			m_pDevice->SetMaterial(&m_pMeshMaterials[i]);
			m_pDevice->SetTexture(0, m_pMeshTextures[i]);

			m_pMesh->DrawSubset(i);
		}
	}
}

void CMesh::RenderMeshShadow(const CFrustum *pFrustum) const
{
	for(DWORD i = 0; i < m_dwNumMaterials; i++)
	{
		m_pMesh->DrawSubset(i);
	}
}
