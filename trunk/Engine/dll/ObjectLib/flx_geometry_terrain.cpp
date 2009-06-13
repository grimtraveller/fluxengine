#include "flx_geometry_terrain.h"
#include "../Core/ResourceManager/flx_resource_manager.h"
#include "../Core/flx_engine.h"
#include "../Core/ResourceManager/IL/il.h"

//Some "masterpiece"! :P

GeometryTerrain::GeometryTerrain(int size, int patch_size, int num_hills, float smoothness, int smooth_passes )
: m_Width(size), 
  m_Length(size),
  m_patchSize(patch_size)
{
	if(patch_size < 2) patch_size = 2;
	if(patch_size > size) patch_size = size;

	m_pHeight	= new float*[m_Width+1];

	m_fOffsetX = 1.0f;
	m_fOffsetY = 1.0f;
	m_fOffsetZ = 1.0f;

	for(int i = 0; i <= m_Length; i++)
	{		
		m_pHeight[i] = new float[m_Width+1];
		memset(m_pHeight[i], 0, sizeof(float)*(m_Length+1));
	}

	m_pVertices = NULL;

	m_nVertexCount = ((unsigned int)GetWidth()+1)*((unsigned int)GetLength()+1);

	//memset(m_pVertices, 0, m_nVertexCount);
	//memset(m_pNormals, 0, m_nVertexCount);


	m_pVertices		= new flx_Vertex[m_nVertexCount];
	m_pTexCoords	= new flx_TexCoord[m_nVertexCount];
	m_pNormals		= new flx_Normal[m_nVertexCount];

	//Add some random Hills and smooth it
	generateTerrain(num_hills);
	for(int i = 0; i < smooth_passes; ++i)
		smoothTerrain(smoothness);

	int deg_x = 0, deg_z = 0;
	for(int z = 0; z <= GetLength(); z++)
	{
		for(int x = 0; x <= GetWidth(); x++)
		{
			//fill Vertex array with data
			m_pVertices[x + z * (GetWidth()+1)].x = (float)x * m_fOffsetX;
			m_pVertices[x + z * (GetWidth()+1)].y = getHeight(x, z);
			m_pVertices[x + z * (GetWidth()+1)].z = (float)z * m_fOffsetZ;

			//fill TexCoord array with data
			m_pTexCoords[x + z * (GetWidth()+1)].u = (float)((float)x/(GetWidth()+1));
			m_pTexCoords[x + z * (GetWidth()+1)].v = (float)((float)z/(GetWidth()+1));
		}
	}

	//Create Indices
	for(int z = 0; z < GetLength()-1; z++)
	{
		//Even rows move left to right
		if(z % 2 == 0)
		{
			int x;
			for(x = 0; x < GetWidth(); x++)
			{
				m_Indices.push_back( x + (z * GetWidth()) );
				m_Indices.push_back( x + (z * GetWidth()) + GetWidth() );
			}

			if(z != GetLength() - 2 ) 
				m_Indices.push_back( --x + (z * GetWidth()) );
		}
		else
		{
			//odd rows move right to left
			int x;
			for(x = GetWidth() -1; x >= 0; x--)
			{
				m_Indices.push_back( x + (z * GetWidth()) );
				m_Indices.push_back( x + (z * GetWidth()) + GetWidth() );
			}

			if(z != GetLength() - 2 ) 
				m_Indices.push_back( ++x + (z * GetWidth()) );
		}
	}

	
	//Fill the buffers with data
	//VertexBuffer.setElementList(m_pVertices, m_nVertexCount);
	//TexCoordBuffer.setElementList(m_pTexCoords, m_nVertexCount);

	//...and calculate the Normals
	computeNormals();

	//Buffers ready to bind!
	//NormalBuffer.build(GL_ARRAY_BUFFER, GL_NORMAL_ARRAY);
	//VertexBuffer.build(GL_ARRAY_BUFFER, GL_VERTEX_ARRAY);
	//TexCoordBuffer.build(GL_ARRAY_BUFFER, GL_TEXTURE_COORD_ARRAY);
	
	buildPatches(0);

	//delete [] m_pVertices; m_pVertices = NULL;
	//delete [] m_pTexCoords; m_pTexCoords = NULL;
	//delete [] m_pNormals; m_pNormals = NULL;
}

