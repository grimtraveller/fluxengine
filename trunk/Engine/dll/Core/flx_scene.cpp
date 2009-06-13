/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#include "flx_scene.h"
#include "flx_renderer.h"
#include "flx_engine.h"
#include "flx_application.h"
using namespace Flux;

//std::vector<Object*> objects;

double DELTA_T = 0.0000000002f;
const double G = 9.81;
int bla =0;

std::vector<AlphaObject*>* Node::s_vAlphaObjects = NULL;
					float* fmpos = new float[2];
	
Scene::Scene()
{
	Node::s_vAlphaObjects	= &m_vAlphaObjects;
	m_bSceneInitialized		= false;
	m_iVisible				= 0;
	m_Frustum.EnableFrustum(true);
}


unsigned int Scene::AddNode(Node *pNode, unsigned int iNestedID)
{
	if(pNode == NULL)
	{
		FLX_LOGGER->Write(LOG_ERROR, "Can not add Node to scene.");
		return 0;
	}

	//check for nested node
	if(iNestedID > (unsigned int)m_vNodes.size())
	{
		FLX_LOGGER->Write(LOG_ERROR, "Can not add this Node under NodeID=%u.\n"
			"NodeID=%u does not exist.", iNestedID, iNestedID);

		delete pNode;

		return 0;
	}

	if(iNestedID == 0)
	{
		//place this node in the root, this node must be a transform node
		if(!pNode->isTransform())
		{
			FLX_LOGGER->Write(LOG_ERROR, "Can not add this node to root. Only\n"
				"Transform nodes are allowed in the root.");

			delete pNode;

			return 0;
		}

		//adjust pointers if needed
		if(m_vNodes.size() != 0)
		{
			Node* pTemp;
			pTemp = m_vNodes[0];

			//move through all existing nodes in the root to the end
			while(pTemp->m_pNext != NULL)
				pTemp = pTemp->m_pNext;

			//correct pointers
			pNode->m_pPrev = pTemp;
			pTemp->m_pNext = pNode;
		}
	}
	else
	{
		Node* pTemp;
		pTemp = m_vNodes[iNestedID -1];

		if(!pTemp->isTransform())
		{
			FLX_LOGGER->Write(LOG_ERROR, "Can not add this node.");

			return 0;
		}

		if(pTemp->m_pIn == NULL)
		{
			pTemp->m_pIn = pNode;
			pNode->m_pOut = pTemp;
		}
		else
		{
			pTemp = pTemp->m_pIn;

			while(pTemp->m_pNext != NULL) {
				pTemp = pTemp->m_pNext;
			}

			pNode->m_pPrev = pTemp;
			pTemp->m_pNext = pNode;
		}
	}

	m_vNodes.push_back(pNode);

	return ((unsigned int)m_vNodes.size());
}

int Scene::AddLight(Light *pLight)
{
	m_vLights.push_back(pLight);
	return ((int)m_vLights.size() -1);
}

unsigned int Scene::SaveGeometry(Geometry* pGeometry)
{
	if(pGeometry == NULL)
		return 0;

	m_vGeometries.push_back(pGeometry);

	return ((unsigned int)m_vGeometries.size());
}


void Scene::DeleteAllGeometries()
{
	for(std::vector<Geometry*>::iterator it = m_vGeometries.begin(); it != m_vGeometries.end(); ++it)
	{
		delete (*it);
	}

	m_vGeometries.clear();
}

void Scene::DeleteAllNodes()
{
	for(std::vector<Node*>::iterator it = m_vNodes.begin(); it != m_vNodes.end(); ++it)
	{
		if(*it != NULL)
			delete (*it);
	}

	m_vNodes.clear();
}

void Scene::DeleteAllTextureTransforms()
{
	for(std::vector<TextureTransform*>::iterator it = m_vTextureTransforms.begin(); it != m_vTextureTransforms.end(); ++it)
	{
		if(*it != NULL)
		{
			*it = NULL;
			delete (*it);
		}
	}

	m_vNodes.clear();
}

unsigned int Scene::SaveTextureTransform(TextureTransform *pTextureTransform)
{
	if(pTextureTransform == NULL)
	{
		FLX_LOGGER->Write(LOG_ERROR, "Invalid TextureTransform pointer");
		return 0;
	}

	m_vTextureTransforms.push_back(pTextureTransform);

	return ((unsigned int)m_vTextureTransforms.size());
}

