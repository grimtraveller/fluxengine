/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#ifndef GEOMETRYTERRAIN_H
#define GEOMETRYTERRAIN_H

#include "flx_terrain_patch.h"
#include "../Core/flx_core.h"
#include "../ObjectLib/flx_geometry_grid.h"
#include "../ObjectLib/flx_vertex3.h"
#include "../ObjectLib/flx_vertex2.h"
#include "../MathLib/flx_vector3.h"
#include "../MathLib/flx_plane.h"
#include "../Core/flx_ogl_buffer.h"
#include <string>
#include <fstream>
#include <vector>

struct vertex
{
	float x;
	float y;
	float z;
};

struct texcoord
{
	float u;
	float v;
};

class ENGINE_API GeometryTerrain : public Geometry
{
public:
	GeometryTerrain(int size, int patch_size, int num_hills = 100, float smoothness = 0.75f, int smooth_passes = 5);
	virtual ~GeometryTerrain();

	void Update();
	void RenderOGL(bool bTexture);

	void	computeNormals();
	Vector3 getNormal(int x, int z);
	Vector3 getNormalf(int x, int z);
	void	computeNormalsArea(int xs, int zs);
	void	buildPatches(bool update);
	void	computeLightmap(Vector3 _vlightSource, bool update = false);

	int		GetWidth() { return m_Width; }
	int		GetLength() { return m_Length; }
	void	setHeight(int x, int z, float y);
	void	SetTriangleHeight(int x, int z, float y);

	void			setPatchSize(unsigned int size);
	unsigned int	getPatchSize();

	float	getHeight(int x, int z);
	float	getHeightf(float x, float z);
	Vector3 getRayIntersection(Ray ray);
	void	Edit(int sx, int sz, float brush_size, float displacement);
	void	saveHeightmapToFile(const std::string& filename);
	void	loadHeightmapFromFile(const std::string& filename);

	float	getHeightAtPos(int x, int z);
	float	getRandomRange(float min, float max);
	void	generateTerrain(unsigned short u_numHills);
	void	addHill();

	void	smoothTerrain(float factor);
	void	smoothTerrain(float factor, int _x, int _z, int _size);

	char* GetTypeString() { return "Terrain"; }
protected:

public:
	flx_ogl_buffer<flx_Vertex>		VertexBuffer;
	flx_ogl_buffer<flx_Normal>		NormalBuffer;
	flx_ogl_buffer<flx_TexCoord>	TexCoordBuffer;
	flx_ogl_buffer<flx_Index>		IndexBuffer;

	std::vector<TerrainPatch>		m_vTerrainPatches;
	unsigned int					m_patchSize;
private:
	float m_fOffsetX;
	float m_fOffsetY;
	float m_fOffsetZ;

//private!
public:
	int m_Width, m_Length;
	float** m_pHeight;	//2D array, holds height(y) value for each x,y pos
	Vertex2* m_vT;		//vector for texture coordinates

	GLuint m_VertexVBO, m_NormalVBO;
	flx_Vertex*		m_pVertices;
	flx_TexCoord*	m_pTexCoords;
	flx_Normal*		m_pNormals;
	std::vector<unsigned int>	m_Indices;


	unsigned int m_nVBOVertices;
	unsigned int m_nVBOTexCoords;
	unsigned int m_nVBONormals;

	unsigned int m_nVertexCount;
};
#endif