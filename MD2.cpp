#include "MD2.h"

// MD2 file magic (an int containing "IDP2")
const int CMD2::m_IDP2 = ('I' + ('D' << 8) + ('P' << 16) + ('2' << 24));

// TGA tag to append to skin names
const int CMD2::m_TGA = ('.' + ('t' << 8) + ('g' << 16) + ('a' << 24));

const unsigned int CMD2::m_Vertex_Decl = D3DFVF_XYZ | D3DFVF_TEX1;

void CMD2::Load(LPDIRECT3DDEVICE9 m_Device, const char *Filename, const char *Texture)
{
	m_pDevice = m_Device;
	m_pFilename = Filename;
	m_SkinName = Texture;

	// Open MD2 file and check if it opened correctly
	std::ifstream MD2File(Filename, std::ios::binary);

	if(!MD2File.is_open())
	{	
		Log.LogFile << "Error opening " << Filename << std::endl;
		throw("Error loading MD2 file");
	}

	// Read in header
	MD2File.read((char *)&m_sHeader, sizeof(m_sHeader));

	// Make sure it's an MD2 file
	if(m_sHeader.Magic != m_IDP2 || m_sHeader.Version != 8)
	{
		Log.LogFile << "Not a valid MD2 file." << std::endl;
		throw("Not a valid MD2 file");
	}

	// Create the texture
	m_Skin.CreateTextureFromFile(m_pDevice, m_SkinName.c_str());

	// Extract the TexCoords
	m_pTexCoords = new tMD2TexCoords[m_sHeader.NumTexCoords];
	MD2File.seekg(m_sHeader.OffsetTexCoords, std::ios::beg);
	MD2File.read((char *)m_pTexCoords, sizeof(tMD2TexCoords) * m_sHeader.NumTexCoords);

	// Extract the triangles
	m_pTriangles = new tMD2Triangles[m_sHeader.NumTriangles];
	MD2File.seekg(m_sHeader.OffsetTriangles, std::ios::beg);
	MD2File.read((char *)m_pTriangles, sizeof(tMD2Triangles) * m_sHeader.NumTriangles);

	// Allocate memory for all the frames
	m_pFrames = new tMD2Frames[m_sHeader.NumFrames];

	MD2File.seekg(m_sHeader.OffsetFrames, std::ios::beg);
	for(int f = 0; f < m_sHeader.NumFrames; f++)
	{
		tMD2TempFrame *pTempFrame = (tMD2TempFrame*)m_Buffer;

		m_pFrames[f].D3DVerts = new tMD2D3DVerts[m_sHeader.NumVertices];

		// Extract the frame into our buffer
		MD2File.read((char *)pTempFrame, m_sHeader.FrameSize);

		// Now we need to transform the verts of the triangle verts using this frame (also swapping y and z)
		for(int i = 0; i < m_sHeader.NumVertices; i++)
		{
			m_pFrames[f].D3DVerts[i].x = (pTempFrame->TriVerts[i].Vertex[0] * pTempFrame->Scale[0]) + 
											pTempFrame->Translate[0];

			m_pFrames[f].D3DVerts[i].z = (pTempFrame->TriVerts[i].Vertex[1] * pTempFrame->Scale[1]) + 
											pTempFrame->Translate[1];

			m_pFrames[f].D3DVerts[i].y = (pTempFrame->TriVerts[i].Vertex[2] * pTempFrame->Scale[2]) + 
											pTempFrame->Translate[2];
		}
	}

	// Close the file, we no longer need it
	MD2File.close();

	// Create the vertex buffer
	m_pDevice->CreateVertexBuffer(m_sHeader.NumTriangles * 3 * sizeof(tMD2D3DVerts), 0, m_Vertex_Decl, D3DPOOL_MANAGED, &m_VB, NULL);
}

void CMD2::SetUpVerts(const int Frame)
{	

}

void CMD2::RenderFrame(float used)
{
	tMD2D3DVerts *p_Verts;

	static int Frame = 0;
	static float Time = 0.0f;

	Time += 0.05f;
	
	if (Time > 1.0f) 
	{	
		Time = 0.05f;
		Frame++;
	}

    m_VB->Lock(0, 0, (void**)&p_Verts, 0);

	for(int t = 0; t < m_sHeader.NumTriangles; t++)
	{
		for(int v = 0; v < 3; v++)
		{
			int index = m_pTriangles[t].VertexIndex[v];
			p_Verts[t*3+v].x = m_pFrames[Frame].D3DVerts[index].x + 
								(m_pFrames[Frame+1].D3DVerts[index].x - m_pFrames[Frame].D3DVerts[index].x) * Time;
			p_Verts[t*3+v].y = m_pFrames[Frame].D3DVerts[index].y + 
								(m_pFrames[Frame+1].D3DVerts[index].y - m_pFrames[Frame].D3DVerts[index].y) * Time;
			p_Verts[t*3+v].z = m_pFrames[Frame].D3DVerts[index].z + 
								(m_pFrames[Frame+1].D3DVerts[index].z - m_pFrames[Frame].D3DVerts[index].z) * Time;
			
			int texIndex = m_pTriangles[t].TextureIndex[v];
			p_Verts[t*3+v].u = m_pTexCoords[texIndex].u / (float)m_sHeader.SkinWidth;
			p_Verts[t*3+v].v = m_pTexCoords[texIndex].v / (float)m_sHeader.SkinHeight;
		}
	}

	m_VB->Unlock();

    m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	// Draw our triangle.
	m_pDevice->SetStreamSource(0, m_VB, 0, sizeof(tMD2D3DVerts));
	m_pDevice->SetFVF(m_Vertex_Decl);
	m_Skin.SetTexture(0);

	m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_sHeader.NumTriangles);
}