unsigned int Scene::SaveMaterial(Material* pMaterial)
{
	if(pMaterial == NULL)
		return 0;

	m_vMaterials.push_back(pMaterial);

	return((unsigned int)m_vMaterials.size());
}

void Scene::DeleteAllMaterials()
{
	for(std::vector<Material*>::iterator it = m_vMaterials.begin(); it != m_vMaterials.end(); ++it)
	{
		if(*it != NULL)
			delete(*it);
	}

	m_vMaterials.clear();
}

bool Scene::AssignMaterial(unsigned int iShapeID, unsigned iMaterialID)
{
	if(iMaterialID > (unsigned int)m_vMaterials.size())
		return false;

	if(iShapeID > (unsigned int)m_vNodes.size())
		return false;

	if( !m_vNodes[iShapeID -1]->isShape())
		return false;

	((NodeShape*)m_vNodes[iShapeID-1])->m_pMaterial = m_vMaterials[iMaterialID - 1];

	return true;
}

bool Scene::AssignShader(unsigned int iShapeID, ResourceShader* shader)
{
	if(iShapeID > (unsigned int)m_vNodes.size())
	{
		FLX_LOGGER->Write(LOG_ERROR, "ShapeID > Node size");
		return false;
	}

	if( !m_vNodes[iShapeID - 1]->isShape())
	{
		FLX_LOGGER->Write(LOG_ERROR, "Node is no Shape!");
		return false;
	}

	if(shader == NULL)
		return false;
	shader->Activate();

				if(!((NodeShape*)m_vNodes[iShapeID -1])->m_node_name.compare("terrain"))
				{
					shader->Uniform("time", (float)Flux::Engine::getInstance()->m_flDeltaTime);
					shader->Uniform("cursorPosition", fmpos);
					shader->Uniform("cursorSize", (Flux::Engine::getInstance()->getTerrainBrushSize()) / 1024);
					shader->UniformTexture("s_texture_alpha0", 0);
					shader->UniformTexture("s_texture_alpha1", 1);
					shader->UniformTexture("s_texture_shadow", 2);

					shader->UniformTexture("s_texture0", 3);
					shader->UniformTexture("s_texture1", 4);
					shader->UniformTexture("s_texture2", 5);
					shader->UniformTexture("s_texture3", 6);
					shader->UniformTexture("s_texture4", 7);
					shader->UniformTexture("s_texture5", 8);
					
				}

	((NodeShape*)m_vNodes[iShapeID -1])->m_pShader = shader;



	return true;
}


bool Scene::AssignTexture(unsigned int iShapeID, const std::string& TextureIdentifier, int tex_id)
{
	if(iShapeID > (unsigned int)m_vNodes.size())
	{
		FLX_LOGGER->Write(LOG_ERROR, "ShapeID > Node size");
		return false;
	}

	if( !m_vNodes[iShapeID - 1]->isShape())
	{
		FLX_LOGGER->Write(LOG_ERROR, "Node is no Shape!");
		return false;
	}

	//((NodeShape*)m_vNodes[iShapeID -1])->m_pTexture[tex_id] = static_cast<ResourceTexture*>(Flux::Engine::getInstance()->ResourceMgr->GetResource(TextureIdentifier))->GetTexture();

	return true;
}

bool Scene::AssignTextureTransform(unsigned int iShapeID, unsigned int iTextureTransformID)
{
	if(iTextureTransformID > (unsigned int)m_vTextureTransforms.size())
	{
		FLX_LOGGER->Write(LOG_ERROR, "TextureTransform ID does not exist!");
		return false;
	}

	if(iShapeID > (unsigned int)m_vNodes.size())
	{
		FLX_LOGGER->Write(LOG_ERROR, "Shape ID does not exist!");
		return false;
	}

	if( !m_vNodes[iShapeID - 1]->isShape() )
	{
		FLX_LOGGER->Write(LOG_ERROR, "Node is not a Shape");
		return false;
	}

	((NodeShape*)m_vNodes[iShapeID - 1])->m_pTextureTransform = m_vTextureTransforms[iTextureTransformID -1];

	return true;
}

void Scene::DeleteAllAlphaObjects() {
	for(int i = 0; i < (int)m_vAlphaObjects.size(); ++i)
	{
		delete m_vAlphaObjects[i];
	}

	m_vAlphaObjects.clear();
}

void Scene::DeleteAllLights()
{
	for(std::vector<Light*>::iterator it = m_vLights.begin(); it != m_vLights.end(); ++it)
	{
		if(*it != NULL)
			delete (*it);
	}
	m_vLights.clear();
}