GeometryTerrain::~GeometryTerrain()
{
	for(int i = 0; i <= m_Length; i++)
		delete[] m_pHeight[i];

	delete[] m_pHeight;

	delete [] m_pVertices; m_pVertices = NULL;
	delete [] m_pTexCoords; m_pTexCoords = NULL;
	delete [] m_pNormals; m_pNormals = NULL;

}




void GeometryTerrain::computeLightmap(Vector3 _vlightSource, bool update)
{
	bool bIntegrateNormals = false;
	std::vector<GLubyte> shadowMapTexture(GetWidth()*GetWidth()*4);
 
	float maxHeight = -99999.0f;

	for(int z =0; z <= GetLength(); ++z)
		for(int x = 0; x <= GetWidth(); ++x)
			maxHeight = max(getHeight(x,z), maxHeight);

	for(int z =0; z <= GetLength(); ++z)
	{
		for(int x = 0; x <= GetWidth(); ++x)
		{
			float ambientLight = 255;
			Ray lightRay(Vector3(x, getHeight(x, z), z), _vlightSource );
			Vector3 current_ray_pos(Vector3(x, getHeight(x, z), z));
			Vector3 direction_to_sun = lightRay.m_v3Direction.normalize();

			int numRayHits = 0;
			while(!(current_ray_pos.x <= 0 || current_ray_pos.x >= GetWidth() || current_ray_pos.z <= 0 || current_ray_pos.z >= GetWidth() ))
			{
				if(current_ray_pos.y > maxHeight) break;

				// Is the terrain blocking the ray at this point?
				if(getHeight((int)floor(current_ray_pos.x), (int)floor(current_ray_pos.z)) > current_ray_pos.y)
				{
					numRayHits++;
					break;
				}
				//light still traveling...
				current_ray_pos += direction_to_sun;	
			}

			float ambientLightNormals = 0;
			if(bIntegrateNormals)
			{
				Vector3 n(
					m_pNormals[x+z * (GetWidth()+1)].x, 
					m_pNormals[x+z * (GetWidth()+1)].y, 
					m_pNormals[x+z * (GetWidth()+1)].z
					);

				ambientLightNormals = 0.5*( 1.0f + dot(n.normalize(), direction_to_sun) );
				if(ambientLightNormals > 1.0f) ambientLightNormals = 1.0f;
				if(ambientLightNormals < 0.0f) ambientLightNormals = 0.0f;
			}

			if(numRayHits > 0)
			{
					//ambientLight = (current_ray_pos - Vector3(x,getHeight(x,z),z)).magnitude() - ambientLightNormals * 255;
					ambientLight = 170;
					if(ambientLight > 255) ambientLight = 255;
					if(ambientLight < 170) ambientLight = 170;
			}

			int index = (x + z * GetWidth()) * 3;
				for (int i = 0; i < 3; ++i) {

					shadowMapTexture[index + i] = (GLubyte)ambientLight;
				}
		}
	}
	
	for(int z =0; z <= GetLength(); ++z)
	{
		for(int x = 0; x <= GetWidth(); ++x)
		{
			int factor = 2;
			ColorOGL colCurrent;
			colCurrent.m_fRed = shadowMapTexture[(x + z * GetWidth()) * 3 + 0];
			colCurrent.m_fGreen = shadowMapTexture[(x + z * GetWidth()) * 3 + 1];
			colCurrent.m_fBlue = shadowMapTexture[(x + z * GetWidth()) * 3 + 2];

			ColorOGL colT;
			ColorOGL colD;
			ColorOGL colL;
			ColorOGL colR;

			if(shadowMapTexture.size() > ((x + (z+factor) * GetWidth()) * 3 + 0) &&
				shadowMapTexture.size() > ((x + (z+factor) * GetWidth()) * 3 + 1) &&
				shadowMapTexture.size() > ((x + (z+factor) * GetWidth()) * 3 + 2)
				)
			{
				colT.m_fRed = shadowMapTexture[(x + (z+factor) * GetWidth()) * 3 + 0];
				colT.m_fGreen = shadowMapTexture[(x + (z+factor) * GetWidth()) * 3 + 1];
				colT.m_fBlue = shadowMapTexture[(x + (z+factor) * GetWidth()) * 3 + 2];
			}

			if(shadowMapTexture.size() > ((x + (z-factor) * GetWidth()) * 3 + 0) &&
				shadowMapTexture.size() > ((x + (z-factor) * GetWidth()) * 3 + 1) &&
				shadowMapTexture.size() > ((x + (z-factor) * GetWidth()) * 3 + 2)
				)
			{
				colD.m_fRed = shadowMapTexture[(x + (z-factor) * GetWidth()) * 3 + 0];
				colD.m_fGreen = shadowMapTexture[(x + (z-factor) * GetWidth()) * 3 + 1];
				colD.m_fBlue = shadowMapTexture[(x + (z-factor) * GetWidth()) * 3 + 2];
			}

			if(shadowMapTexture.size() > ( (x+factor + z * GetWidth()) * 3 + 0) &&
				shadowMapTexture.size() > ((x+factor + z * GetWidth()) * 3 + 1) &&
				shadowMapTexture.size() > ((x+factor + z * GetWidth()) * 3 + 2)
				)
			{
				colL.m_fRed = shadowMapTexture[(x+factor + z * GetWidth()) * 3 + 0];
				colL.m_fGreen = shadowMapTexture[(x+factor + z * GetWidth()) * 3 + 1];
				colL.m_fBlue = shadowMapTexture[(x+factor + z * GetWidth()) * 3 + 2];
			}

			if(shadowMapTexture.size() > (( x-factor + z * GetWidth()) * 3 + 0) &&
				shadowMapTexture.size() > ((x-factor + z * GetWidth()) * 3 + 1) &&
				shadowMapTexture.size() > ((x-factor + z * GetWidth()) * 3 + 2)
				)
			{
				colR.m_fRed = shadowMapTexture[(x-factor + z * GetWidth()) * 3 + 0];
				colR.m_fGreen = shadowMapTexture[(x-factor + z * GetWidth()) * 3 + 1];
				colR.m_fBlue = shadowMapTexture[(x-factor + z * GetWidth()) * 3 + 2];
			}

			shadowMapTexture[(x + z * GetWidth()) * 3 + 0] = (colT.m_fRed+colD.m_fRed+colR.m_fRed+colL.m_fRed+colCurrent.m_fRed)/5;
			shadowMapTexture[(x + z * GetWidth()) * 3 + 1] = (colT.m_fGreen+colD.m_fGreen+colR.m_fGreen+colL.m_fGreen+colCurrent.m_fGreen)/5;
			shadowMapTexture[(x + z * GetWidth()) * 3 + 2] = (colT.m_fBlue+colD.m_fBlue+colR.m_fBlue+colL.m_fBlue+colCurrent.m_fBlue)/5;
			
		}
	}

	if(update)
	{
		ResourceManager::getInstance()->getTexture2D("resources/textures/lightmap.tga")->bind(2);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0, GetWidth(), GetWidth(), GL_RGBA, GL_UNSIGNED_BYTE, &shadowMapTexture[0]);
	}
	else
	{
		ilTexImage(GetWidth(),GetWidth(), 1, 3, IL_RGB, IL_UNSIGNED_BYTE, &shadowMapTexture[0]);
		ilSetData(&shadowMapTexture[0]);
		ilSetInteger(IL_IMAGE_BITS_PER_PIXEL,32);
		ilEnable(IL_FILE_OVERWRITE);
		ilSave(IL_TGA, "resources/textures/lightmap.tga");
	}
}


