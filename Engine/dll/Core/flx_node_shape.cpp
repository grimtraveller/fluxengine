/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

//Now, this is nothing but chaotic, experimental crap. There are tons of lines
//which SHOULD NOT be implemented in this class.

#include "flx_node_shape.h"
#include "flx_scene.h"
#include "flx_renderer.h"
#include "flx_engine.h"
#include "../Core/flx_logger.h"
#include "../Core/flx_application.h"
#include "../Core/ResourceManager/IL/il.h"

using namespace Flux;

int c = 0;
GLubyte *data;
float intensity = 5;
float texmove = 1.0f;
NodeShape::NodeShape(Geometry*  pGeometry)
: m_pGeometry(pGeometry)
{
	m_Type					= SHAPE;
	m_pMaterial				= NULL;
	m_pShader				= NULL;
	m_ignoreFrustumCulling	= false;

	for(int i = 0; i < 7; i++)
	{
		m_pTexture[i] = NULL;
	}

	m_pTextureTransform = NULL;

	if(strstr(this->m_pGeometry->GetTypeString(), "Terrain") != 0)
	{
			data = (GLubyte*)malloc(1024*1024*4);
			//memset(data, 0, 1024*1024*4);
	}
}

NodeShape::~NodeShape()
{
	free(data);
}


