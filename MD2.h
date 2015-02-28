#ifndef _cmd2_h
#define _cmd2_h

#include "base.h"
#include <fstream>
#include <string>
#include "texture.h"

class CMD2
{
	struct tMD2Header
	{ 
		int Magic; 
		int Version; 
		int SkinWidth; 
		int SkinHeight; 
		int FrameSize; 
		int NumSkins; 
		int NumVertices; 
		int NumTexCoords; 
		int NumTriangles; 
		int NumGLCommands; 
		int NumFrames; 
		int OffsetSkins; 
		int OffsetTexCoords; 
		int OffsetTriangles; 
		int OffsetFrames; 
		int OffsetGlCommands; 
		int OffsetEnd; 
	} m_sHeader;

	struct tMD2TriVerts
	{
		byte Vertex[3];	
		byte LightNormalIndex;
	};

	struct tMD2TempFrame
	{
		float Scale[3];
		float Translate[3];
		char Name[16];
		tMD2TriVerts TriVerts[1];
	};

	struct tMD2D3DVerts
	{
		float x, y, z;
		float u, v;
	};

	struct tMD2Frames
	{
		char Name[16];
		tMD2D3DVerts *D3DVerts;
	};

	struct tMD2Triangles
	{
		short VertexIndex[3];
		short TextureIndex[3];
	};

	struct tMD2TexCoords
	{
		short u, v;
	};

	LPDIRECT3DDEVICE9 m_pDevice;
	LPDIRECT3DVERTEXBUFFER9 m_VB; // vertex buffer
	tMD2TexCoords *m_pTexCoords;
	tMD2Triangles *m_pTriangles;
	tMD2Frames *m_pFrames;
	CTexture m_Skin;
	static const int m_IDP2;
	static const int m_TGA;
	static const unsigned int m_Vertex_Decl;
	UCHAR m_Buffer[2048 * 4 + 128];

public:
	std::string m_pFilename;
	std::string m_SkinName;

	CMD2() {};
	CMD2(LPDIRECT3DDEVICE9 m_Device, const char *Filename, const char *Texture) { Load(m_Device, Filename, Texture); };
	//~CMd2();

	void Load(LPDIRECT3DDEVICE9 m_Device, const char *Filename, const char *Texture);
	void RenderFrame(float Time);
	void SetUpVerts(const int Time);
};

#endif