void GeometryTerrain::Update()
{
}

void GeometryTerrain::RenderOGL(bool bTexture)
{
	/*
	ResourceManager::getInstance()->getTexture2D("resources/textures/alpha.tga")->bind(0);
	ResourceManager::getInstance()->getTexture2D("resources/textures/tile_snow.png")->bind(1);
	ResourceManager::getInstance()->getTexture2D("resources/textures/grass.png")->bind(2);
	ResourceManager::getInstance()->getTexture2D("resources/textures/tile_rock0.png")->bind(3);
	ResourceManager::getInstance()->getTexture2D("resources/textures/lightmap.tga")->bind(4);


	//Binding the buffers...
	VertexBuffer.bind();
	NormalBuffer.bind();
	TexCoordBuffer.bind();
	//Ready! GO!
	glDrawElements(GL_TRIANGLE_STRIP, m_Indices.size(), GL_UNSIGNED_INT, &m_Indices[0] );

	//Unbind the buffers
	TexCoordBuffer.unbind();
	NormalBuffer.unbind();
	VertexBuffer.unbind();
	*/

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	for(unsigned int i = 0; i < m_vTerrainPatches.size(); ++i)
	{
		m_vTerrainPatches[i].render();
	}
	
}

void GeometryTerrain::setHeight(int x, int z, float y)
{
	if(x <= (GetWidth()+1) && z <= (GetWidth()+1) && x >= 0 && z >= 0)
		m_pHeight[z][x] = y;
}