void NodeShape::RenderOGL(bool bSecondPass)
{
	
	register float dist_to_eye = 0;
	if(!ignoresFrustumCulling())
	{
		if(Renderer::getInstance()->GetCurrentScene()->GetFrustum().IsBoxInFrustum(
			AABBox(
					m_pGeometry->m_AABBox.vecMax + (dynamic_cast<NodeTransform*>(m_pOut)->m_v3Translate),
					m_pGeometry->m_AABBox.vecMin + (dynamic_cast<NodeTransform*>(m_pOut)->m_v3Translate))
					))
		{
			//if(Renderer::getInstance()->GetCurrentScene()->GetFrustum().IsBoxInFrustum( m_pGeometry->m_AABBox.transform(m_pGeometry->m_AABBox, view)))
			glGetFloatv(GL_MODELVIEW_MATRIX, &m_Matrix.m[0]);
	
			dist_to_eye =	(m_Matrix.m[12] * m_Matrix.m[12] +
				m_Matrix.m[13] * m_Matrix.m[13] +
				m_Matrix.m[14] * m_Matrix.m[14]);


			
			if(dist_to_eye > 380*380)	
			{
				//if(m_pShader != NULL)
				//	m_pShader->Deactivate();
				return;
			}
			

		}
		else
		{

			if(m_pShader != NULL)
				m_pShader->Deactivate();
			return;
		}
		this->m_bVisible = false;
	}

	this->m_bVisible = true;

	//if(!ignoresFrustumCulling() && strstr(this->m_pGeometry->GetTypeString(), "Mesh") != 0 && dist_to_eye < 250)
	if(!ignoresFrustumCulling() && strstr(this->m_pGeometry->GetTypeString(), "Mesh") != 0)
	{
		if(AABBox(
			m_pGeometry->m_AABBox.vecMax + (dynamic_cast<NodeTransform*>(m_pOut)->m_v3Translate),
			m_pGeometry->m_AABBox.vecMin + (dynamic_cast<NodeTransform*>(m_pOut)->m_v3Translate)).isInBox(
			Flux::Player::getInstance()->getPosition()))
		{
			Vector3 cam_offset(0,0,0);
			if(((GeometryMesh*)this->m_pGeometry)->checkCollision(
				Flux::Player::getInstance()->getPosition(),
				(dynamic_cast<NodeTransform*>(m_pOut)->m_v3Translate),
				cam_offset
				))
			{
				//Flux::Player::getInstance()->setCurrentVelocity(0.5, 0.5, 0.5);
				Flux::Player::getInstance()->setPosition(Vector3(
					Flux::Player::getInstance()->getPosition().x + cam_offset.x,
					Flux::Player::getInstance()->getPosition().y + cam_offset.y,
					Flux::Player::getInstance()->getPosition().z + cam_offset.z
					)
					);
				Flux::Player::getInstance()->m_bCollides = true;
			}
			else
			{
				Flux::Player::getInstance()->m_bCollides = false;
			}
		}
	}

	if(Flux::Engine::getInstance()->m_bterrainPaint)
	{
		if(strstr(this->m_pGeometry->GetTypeString(), "Terrain") != 0)
		{
			ResourceManager::getInstance()->getTexture2D("resources/textures/alpha.tga")->bind(0);
		
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

			int u,v;

			if(Engine::getInstance()->m_mouseCoord3D.x > 0 && Engine::getInstance()->m_mouseCoord3D.x < 1024 &&
				Engine::getInstance()->m_mouseCoord3D.z > 0 && Engine::getInstance()->m_mouseCoord3D.z < 1024)
			{
				u = Engine::getInstance()->mouse3dx_2_uv(1024, 1024);
				v = Engine::getInstance()->mouse3dy_2_uv(1024, 1024);
				int radius = Flux::Engine::getInstance()->getTerrainBrushSize()*8;


				for(int y = -radius; y < radius; y++)
				{
					for(int x = -radius; x < radius; x++)
					{

						float dist_sq = sqrt((float)(u - (x*128)) * (float)(u - (x*128)) + (float)(v - (y*128)) * (float)(v - (y*128)));
						float opacity = fabs((sqrt((float)radius*(float)radius) - dist_sq)/60);
						opacity = fabs(opacity - 128);
						//float opacity = Engine::getInstance()->m_iterrainPaintOpacity;
						if(opacity > 255 ) opacity = 255;
						if(opacity < 0) opacity = 0;

						//if(data[(u+x + (v+y)*1024)*3 + Engine::getInstance()->m_iterrainPaintLayer] > 0)
						if(dist_sq < radius*radius*2)
							Engine::getInstance()->write_uv_byte_2_texture_mem(data, opacity, Engine::getInstance()->m_iterrainPaintLayer, u+x, v+y, 1024, 4);

					}
				}
			}

			glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0, 1024,1024, GL_RGBA, GL_UNSIGNED_BYTE, data);

		}

	}


	if(Flux::Engine::getInstance()->m_bKeys[VK_KEYJ])
	{
		if(data != NULL)
		{
			ilTexImage(1024,1024, 1, 4, IL_RGBA, IL_UNSIGNED_BYTE, data);
			ilSetData(data);
			ilSetInteger(IL_IMAGE_BITS_PER_PIXEL,32);
			ilEnable(IL_FILE_OVERWRITE);
			ilSave(IL_TGA, "resources/textures/alpha.tga");
		}
	}

	static bool bObjectSolid = true;

	
	/*if(Flux::Engine::getInstance()->m_bLButtonDown)
	{
		//if(	AABBox(
		//			m_pGeometry->m_AABBox.vecMax + (dynamic_cast<NodeTransform*>(m_pOut)->m_v3Translate),
		//			m_pGeometry->m_AABBox.vecMin + (dynamic_cast<NodeTransform*>(m_pOut)->m_v3Translate)
		//			).ClipSegment(Flux::Engine::getInstance()->m_mouseRay.m_v3Origin, Flux::Engine::getInstance()->m_mouseRay.m_v3Direction))
		//{
		if(	AABBox(
					m_pGeometry->m_AABBox.vecMax + (dynamic_cast<NodeTransform*>(m_pOut)->m_v3Translate),
					m_pGeometry->m_AABBox.vecMin + (dynamic_cast<NodeTransform*>(m_pOut)->m_v3Translate)
					).intersectionRay(Flux::Engine::getInstance()->m_mouseRay))
		{	
			//m_pGeometry->m_bRenderAABBox	= true;
			//m_pGeometry->m_bSelected		= true;
		}
		else
		{
			m_pGeometry->m_bRenderAABBox	= false;
			m_pGeometry->m_bSelected		= false;
		}
	}
	*/
	


	if(m_pMaterial)
	{
		if( (m_pMaterial->m_f4Ambient[3] < 0.997f	) ||
			(m_pMaterial->m_f4Diffuse[3] < 0.997f	) ||
			(m_pMaterial->m_f4Specular[3] < 0.997f	) ||
			(m_pMaterial->m_f4Emmissive[3] < 0.997f	))
		{
			glEnable(GL_BLEND);
			bObjectSolid = false;
		}

		glMaterialfv(GL_FRONT, GL_AMBIENT,		&m_pMaterial->m_f4Ambient[0]);
		glMaterialfv(GL_FRONT, GL_DIFFUSE,		&m_pMaterial->m_f4Diffuse[0]);
		glMaterialfv(GL_FRONT, GL_SPECULAR,		&m_pMaterial->m_f4Specular[0]);
		glMaterialfv(GL_FRONT, GL_EMISSION,		&m_pMaterial->m_f4Emmissive[0]);
		glMaterialf (GL_FRONT, GL_SHININESS,	 m_pMaterial->m_fShininess);
	}
	else
	{
		//default material value (grey)
		static float f4Temp[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &f4Temp[0]);

		f4Temp[0] = f4Temp[1] = f4Temp[2] = 0.8f;
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &f4Temp[0]);

		f4Temp[0] = f4Temp[1] = f4Temp[2] = 0.5f;
		GLfloat mShininess[] = { 20 };
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,		&f4Temp[0]);
		f4Temp[0] = f4Temp[1] = f4Temp[2] = 0.3f;
		f4Temp[3] = 0.0f;
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,		&f4Temp[0]);
		glMaterialfv (GL_FRONT_AND_BACK, GL_SHININESS,	mShininess);
	}

	if(bObjectSolid || bSecondPass)
	{
		glEnable(GL_BLEND);
		if(m_pTextureTransform)
		{
			glMatrixMode(GL_TEXTURE);
			glPushMatrix();

			glLoadIdentity();

			//Move the texture
			glTranslatef(m_pTextureTransform->m_f2Translate[0], m_pTextureTransform->m_f2Translate[1], 0);
			glTranslatef(m_pTextureTransform->m_f2Centre[0], m_pTextureTransform->m_f2Centre[1], 0);
			//Rotate it
			glRotatef(m_pTextureTransform->m_fRotate, 0.0f, 0.0f, 1.0f);
			//Scale it
			glScalef(m_pTextureTransform->m_f2Scale[0], m_pTextureTransform->m_f2Scale[1], 1.0f);
			//and correct by offset to centre
			glTranslatef(-m_pTextureTransform->m_f2Centre[0], -m_pTextureTransform->m_f2Centre[1], 0);
		}

		if(this->m_pOut->isTransform() && this->m_bVisible)
		{
			if(m_pShader != NULL)
			{
				if(dist_to_eye < 350*350)
				{
					//glDisable(GL_LIGHTING);
					m_pGeometry->m_bRenderTextures = true;
					m_pShader->Activate();
				}
				else
				{
					glDisable(GL_LIGHTING);
					m_pGeometry->m_bRenderTextures = false;
					m_pShader->Deactivate();
				}

				if(!this->m_node_name.compare("terrain"))
				{
					m_pShader->Uniform("cursorPosition", Flux::Engine::getInstance()->m_mouseCoord3D.x / 1024, Flux::Engine::getInstance()->m_mouseCoord3D.z / 1024);
					m_pShader->Uniform("cursorSize", (Flux::Engine::getInstance()->getTerrainBrushSize()) /1024);
				}
				
				if(!this->m_node_name.compare("Skydome"))
				{
					float g_tint[4] = {0.64f, 0.74f, 0.9f, 1.0f};
					float g_fog[4];
					g_fog[0] = g_tint[0];
					g_fog[1] = g_tint[1];
					g_fog[2] = g_tint[2];
					g_fog[3] = g_tint[3];
					glFogfv(GL_FOG_COLOR, g_fog);
					ResourceManager::getInstance()->getTexture2D("resources/textures/cloud.png")->bind(0);
					m_pShader->UniformTexture("s_texture_0", 0);
					ResourceManager::getInstance()->getTexture2D("resources/textures/cloud.png")->bind(1);
					m_pShader->UniformTexture("s_texture_1", 1);



					m_pShader->Uniform("time", (float)Flux::Engine::getInstance()->m_flTime*2);
					m_pShader->Uniform("intense", intensity);
					m_pShader->Uniform("horizon", g_tint);

					if(Flux::Engine::getInstance()->m_bKeys[VK_KEYNUMPLUS]) intensity += 1.0f;
					if(Flux::Engine::getInstance()->m_bKeys[VK_KEYNUMMINUS]) intensity -= 1.0f;
				
				}

				if(!this->m_node_name.compare("water"))
				{
					texmove += 0.01f;
					ResourceManager::getInstance()->getTexture2D("resources/textures/water_reflection.tga")->bind(0);
					m_pShader->UniformTexture("water_reflection", 0);
					ResourceManager::getInstance()->getTexture2D("resources/textures/water_refraction.tga")->bind(1);
					m_pShader->UniformTexture("water_refraction", 1);
					ResourceManager::getInstance()->getTexture2D("resources/textures/water_depthmap.tga")->bind(4);
					m_pShader->UniformTexture("water_depthmap", 4);

					float lightposition[4] = {
					Flux::Renderer::getInstance()->GetCurrentScene()->m_vLights[0]->GetPosition().x,
					Flux::Renderer::getInstance()->GetCurrentScene()->m_vLights[0]->GetPosition().y,
					Flux::Renderer::getInstance()->GetCurrentScene()->m_vLights[0]->GetPosition().z,
					1};
					/*float viewposition[4] = {
						Flux::Player::getInstance()->getPosition().x,
					Flux::Player::getInstance()->getPosition().y,
					Flux::Player::getInstance()->getPosition().z,
					1};
					*/
					float viewposition[4] = {
					1,
					1,
					1,
					1};

						float waterColor[4] = { 1.0, 1.0, 1.0, 1.0 };
						float waterDepth[4] = { 1.0, 1.0, 1.0, 1.0 };


					ResourceManager::getInstance()->getTexture2D("resources/textures/w_normalmap.bmp")->bind(2);
					m_pShader->UniformTexture("water_normal", 2);
					ResourceManager::getInstance()->getTexture2D("resources/textures/w_distortion1.bmp")->bind(3);
					m_pShader->UniformTexture("water_dudvmap", 3);
					m_pShader->Uniform("lightpos", lightposition);
					m_pShader->Uniform("viewpos", viewposition);
					m_pShader->Uniform("time", texmove);
					m_pShader->Uniform("time2", -texmove);
					m_pShader->Uniform("waterColor", waterColor);
					m_pShader->Uniform("waterColor", waterDepth);
				}

				
			}
			m_pGeometry->RenderOGL(true);
			Renderer::getInstance()->GetCurrentScene()->m_iVisible++;

			if(m_pShader != NULL)
				m_pShader->Deactivate();


			//glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_TEXTURE_2D);
		}
		

	}
	else
	{
		if(bObjectSolid || bSecondPass)
			if(this->m_pOut->isTransform() && this->m_bVisible)
			{
				m_pGeometry->RenderOGL(false);
				Renderer::getInstance()->GetCurrentScene()->m_iVisible++;

				if(m_pShader != NULL)
					m_pShader->Deactivate();
			}
	}

	if(m_pShader != NULL)
		m_pShader->Deactivate();

	//save this object to be rendered on 2nd pass
	if(!bObjectSolid && !bSecondPass)
	{
		AlphaObject* pObject;
		pObject = new AlphaObject();
		if(pObject)
		{
			pObject->pShape = this;

			//get current values for model view matrix so that
			//we know  where to draw this shape on 2nd pass
			glGetFloatv(GL_MODELVIEW_MATRIX, &pObject->f16Matrix[0]);

			//calculate distance
			pObject->fDist2Eye =	pObject->f16Matrix[12] * pObject->f16Matrix[12] +
				pObject->f16Matrix[13] * pObject->f16Matrix[13] +
				pObject->f16Matrix[14] * pObject->f16Matrix[14];

			static bool bPlaced = false;
			for(std::vector<AlphaObject*>::iterator it = s_vAlphaObjects->begin(); it != s_vAlphaObjects->end(); ++it)
			{
				if(pObject->fDist2Eye > (*it)->fDist2Eye)
				{
					s_vAlphaObjects->insert(it, pObject);
					bPlaced = true;
					break;
				}
			}

			if(!bPlaced)
				s_vAlphaObjects->push_back(pObject);
		}
	}

	if(m_pNext)
		m_pNext->RenderOGL(bSecondPass);
		if(m_pShader != NULL)
		m_pShader->Deactivate();

}

inline bool NodeShape::ignoresFrustumCulling()
{ return m_ignoreFrustumCulling; }

void NodeShape::ignoresFrustumCulling(bool ignore)
{ m_ignoreFrustumCulling = ignore; }

void NodeShape::Update()
{ }
