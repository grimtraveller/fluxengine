/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#include "flx_resource_mesh.h"
#include "../../Core/flx_engine.h"

ResourceMesh2::ResourceMesh2()
{
	m_pScene = NULL;
}

ResourceMesh2::~ResourceMesh2()
{
	delete VertexBuffer;
	delete NormalBuffer;
	delete TexCoordBuffer;
	delete ColorBuffer;
	//delete vIndexBuffers;
	//delete m_Textures;

}

void ResourceMesh2::Load(std::string _filename)
{
	
	FLX_LOGGER->Write(LOG_DEFAULT, "Trying to load mesh %s...", _filename.c_str());
	
	aiSetImportPropertyFloat(AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE, 45.f);

	m_pScene = (aiScene*)aiImportFile(_filename.c_str(),
		aiProcess_CalcTangentSpace		   | // calculate tangents and bitangents if possible
		aiProcess_JoinIdenticalVertices    | // join identical vertices/ optimize indexing
		aiProcess_Triangulate			   | // triangulate polygons with more than 3 edges
		aiProcess_GenSmoothNormals		   | // generate smooth normal vectors if not existing
		//aiProcess_ConvertToLeftHanded	   | // convert everything to D3D left handed space
		aiProcess_SplitLargeMeshes         | // split large, unrenderable meshes into submeshes
		aiProcess_ValidateDataStructure    | // perform a full validation of the loader's output
		aiProcess_ImproveCacheLocality     | // improve the cache locality of the output vertices
		aiProcess_RemoveRedundantMaterials | // remove redundant materials
		aiProcess_SortByPType              | // make 'clean' meshes which consist of a single typ of primitives
		aiProcess_FindDegenerates          | // remove degenerated polygons from the import
		aiProcess_FindInvalidData          | // detect invalid model data, such as invalid normal vectors
		aiProcess_GenUVCoords              | // convert spherical, cylindrical, box and planar mapping to proper UVs
		aiProcess_TransformUVCoords        | // preprocess UV transformations (scaling, translation ...)
		//aiProcess_FindInstances            | // search for instanced meshes and remove them by references to one master
		aiProcess_LimitBoneWeights         | // limit bone weights to 4 per vertex
		aiProcess_PreTransformVertices |
		0);

	if(m_pScene == NULL)
	{
		FLX_LOGGER->Write(LOG_DEFAULT, "[FAILED]\n");
		return;
	}

	FLX_LOGGER->Write(LOG_DEFAULT, "[DONE]\n");

	VertexBuffer	= new flx_ogl_buffer<flx_Vertex>[m_pScene->mNumMeshes];
	NormalBuffer	= new flx_ogl_buffer<flx_Normal>[m_pScene->mNumMeshes];
	TexCoordBuffer	= new flx_ogl_buffer<flx_TexCoord>[m_pScene->mNumMeshes];
	ColorBuffer		= new flx_ogl_buffer<flx_VertexColor>[m_pScene->mNumMeshes];


	for(unsigned int nMat = 0; nMat < m_pScene->mNumMaterials; ++nMat)
	{
		aiTextureType t_MatType;
		t_MatType = aiTextureType_DIFFUSE;
		unsigned int t_MatIndex = 0;
		aiString t_MatPath;
		m_pScene->mMaterials[nMat]->GetTexture(t_MatType, t_MatIndex, &t_MatPath);

		if(t_MatPath.length > 0)
		{
			
			FLX_LOGGER->Write(LOG_DEFAULT, "Mesh %s: Loading Material %s\n", _filename.c_str(), t_MatPath.data);
			m_Textures.push_back((ResourceTexture2D*)ResourceManager::getInstance()->loadResource(
				ResourceManager::getInstance()->FLX_TEXTURE2D, 
				ResourceManager::getInstance()->getTexturePath()+t_MatPath.data)
				);
		}
	}
	FLX_LOGGER->Write(LOG_DEFAULT, "Mesh %s: Loaded Materials[%d]\n", _filename.c_str(), m_pScene->mNumMaterials);


	for(unsigned int m = 0; m < m_pScene->mNumMeshes; ++m)
	{
		//Fill the Vertex array
		flx_Vertex* m_pVertices = new flx_Vertex[m_pScene->mMeshes[m]->mNumVertices];
		for(int i = 0; i < m_pScene->mMeshes[m]->mNumVertices; i++)
		{
			m_pVertices[i].x = m_pScene->mMeshes[m]->mVertices[i].x;
			m_pVertices[i].y = m_pScene->mMeshes[m]->mVertices[i].y;
			m_pVertices[i].z = m_pScene->mMeshes[m]->mVertices[i].z;
		}


		//Fill the Normals array
		flx_Normal* m_pNormals = new flx_Normal[m_pScene->mMeshes[m]->mNumVertices];
		for(int i = 0; i < m_pScene->mMeshes[m]->mNumVertices; i++)
		{
			m_pNormals[i].x = m_pScene->mMeshes[m]->mNormals[i].x;
			m_pNormals[i].y = m_pScene->mMeshes[m]->mNormals[i].y;
			m_pNormals[i].z = m_pScene->mMeshes[m]->mNormals[i].z;
		}

		//Fill Vertex color array
		flx_VertexColor* m_pColors = new flx_VertexColor[m_pScene->mMeshes[m]->mNumVertices];
		for(int i = 0; i < m_pScene->mMeshes[m]->mNumVertices; i++)
		{
			if(m_pScene->mMeshes[m]->HasVertexColors(0))
			{
				m_pColors[i].r = m_pScene->mMeshes[m]->mColors[0][i].r;
				m_pColors[i].g = m_pScene->mMeshes[m]->mColors[0][i].g;
				m_pColors[i].b = m_pScene->mMeshes[m]->mColors[0][i].b;
				m_pColors[i].a = m_pScene->mMeshes[m]->mColors[0][i].a;
			}
			else
			{
				m_pColors[i].r = 1.0f;
				m_pColors[i].g = 1.0f;
				m_pColors[i].b = 1.0f;
				m_pColors[i].a = 1.0f;
			}
		}

		//Fill the Texture coord array
		flx_TexCoord* m_pTexCoords = new flx_TexCoord[m_pScene->mMeshes[m]->mNumVertices];

			for(int i = 0; i < m_pScene->mMeshes[m]->mNumVertices; i++)
			{
				if(m_pScene->mMeshes[m]->HasTextureCoords(0))
				{
					m_pTexCoords[i].u = m_pScene->mMeshes[m]->mTextureCoords[0][i].x;
					m_pTexCoords[i].v = m_pScene->mMeshes[m]->mTextureCoords[0][i].y;
				}
				else
				{
					m_pTexCoords[i].u = 0.5f;
					m_pTexCoords[i].v = 0.5f;
				}
			}



			std::vector<flx_Index> vIndices;
			for(unsigned int i = 0; i < m_pScene->mMeshes[m]->mNumFaces; ++i)
			{
				flx_Index t_indices[3];
				for(unsigned int a = 0; a < 3; ++a)
				{
					flx_Index t_index = m_pScene->mMeshes[m]->mFaces[i].mIndices[a];
					t_indices[a] = t_index;
					vIndices.push_back(t_index);
				}	
			}
			flx_ogl_buffer<flx_Index> buffer;
			buffer.setElementList(&vIndices[0], vIndices.size());
			buffer.build(GL_ELEMENT_ARRAY_BUFFER, 0);
			vIndexBuffers.push_back(buffer);

			VertexBuffer[m].setElementList(m_pVertices, m_pScene->mMeshes[m]->mNumVertices);
			NormalBuffer[m].setElementList(m_pNormals, m_pScene->mMeshes[m]->mNumVertices);
			TexCoordBuffer[m].setElementList(m_pTexCoords, m_pScene->mMeshes[m]->mNumVertices);
			
			if(m_pScene->mMeshes[m]->HasTextureCoords(0)) 
			{
				ColorBuffer[m].setElementList(m_pColors, m_pScene->mMeshes[m]->mNumVertices);
				ColorBuffer[m].build(GL_ARRAY_BUFFER, GL_COLOR_ARRAY);
			}

			VertexBuffer[m].build(GL_ARRAY_BUFFER, GL_VERTEX_ARRAY);
			NormalBuffer[m].build(GL_ARRAY_BUFFER, GL_NORMAL_ARRAY);
			TexCoordBuffer[m].build(GL_ARRAY_BUFFER, GL_TEXTURE_COORD_ARRAY);
			

				
		}

		FLX_LOGGER->Write(LOG_DEFAULT, "Mesh %s: Loaded successfully\n", _filename.c_str());
	}


	void ResourceMesh2::Draw(bool _selected, bool _renderTextures)
	{
		glDisable(GL_CULL_FACE);
		
	/*	if(_selected)
			glBlendFunc(GL_ONE, GL_ONE);
		else
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
*/
		for(unsigned int m = 0; m < m_pScene->mNumMeshes; ++m)
		{
			if(m_pScene->mMeshes[m]->mMaterialIndex < m_Textures.size())
				if(!m_Textures.empty())
					m_Textures[m_pScene->mMeshes[m]->mMaterialIndex]->bind(0);

			VertexBuffer[m].bind();
			NormalBuffer[m].bind();

			if(m_pScene->mMeshes[m]->HasTextureCoords(0)) 
					TexCoordBuffer[m].bind();

			//if(m_pScene->mMeshes[m]->HasVertexColors(0)) 
			//	ColorBuffer[m].bind();

			vIndexBuffers[m].bind();
			glDrawRangeElements(GL_TRIANGLES, 0, VertexBuffer[m].getLength(), vIndexBuffers[m].getLength(), GL_UNSIGNED_INT, 0);
			vIndexBuffers[m].unbind();

			NormalBuffer[m].unbind();

			if(m_pScene->mMeshes[m]->HasTextureCoords(0)) 
				TexCoordBuffer[m].unbind();

			VertexBuffer[m].unbind();

			//if(m_pScene->mMeshes[m]->HasVertexColors(0)) 
			//	ColorBuffer[m].unbind();
		}
	}