void GeometryTerrain::SetTriangleHeight(int x, int z, float y)
{
	if(x <= GetWidth() && z <= GetLength() && x >= 0 && z >= 0)
	{
		m_pHeight[z][x] = y;
		m_pHeight[z][x+1] = y;
		m_pHeight[z+1][x] = y;
	}
}

float GeometryTerrain::getHeight(int x, int z)
{
	if(x <= ((GetWidth()+1) * m_fOffsetX) && z  <= ((GetWidth()+1) * m_fOffsetZ) && x >= 0 && z >= 0)
		return m_pHeight[z][x];

	else return 0;
}

void GeometryTerrain::Edit(int sx, int sz, float brush_size, float displacement)
{
	//std::vector<int> affectedPatchesIndex;

	for(int z = 0; z <= GetLength(); z++)
	{
		for(int x = 0; x <= GetWidth(); x++)
		{
			int xt = x-sx;
			int zt = z-sz;
			float d = sqrt((double)(xt*xt + zt*zt)) / brush_size ;

			if(fabs(d) <= 1.0f)
			{
				float newHeight = (displacement/2 + sin(d * Math::PI ) * displacement/2) * Flux::Engine::getInstance()->m_flDeltaTime*4;
				setHeight(x,z, getHeight(x,z)+ newHeight);
				m_pVertices[x + z * (GetWidth()+1)].y += newHeight;
				//affectedPatchesIndex.push_back();
			}
		}
	}

	//VertexBuffer.update(m_pVertices, m_nVertexCount);
	//NormalBuffer.update(m_pNormals, m_nVertexCount);
}

Vector3 GeometryTerrain::getRayIntersection(Ray ray)
{
	float t = 0;
	Vector3 intersection(0,0,0);

	for(int z = 0; z < GetLength() - 1; z++)
	{

		for(int x = 0; x < GetWidth(); x++)
		{
			Vector3 a((int)x	,	getHeight( (int)x	, (int)z   )	, (int)z); 
			Vector3 b((int)x+1	,	getHeight( (int)x+1	, (int)z )		, (int)z); 
			Vector3 c((int)x	,	getHeight( (int)x	, (int)z+1 )	, (int)z+1); 

			Plane plane(a,b,c);

			t = dot( plane.GetNormal(), (a - ray.m_v3Origin) ) / dot(plane.GetNormal(),  ray.m_v3Direction);
			intersection = ray.m_v3Origin + (ray.m_v3Direction * t);

			if(t > 0.0 || t < 0.0)
				return intersection;

		}
	}
	return intersection;
}


float GeometryTerrain::getHeightf(float x, float z)
{
	//Before returning the height values let's do some
	//bilinear interpolation (for smooth walking etc...)


	//z += 1; //hack!

	int Xa = x;
	int Xb = x+1;

	int Za = z;
	int Zb = z+1;

	float HTL = getHeight(Xa, Za);
	float HTR = getHeight(Xb, Za);
	float HBL = getHeight(Xa, Zb);
	float HBR = getHeight(Xb, Zb);


	float dx = (x - Xa) / (Xb - Xa);  //(Xb - Xa = 1)
	float dz = (z - Za) / (Zb - Za);  //(Zb - Za = 1)

	float lerpT = HTL + dx * (HTR - HTL);
	float lerpB = HBL + dx * (HBR - HBL); 
	float lerp  = lerpT + dz * (lerpB - lerpT);

	return lerp;
}

Vector3 GeometryTerrain::getNormal(int x, int z) 
{
	return Vector3(m_pNormals[x+z * (GetWidth()+1)].x,m_pNormals[x+z * (GetWidth()+1)].y,m_pNormals[x+z * (GetWidth()+1)].z);
}


