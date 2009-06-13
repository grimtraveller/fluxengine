/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#include "flx_geometry_mesh.h"
#include "../Core/flx_scene.h"
#include "../Core/flx_renderer.h"
#include "../MathLib/flx_math.h"

bool signum(float value)
{
	if( value < 0.0f ) return 1;
	else 		   return 0;
}	

#define SIGNUM(value) ((value < 0.0f) ? 1:0)

GeometryMesh::GeometryMesh(std::string _filename)
{
	pMesh.Load(_filename);
	mNumVertices = 0;

	Vector3 tmax  = Vector3(-999999,-999999,-999999);
	Vector3 tmin  = Vector3(999999,999999,999999);

	for(int j = 0; j < pMesh.m_pScene->mNumMeshes; ++j)
	{
		for(int i = 0; i < pMesh.m_pScene->mMeshes[j]->mNumVertices; ++i)
		{
			tmin.x = min(pMesh.m_pScene->mMeshes[j]->mVertices[i].x, tmin.x);
			tmin.y = min(pMesh.m_pScene->mMeshes[j]->mVertices[i].y, tmin.y);
			tmin.z = min(pMesh.m_pScene->mMeshes[j]->mVertices[i].z, tmin.z);

			tmax.x = max(pMesh.m_pScene->mMeshes[j]->mVertices[i].x, tmax.x);
			tmax.y = max(pMesh.m_pScene->mMeshes[j]->mVertices[i].y, tmax.y);
			tmax.z = max(pMesh.m_pScene->mMeshes[j]->mVertices[i].z, tmax.z);
		}
		mNumVertices += pMesh.m_pScene->mMeshes[j]->mNumVertices;
	}
	m_AABBox.vecMin = tmin;
	m_AABBox.vecMax = tmax;
}

GeometryMesh::~GeometryMesh()
{
}

void GeometryMesh::Update()
{

}

void GeometryMesh::RenderOGL(bool bTextured)
{
	if(m_bRenderAABBox)
		m_AABBox.render();

	pMesh.Draw(false, m_bRenderTextures);

	Flux::Renderer::getInstance()->mNumVisibleVertices += mNumVertices;
}



bool GeometryMesh::checkCollision(const Vector3& _point, Vector3 _offset, Vector3& _ret_offset)
{
	Vector3 vS, vV, vTV;
	float t, s1, s2, mscale;
	Vector3 vR, vQ1, vQ2;
	Vector3 vIntersect;
	Vector3 cV;

	Vector3 cdA[5];

	
	/*cdA[0] = Vector3( 0.5f, 0.0f, 0.0f);
	cdA[1] = Vector3( 0.0f, 0.5f, 0.0f);
	cdA[2] = Vector3( 0.0f, 0.0f, 0.5f); 
	cdA[3] = Vector3( 0.4f, 0.0f, 0.4f);
	cdA[4] = Vector3( -0.4f, 0.0f, 0.4f);
	*/
	

	cdA[0] = Vector3( 0.9f, 0.0f, 0.0f);
	cdA[1] = Vector3( 0.0f, 0.9f, 0.0f);
	cdA[2] = Vector3( 0.0f, 0.0f, 0.9f); 
	cdA[3] = Vector3( 0.8f, 0.0f, 0.8f);
	cdA[4] = Vector3( -0.8f, 0.0f, 0.8f);

	for(int i = 0; i < pMesh.m_pScene->mNumMeshes; ++i)
	{
		for(int j = 0; j < pMesh.m_pScene->mMeshes[i]->mNumFaces; ++j)
		{
			Vector3 pot0(
				pMesh.m_pScene->mMeshes[i]->mVertices[pMesh.m_pScene->mMeshes[i]->mFaces[j].mIndices[0]].x,
				pMesh.m_pScene->mMeshes[i]->mVertices[pMesh.m_pScene->mMeshes[i]->mFaces[j].mIndices[0]].y,
				pMesh.m_pScene->mMeshes[i]->mVertices[pMesh.m_pScene->mMeshes[i]->mFaces[j].mIndices[0]].z
				);

			Vector3 pot1(
				pMesh.m_pScene->mMeshes[i]->mVertices[pMesh.m_pScene->mMeshes[i]->mFaces[j].mIndices[1]].x,
				pMesh.m_pScene->mMeshes[i]->mVertices[pMesh.m_pScene->mMeshes[i]->mFaces[j].mIndices[1]].y,
				pMesh.m_pScene->mMeshes[i]->mVertices[pMesh.m_pScene->mMeshes[i]->mFaces[j].mIndices[1]].z
				);

			Vector3 pot2(
				pMesh.m_pScene->mMeshes[i]->mVertices[pMesh.m_pScene->mMeshes[i]->mFaces[j].mIndices[2]].x,
				pMesh.m_pScene->mMeshes[i]->mVertices[pMesh.m_pScene->mMeshes[i]->mFaces[j].mIndices[2]].y,
				pMesh.m_pScene->mMeshes[i]->mVertices[pMesh.m_pScene->mMeshes[i]->mFaces[j].mIndices[2]].z
				);

			pot0 = pot0 + _offset;
			pot1 = pot1 + _offset;
			pot2 = pot2 + _offset;


			//Calculate the normal of face plane
			Vector3 normal = cross(pot1-pot0, pot2-pot0);
			normal.normalize();

			float dist = -dot(normal, pot0);

			for(int k = 0; k < 5; ++k)
			{
				vS = _point + cdA[k];
				vV = _point - cdA[k];
				vTV = vV - vS;

				//distance to plane = dot(normal, point) + dist_origin
				
				s1 = dot(normal, vS) + dist;
				s2 = dot(normal, vV) + dist;

				if(SIGNUM(s1) !=  SIGNUM(s2))
				{
					t = -s1 / dot(normal, vTV);

					vIntersect = vS + (vTV*t);

					vR = vIntersect - pot0;
					vQ1 = pot1 - pot0;
					vQ2 = pot2 - pot0;

					float detp1 = dot(vQ1, vQ1) * dot(vQ2, vQ2);
					float detp2 = dot(vQ1, vQ2) * dot(vQ1, vQ2);

					mscale = 1.0f / (detp1 - detp2);

					float matrixA[4] = { 
						dot(vQ2, vQ2), -dot(vQ1, vQ2),
						-dot(vQ1, vQ2), dot(vQ1, vQ1)
					};

					float matrixB[2] = { dot(vR, vQ1), dot(vR, vQ2) };

					for(int l = 0; l < 4; ++l) matrixA[l] *= mscale;

					float w1 = matrixA[0] * matrixB[0] + matrixA[1] * matrixB[1];
					float w2 = matrixA[2] * matrixB[0] + matrixA[3] * matrixB[1];

					float w0 = 1.0f - w1 - w2;

					if(SIGNUM(w0) == 0 && SIGNUM(w1) == 0 && SIGNUM(w2) == 0)
					{
						Vector3 offset;

						if(t >= 0.0f) offset = vIntersect - vV;
						else			offset = vIntersect - vS;

						_ret_offset = offset;

						return true;
					}
				}
			}

		}
	}
	

	return false;
}