void Scene::Update(float deltaTime)
{
	if(!m_bSceneInitialized)
		FLX_LOGGER->Write(LOG_ERROR, "Scene was not initialized!");

	
	Matrix4x4 matView, matProj;
	glGetFloatv(GL_MODELVIEW_MATRIX, &matView.m[0]);
	glGetFloatv(GL_PROJECTION_MATRIX, &matProj.m[0]);
	m_Frustum.extract(matView, matProj);
	Update();


	m_iVisible = 0;
	Flux::Renderer::getInstance()->mNumVisibleVertices = 0;
	
	LightsEnabled();
	Render();

	switch (Flux::Renderer::getInstance()->GetRendererType())
	{
	case R_OPENGL: 
		//clear 2nd render pass container
		DeleteAllAlphaObjects();

		if(m_vNodes.size() > 0)
			this->m_vNodes[0]->RenderOGL(false); 
		else
			exit(0);
		

		glEnable(GL_BLEND);
		glMatrixMode(GL_MODELVIEW);

		for(std::vector<AlphaObject*>::iterator it = m_vAlphaObjects.begin(); it!= m_vAlphaObjects.end(); ++it)
		{
			glPushMatrix();
			glLoadMatrixf(&(*it)->f16Matrix[0]);

			(*it)->pShape->RenderOGL(true);

			glPopMatrix();
		}
		glDisable(GL_BLEND);

		break;
	}


	fmpos[0] = Flux::Engine::getInstance()->m_mouseCoord3D.x / 1024;
	fmpos[1] = Flux::Engine::getInstance()->m_mouseCoord3D.z / 1024;
	LightsDisabled();
}


void Scene::LightsEnabled()
{
	if((int)m_vLights.size() == 0)
		return; //there aren't any lights

	//reset counter
	m_iLightCounter = 0;

	//glEnable(GL_LIGHTING);

	Light* pLight = NULL;
	for(int iLightID = 0; iLightID < (int)m_vLights.size(); iLightID++)
	{
		if(m_iLightCounter >= pLight->s_iMaxNumLights)
		{
			//GPU can't handle that amount of lights!
			//todo: error implemention
			break;
		}

		pLight = dynamic_cast<Light*>( m_vLights[iLightID] );
		if(pLight->isOn())
			pLight->Render(m_iLightCounter++);

		m_iLightCounter++;
	}
}

void Scene::RunThread(void* instance)
{
	static_cast<Scene*>(instance)->Initialize();
}


void Scene::LightsDisabled()
{
	if((int)m_vLights.size() == 0)
		return; //there aren't any lights

	//glDisable(GL_LIGHTING);

	//reset counter
	m_iLightCounter = 0;

	Light* pLight;
	for(int iLightID = 0; iLightID < (int)m_vLights.size(); iLightID++)
	{
		if(m_iLightCounter >= pLight->s_iMaxNumLights)
		{
			//GPU can't handle that amount of lights!
			//todo: error implemention
			break;
		}

		pLight = dynamic_cast<Light*>( m_vLights[iLightID] );
		if(pLight->isOn())
			pLight->Disable(m_iLightCounter++);

		m_iLightCounter++;
	}
}


void Scene::PlayerUpdate(float fTime)
{
	//Player::getInstance()->Update(fTime);	
}

void Scene::ToggleHeadLight()
{
	if((int)m_vLights.size() == 0)
		return;
}

void Scene::HUD()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	
	glTranslated(Flux::Engine::getInstance()->getMousePosition().x, -Flux::Engine::getInstance()->getMousePosition().y + Flux::Engine::getInstance()->GetWindowHeight()-50, 0);

	
	ResourceManager::getInstance()->getTexture2D("resources/textures/cursor.png")->bind(0);

	glColor4f(1,1,1,1);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	

	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex2f(0,	0);
		glTexCoord2f(1, 0);
		glVertex2f(25, 0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(25, 25);
		glTexCoord2f(0, 1.0f);
		glVertex2f(0, 25);
	glEnd();

	glEnable(GL_DEPTH_TEST);

}

bool Scene::ClearAll()
{
	DeleteAllLights();
	DeleteAllGeometries();
	DeleteAllNodes();
	DeleteAllMaterials();
	DeleteAllTextureTransforms();

	//EntityManager::instance()->CleanUp();
	return true;
}


Scene::~Scene()
{

}