Vector3 GeometryTerrain::getNormalf(int x, int z)
{
	int Xa = x;
	int Xb = x+1;

	int Za = z;
	int Zb = z+1;

	Vector3 HTL = getNormal(Xa, Za);
	Vector3 HTR = getNormal(Xb, Za);
	Vector3 HBL = getNormal(Xa, Zb);
	Vector3 HBR = getNormal(Xb, Zb);

	float dx = (x - Xa) / (Xb - Xa);  //(Xb - Xa = 1)
	float dz = (z - Za) / (Zb - Za);  //(Zb - Za = 1)

	Vector3 lerpT = HTL + dx * (HTR - HTL);
	Vector3 lerpB = HBL + dx * (HBR - HBL); 
	Vector3 lerp  = lerpT + dz * (lerpB - lerpT);

	return lerp;
}

void GeometryTerrain::generateTerrain(unsigned short u_numHills)
{
	srand( time(NULL) );
	for( int i = 0; i < u_numHills; ++i )
	{
		addHill();
	}
}

void GeometryTerrain::addHill()
{
	float radius = getRandomRange(0, 100.0f);

	float x, y;

	x = getRandomRange(-radius, GetWidth() + radius);
	y = getRandomRange(-radius, GetLength() + radius);

	float radius_sq = sqrt(radius * radius);
	float dist_sq, height;

	int x_min = (int)x - (int)radius -1;
	int x_max = (int)x + (int)radius +1;

	if(x_min < 0) x_min = 0;
	if(x_max >= GetWidth() ) x_max = GetWidth()-1;

	int y_min = (int)y - (int)radius -1;
	int y_max = (int)y + (int)radius +1;

	if(y_min < 0) y_min = 0;
	if(y_max >= GetLength() ) y_max = GetLength() -1;

	for(int ix = x_min; ix <= x_max; ++ix)
	{
		for(int iz = y_min; iz <= y_max; ++iz)
		{
			dist_sq = sqrt((x - ix) * (x - ix) + (y - iz) * (y - iz));
			height = (radius_sq - dist_sq) / 2;

			if(height > 0)
				setHeight(ix, iz, getHeight(ix, iz) + height);
		}
	}
}

float GeometryTerrain::getRandomRange( float min, float max )
{
	return( ( rand() * ( max - min ) / RAND_MAX ) + min );
}


void GeometryTerrain::computeNormals() {

	for(unsigned int z = 0; z < (GetWidth()+1)-1; ++z) 
	{
		for(unsigned int x = 0; x < (GetWidth()+1); ++x) 
		{
			Vector3 sum(0.0f, 0.0f, 0.0f);

			Vector3 out;
			if (z > 0) {
				out = Vector3(0.0f, m_pHeight[z - 1][x] - m_pHeight[z][x], -1.0f);
			}
			Vector3 in;
			if (z < (GetWidth()+1) - 1) {
				in = Vector3(0.0f, m_pHeight[z + 1][x] - m_pHeight[z][x], 1.0f);
			}
			Vector3 left;
			if (x > 0) {
				left = Vector3(-1.0f, m_pHeight[z][x - 1] - m_pHeight[z][x], 0.0f);
			}
			Vector3 right;
			if (x < (GetWidth()+1) - 1) {
				right = Vector3(1.0f, m_pHeight[z][x + 1] - m_pHeight[z][x], 0.0f);
			}


			if (x > 0 && z > 0) {
				sum += out.cross(left);
			}
			if (x > 0 && z < (GetWidth()+1) - 1) {
				sum += left.cross(in);
			}
			if (x < (GetWidth()+1) - 1 && z < (GetWidth()+1) - 1) {
				sum += in.cross(right);
			}
			if (x < (GetWidth()+1) - 1 && z > 0) {
				sum += right.cross(out);
			}

			m_pNormals[x+z * (GetWidth()+1)].x = sum.x;
			m_pNormals[x+z * (GetWidth()+1)].y = sum.y;
			m_pNormals[x+z * (GetWidth()+1)].z = sum.z;
		}
	}

	//Fill the Normal Buffer with Data
	NormalBuffer.setElementList(m_pNormals, m_nVertexCount);
}


void GeometryTerrain::computeNormalsArea(int xs, int zs) {

	/*Vector3** normals = new Vector3*[GetLength()];

	for(int i = 0; i < GetLength(); i++) 
	{
	normals[i] = new Vector3[GetWidth()];
	}
	*/

	for(unsigned int z = zs; z-10 < zs+10; ++z) 
	{
		for(unsigned int x = xs-10; x < xs+10; ++x) 
		{
			Vector3 sum(0.0f, 0.0f, 0.0f);

			Vector3 out;
			if (z > 0) {
				out = Vector3(0.0f, m_pHeight[z - 1][x] - m_pHeight[z][x], -1.0f);
			}
			Vector3 in;
			if (z < GetLength() - 1) {
				in = Vector3(0.0f, m_pHeight[z + 1][x] - m_pHeight[z][x], 1.0f);
			}
			Vector3 left;
			if (x > 0) {
				left = Vector3(-1.0f, m_pHeight[z][x - 1] - m_pHeight[z][x], 0.0f);
			}
			Vector3 right;
			if (x < GetWidth() - 1) {
				right = Vector3(1.0f, m_pHeight[z][x + 1] - m_pHeight[z][x], 0.0f);
			}


			if (x > 0 && z > 0) {
				sum += out.cross(left);
			}
			if (x > 0 && z < GetLength() - 1) {
				sum += left.cross(in);
			}
			if (x < GetWidth() - 1 && z < GetLength() - 1) {
				sum += in.cross(right);
			}
			if (x < GetWidth() - 1 && z > 0) {
				sum += right.cross(out);
			}


			m_pNormals[x+z * GetWidth()].x = sum.x;
			m_pNormals[x+z * GetWidth()].y = sum.y;
			m_pNormals[x+z * GetWidth()].z = sum.z;
		}
	}
	//NormalBuffer.setElementList(m_pNormals, m_nVertexCount);
}

void GeometryTerrain::smoothTerrain(float factor)
{
	//A common algrorithm I've found

	/* Rows, left to right */
	for(int x = 1; x < GetWidth(); x++)
		for (int z = 0; z < GetLength(); z++)
			setHeight(x, z, getHeight(x - 1, z) * (1 - factor) + getHeight(x, z) * factor);

	/* Rows, right to left*/
	for(int x = GetWidth() - 2; x < -1; x--)
		for (int z = 0;z < GetLength(); z++)
			setHeight(x, z, getHeight(x + 1, z) * (1 - factor) + getHeight(x, z) * factor);

	/* Columns, bottom to top */
	for(int x = 0; x < GetWidth(); x++)
		for (int z = 1;z < GetLength(); z++)
			setHeight(x, z, getHeight(x, z - 1) * (1 - factor) + getHeight(x, z) * factor);

	/* Columns, top to bottom */
	for(int x = 0;x < GetWidth(); x++)
		for (int z = GetLength(); z < -1; z--)
			setHeight(x, z, getHeight(x, z + 1) * (1 - factor) + getHeight(x, z) * factor);

	//We changed the Normals, so we've got to re-calculate normals 
	//for lighting after each pass
	computeNormals();
}

void GeometryTerrain::smoothTerrain(float factor, int _x, int _z, int _size)
{
	int begin_x = _x - _size;
	int end_x	= _x + _size;
	int begin_z = _z - _size;
	int end_z	= _z + _size;

	//A common algrorithm I've found

	//if(begin_x > GetWidth()+1 || begin_x-2 < 0 || begin_z > GetLength()+1 || begin_z < 0
	//	|| end_x > GetWidth()+1 || end_x < 0 || end_z > GetLength()+1 || end_z < 0)
	//	return;
	

		for(int x = begin_x; x < end_x; x++)
			for (int z = begin_z; z < end_z; z++)
			{
				setHeight(x, z, getHeight(x - 1, z) * (1 - factor) + getHeight(x, z) * factor);
			}

		for(int x = begin_x - 2; x < -1; x--)
			for (int z = begin_z; z < end_z; z++)
				setHeight(x, z, getHeight(x + 1, z) * (1 - factor) + getHeight(x, z) * factor);

		for(int x = begin_x; x < end_x; x++)
			for (int z = begin_z; z < end_z; z++)
				setHeight(x, z, getHeight(x, z - 1) * (1 - factor) + getHeight(x, z) * factor);

		for(int x = begin_x;x < end_x; x++)
			for (int z = end_z; z < begin_z; z--)
				setHeight(x, z, getHeight(x, z + 1) * (1 - factor) + getHeight(x, z) * factor);

	for(int z = 0; z <= GetLength(); z++)
	{
		for(int x = 0; x <= GetWidth(); x++)
		{
			m_pVertices[x + z * (GetWidth()+1)].y = getHeight(x, z);
		}
	}
}


struct data
{
	float height;
};

void GeometryTerrain::saveHeightmapToFile(const std::string& filename)
{
	std::ofstream terrain_file (filename.c_str(), std::ios::out | std::ios::binary);

	for(int z = 0; z < GetLength(); z++)
	{
		for(int x = 0; x < GetWidth(); x++)
		{
			data t_data;
			t_data.height = getHeight(x, z);
			terrain_file.write ((const char*)&t_data, sizeof(data));
		}
	}
	terrain_file.close();
}

void GeometryTerrain::loadHeightmapFromFile(const std::string& filename)
{

	std::fstream terrain_file (filename.c_str(), std::ios::in | std::ios::binary);

	data t_data;

	for(int z = 0; z < GetLength(); z++)
	{
		for(int x = 0; x < GetWidth(); x++)
		{
			terrain_file.read((char*)&t_data, sizeof (data));
			setHeight(x, z, t_data.height);
		}
	}
	terrain_file.close();

	m_pVertices		= new flx_Vertex[m_nVertexCount];
	m_pTexCoords	= new flx_TexCoord[m_nVertexCount];
	m_pNormals		= new flx_Normal[m_nVertexCount];

	for(int z = 0; z <= GetLength(); z++)
	{
		for(int x = 0; x <= GetWidth(); x++)
		{
			//fill Vertex array with data
			m_pVertices[x + z * (GetWidth()+1)].x = (float)x;
			m_pVertices[x + z * (GetWidth()+1)].y = getHeight(x, z);
			m_pVertices[x + z * (GetWidth()+1)].z = (float)z;

			//fill TexCoord array with data
			m_pTexCoords[x + z * (GetWidth()+1)].u = (float)((float)x/(GetWidth()+1));
			m_pTexCoords[x + z * (GetWidth()+1)].v = (float)((float)z/(GetWidth()+1));
		}
	}
	

	//Let's compute the normals for each vertex
	computeNormals();

	buildPatches(1);
}

void GeometryTerrain::buildPatches(bool update)
{
	int currentPatchIndex = 0;

		//Iterate through the base grid and divide it into chunks
		for(unsigned int patchZ = 0; patchZ < GetWidth(); patchZ += m_patchSize)
		{
			for(unsigned int patchX = 0; patchX < GetWidth(); patchX += m_patchSize)
			{
				TerrainPatch new_patch;

				new_patch.m_vertexCount = (m_patchSize+1)*(m_patchSize+1);

				flx_Vertex		*patchVertices	= new flx_Vertex[new_patch.m_vertexCount];
				flx_TexCoord	*patchTexCoords	= new flx_TexCoord[new_patch.m_vertexCount];
				flx_Normal		*patchNormals	= new flx_Normal[new_patch.m_vertexCount];

				Vector3 tmax  = Vector3(-999999,-999999,-999999);
				Vector3 tmin  = Vector3(999999,999999,999999);

				//Fill vertex, texcoord and normal arrays for their buffers
				for(unsigned int z = 0; z < m_patchSize+1; ++z)
				{
					unsigned int x;
					for(x = 0; x < m_patchSize+1; ++x)
					{
						patchVertices[x + z * (m_patchSize+1)].x = m_pVertices[(x + patchX) + (z + patchZ) * (GetWidth()+1)].x;
						patchVertices[x + z * (m_patchSize+1)].y = m_pVertices[(x + patchX) + (z + patchZ) * (GetWidth()+1)].y;
						patchVertices[x + z * (m_patchSize+1)].z = m_pVertices[(x + patchX) + (z + patchZ) * (GetWidth()+1)].z;

						if(!update)
						{
							patchTexCoords[x + z * (m_patchSize+1)].u = (float)((float)((x + patchX) * m_fOffsetX) / (GetWidth()+1));
							patchTexCoords[x + z * (m_patchSize+1)].v = (float)((float)((z + patchZ) * m_fOffsetZ) / (GetWidth()+1));
						}

						patchNormals[x + z * (m_patchSize+1)].x =  m_pNormals[(x + patchX) + (z + patchZ) * (GetWidth()+1)].x;
						patchNormals[x + z * (m_patchSize+1)].y =  m_pNormals[(x + patchX) + (z + patchZ) * (GetWidth()+1)].y;
						patchNormals[x + z * (m_patchSize+1)].z =  m_pNormals[(x + patchX) + (z + patchZ) * (GetWidth()+1)].z;

						//Find the corners for the AABB
						tmin.x = min(patchVertices[x + z * (m_patchSize+1)].x, tmin.x);
						tmin.y = min(patchVertices[x + z * (m_patchSize+1)].y, tmin.y);
						tmin.z = min(patchVertices[x + z * (m_patchSize+1)].z, tmin.z);
						tmax.x = max(patchVertices[x + z * (m_patchSize+1)].x, tmax.x);
						tmax.y = max(patchVertices[x + z * (m_patchSize+1)].y, tmax.y);
						tmax.z = max(patchVertices[x + z * (m_patchSize+1)].z, tmax.z);
					}		
				}

				//Set the corners of AABB
				new_patch.m_aabb.vecMax = tmax;
				new_patch.m_aabb.vecMin = tmin;

				//Create the indices and degenerates
				unsigned int nvert = m_patchSize+1;
				int z;
				if(!update)
				{
					for(int lod_level = 1; lod_level <= 1; ++lod_level)
					{
						int offset = lod_level;
						for(z = 0; z < nvert-1; z++)
						{
							if(z % 2)
							{
								//move right to left
								int x;
								for(x = nvert-1; x > 0; x--)
								{
									new_patch.m_Indices[lod_level-1].push_back(x + (z + 1) * nvert);
									new_patch.m_Indices[lod_level-1].push_back(x + (z    ) * nvert -1);
								}
								//put in some degenerates
								new_patch.m_Indices[lod_level-1].push_back( (z + 1) * nvert );
								new_patch.m_Indices[lod_level-1].push_back( (z + 1) * nvert );
							}
							else
							{
								//move from the left to the right
								int x;
								for(x = 0; x < nvert; x++)
								{
									new_patch.m_Indices[lod_level-1].push_back(x + (z    ) * nvert );
									new_patch.m_Indices[lod_level-1].push_back(x + (z + 1) * nvert );
								}
								//put in some degenerates
								new_patch.m_Indices[lod_level-1].push_back( (z + 2) * nvert - 1 );
								new_patch.m_Indices[lod_level-1].push_back( (z + 2) * nvert - 1 );
							}
						}
					}
				}
				//new_patch.m_Indices.push_back(new_patch.m_Indices[new_patch.m_Indices.size()-1]);

				//Tell the buffers to use the data we've just created

				if(!update)
				{
					new_patch.VertexBuffer.setElementList(patchVertices, new_patch.m_vertexCount);
					new_patch.TexCoordBuffer.setElementList(patchTexCoords, new_patch.m_vertexCount);
					new_patch.NormalBuffer.setElementList(patchNormals, new_patch.m_vertexCount);

					new_patch.VertexBuffer.build(GL_ARRAY_BUFFER, GL_VERTEX_ARRAY);
					new_patch.TexCoordBuffer.build(GL_ARRAY_BUFFER, GL_TEXTURE_COORD_ARRAY);
					new_patch.NormalBuffer.build(GL_ARRAY_BUFFER, GL_NORMAL_ARRAY);
				}

				if(update)
				{
					m_vTerrainPatches[currentPatchIndex].m_aabb = new_patch.m_aabb;
					m_vTerrainPatches[currentPatchIndex].VertexBuffer.update(patchVertices, new_patch.m_vertexCount);
					m_vTerrainPatches[currentPatchIndex].NormalBuffer.update(patchNormals, new_patch.m_vertexCount);
				}

				if(!update)
					m_vTerrainPatches.push_back(new_patch);

				//Get back my memory! We don't use those arrays anymore...
				delete [] patchVertices; patchVertices = NULL;
				delete [] patchTexCoords; patchTexCoords = NULL;
				delete [] patchNormals; patchNormals = NULL;
				currentPatchIndex++;
			}
		}
}

unsigned int GeometryTerrain::getPatchSize()
{ return m_patchSize; }

void GeometryTerrain::setPatchSize(unsigned int size)
{
	if(size > GetWidth())	size = GetWidth();
	if(size <= 0)			size = 1;

	m_patchSize = size;
}