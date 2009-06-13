/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

//WARNING! This is 100% messed up experimental code!
//That's not what I'd call an archetype of a good game client

//NOTE that this example uses some WoW models, which I can't share
//on a public SVN. Ask me or use some other models and textures from the resource/models
//folder

#include <math.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <list>
#include <process.h>

#include "Core/flx_application.h"
#include "Core/ResourceManager/flx_resource_mesh.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment (lib, "Engine.lib")

Flux::Application		*gameapp	= Flux::Application::getInstance();
Flux::Renderer			*renderer	= Flux::Renderer::getInstance();
ResourceManager			*resman		= ResourceManager::getInstance();
Flux::Engine			*flx		= Flux::Engine::getInstance();
LightPoint*				pLight;

class					Scene_LandScape;

Scene_LandScape*		s_LandScape;

NodeTransform*			pNodeTransform5;
NodeTransform*			pNodeTransform6;
float d_raise =0;
Vector3 old_pos(0,0,0);
Vector3 old_vel(0,0,0);
float lrot = 0;

bool moveForwardPressed = false;
bool moveBackwardPressed = false;
bool jumpPressed = false;

Node* pTerrain;
GeometryTerrain* terrain;
NodeTransform* pNodeTransform4;

class Scene_LandScape : public Flux::Scene
{
public:
	Scene_LandScape() { };
	virtual ~Scene_LandScape() { };

	inline void Update()
	{
		Flux::Player::getInstance()->Update((float)Flux::Engine::getInstance()->m_flDeltaTime);

	

			float height = 3 + terrain->getHeightf(Flux::Player::getInstance()->getPosition().x, Flux::Player::getInstance()->getPosition().z);
			if(Flux::Player::getInstance()->getPosition().y <= height)
			{
				Flux::Player::getInstance()->setPosition(Vector3(Flux::Player::getInstance()->getPosition().x, height,Flux::Player::getInstance()->getPosition().z));
			}

		pNodeTransform4->m_v3Translate.x = Flux::Player::getInstance()->getPosition().x;
		pNodeTransform4->m_v3Translate.z = Flux::Player::getInstance()->getPosition().z;

		if(Flux::Engine::getInstance()->m_bKeys[VK_KEY1])
			Flux::Engine::getInstance()->m_iterrainPaintLayer = 1;
		if(Flux::Engine::getInstance()->m_bKeys[VK_KEY2])
			Flux::Engine::getInstance()->m_iterrainPaintLayer = 2;
		if(Flux::Engine::getInstance()->m_bKeys[VK_KEY3])
			Flux::Engine::getInstance()->m_iterrainPaintLayer = 3;
		if(Flux::Engine::getInstance()->m_bKeys[VK_KEY4])
			Flux::Engine::getInstance()->m_iterrainPaintLayer = 4;
		if(Flux::Engine::getInstance()->m_bKeys[VK_KEY5])
			Flux::Engine::getInstance()->m_iterrainPaintLayer = 5;

		if(Flux::Engine::getInstance()->m_bKeys[VK_KEYP])
		{
			if(Flux::Engine::getInstance()->m_bLButtonDown)
			{
				Flux::Engine::getInstance()->m_bterrainPaint = true;
			}
			else
			{
				Flux::Engine::getInstance()->m_bterrainPaint = false;
			}
		}


		if(Flux::Engine::getInstance()->m_bKeys[VK_KEYR])
		{
			d_raise += (float)Flux::Engine::getInstance()->get_mwheel_rot()/4;
			terrain->Edit(Flux::Engine::getInstance()->m_mouseCoord3D.x, Flux::Engine::getInstance()->m_mouseCoord3D.z, Flux::Engine::getInstance()->getTerrainBrushSize(), d_raise);
			terrain->buildPatches(1);
			terrain->computeNormals();
		}
		if(Flux::Engine::getInstance()->m_bKeys[VK_KEYT])
		{
		
			terrain->smoothTerrain(0.5, (int)Flux::Engine::getInstance()->m_mouseCoord3D.x, (int)Flux::Engine::getInstance()->m_mouseCoord3D.z, (int)Flux::Engine::getInstance()->getTerrainBrushSize());
			terrain->buildPatches(1);
		}

		if(Flux::Engine::getInstance()->m_bKeys[VK_KEYL])
		{
			
			terrain->computeLightmap(Flux::Renderer::getInstance()->GetCurrentScene()->m_vLights[0]->GetPosition(), true);
		}
		if(Flux::Engine::getInstance()->m_bKeys[VK_KEYF])
			Flux::Engine::getInstance()->setTerrainBrushSize(Flux::Engine::getInstance()->getTerrainBrushSize() + (float)Flux::Engine::getInstance()->get_mwheel_rot()/2);


		if(Flux::Engine::getInstance()->m_bKeys[VK_KEY9]) {
			terrain->saveHeightmapToFile("terrain2.txt");
		}

		if(Flux::Engine::getInstance()->m_bKeys[VK_KEY0]) {
			terrain->computeNormals();
			//terrain->computeLightmap(Flux::Renderer::getInstance()->GetCurrentScene()->m_vLights[0]->GetPosition(), true);
		}

		char caption[400];
		sprintf(caption, "Visible Meshes: %u Visible Vertices %u", 
			Flux::Renderer::getInstance()->GetCurrentScene()->m_iVisible, 
			Flux::Renderer::getInstance()->mNumVisibleVertices
			);
		SetWindowText(Flux::Application::getInstance()->GetWnd(), caption);
	}

	void Render()
	{
		
	}

	bool Initialize()
	{
		resman->setTexturePath("resources/textures/");
		resman->loadResource(resman->FLX_TEXTURE2D, "resources/textures/cursor.png");
		resman->loadResource(resman->FLX_TEXTURE2D, "resources/textures/alpha.tga");
		resman->loadResource(resman->FLX_TEXTURE2D, "resources/textures/alpha1.tga");
		resman->loadResource(resman->FLX_TEXTURE2D, "resources/textures/grass.png");
		resman->loadResource(resman->FLX_TEXTURE2D, "resources/textures/rock.png");
		resman->loadResource(resman->FLX_TEXTURE2D, "resources/textures/ground.png");
		resman->loadResource(resman->FLX_TEXTURE2D, "resources/textures/tile_rock0.png");
		resman->loadResource(resman->FLX_TEXTURE2D, "resources/textures/tile_snow.png");
		resman->loadResource(resman->FLX_TEXTURE2D, "resources/textures/loading.png");
		resman->loadResource(resman->FLX_TEXTURE2D, "resources/textures/MullgoreBaseGrass.tga");
		resman->loadResource(resman->FLX_TEXTURE2D, "resources/textures/lightmap.tga");
		resman->loadResource(resman->FLX_TEXTURE2D, "resources/textures/cloud.png");
		resman->loadResource(resman->FLX_TEXTURE2D, "resources/textures/sun.png");
		resman->loadResource(resman->FLX_TEXTURE2D, "resources/textures/terrain_water_caustics.jpg");
		resman->loadResource(resman->FLX_TEXTURE2D, "resources/textures/terrain_water_NM.jpg");

		resman->loadResource(resman->FLX_SHADER,	"resources/shader/ppl");
		resman->loadResource(resman->FLX_SHADER,	"resources/shader/terrain");
		resman->loadResource(resman->FLX_SHADER,	"resources/shader/sky");
		resman->loadResource(resman->FLX_SHADER,	"resources/shader/water");
		

		Flux::Player::getInstance()->perspective(90.0f, 4.0f / 3.0f, 0.1f, 520.0f);
		Flux::Player::getInstance()->setBehavior(Camera::CAMERA_BEHAVIOR_SPECTATOR);
		Flux::Player::getInstance()->setPosition(Vector3(128,3,128));
		Flux::Player::getInstance()->setAcceleration(Vector3(50.0, 28.0f, 50.0f));
		Flux::Player::getInstance()->setVelocity(12.0f, 8.0f, 12.0f);
		Flux::Player::getInstance()->setRotationSpeed(0.1f);

		pLight = new LightPoint;
		pLight->SetDiffuseColor(ColorOGL(255,245,245));
		//pLight->SetDiffuseColor(ColorOGL(155,145,145));
		pLight->SetAmbientColor(ColorOGL(200,200,200));
		pLight->SetSpecularColor(ColorOGL(255,245,200));
		pLight->SetPosition(Vector3(100,500,300));
		AddLight(pLight);

		NodeTransform* pNodeTransform = new NodeTransform();
		pNodeTransform->m_v3Translate = Vector3(0, 0, 0);
		pNodeTransform->SetName("ROOT_TRANSFORM");
		unsigned int iNodeRoot = AddNode(pNodeTransform, 0);

		int terrain_size = 1024;
		
		//bug: skybox disappears e.g. when terrain_size is 256 and patch size > 16 (=32)
		//GeometryTerrain* pTerrain = new GeometryTerrain(terrain_size, 16, 0, 0.1, 0);
		GeometryTerrain* pTerrain = new GeometryTerrain(terrain_size, 32, 0, 0.9, 0);
		pTerrain->loadHeightmapFromFile("terrain.txt");
		//pTerrain->computeLightmap(Flux::Renderer::getInstance()->GetCurrentScene()->m_vLights[0]->GetPosition());
		NodeShape* pShape = new NodeShape(pTerrain);
		pShape->SetName("terrain");
		pShape->ignoresFrustumCulling(true);
		SaveGeometry(pTerrain);
		int iShape = AddNode(pShape, iNodeRoot);
		AssignShader(iShape, ((ResourceShader*)resman->getResource("resources/shader/terrain")));
		//((ResourceShader*)resman->getResource("resources/shader/terrain"))->Activate();
		//((ResourceShader*)resman->getResource("resources/shader/sky"))->Activate()


		terrain = pTerrain;
		int nodemesh=0;

		if(((ResourceShader*)resman->getResource("resources/shader/ppl")) != NULL)
			((ResourceShader*)resman->getResource("resources/shader/ppl"))->Activate();
		GeometryMesh* pMesh2 = new GeometryMesh("resources/models/manerothtree.3ds");
		for(int i = 0; i < 10; i++)
		{
		
			float x = (rand()%terrain_size);
			float z = (rand()%terrain_size);

			if(pTerrain->getHeightf(x,z) > 20) 
			{
				i--;
				continue;
			}
			pNodeTransform6 = new NodeTransform();
			pNodeTransform6->m_v3Translate = Vector3(x, pTerrain->getHeightf(x,z)-1.5f, z);
			int iNodeID = AddNode(pNodeTransform6, iNodeRoot);
			int iNodeMesh = SaveGeometry(pMesh2);

			NodeShape* pShapeMesh = new NodeShape(pMesh2);
			AssignShader(AddNode(pShapeMesh, iNodeID), ((ResourceShader*)resman->getResource("resources/shader/ppl")));
		}


		

		/*GeometryMesh* pMesh4 = new GeometryMesh("resources/models/walle.3ds");
		for(int i = 0; i < 1; i++)
		{
			float x = Flux::Player::getInstance()->getPosition().x+20;
			float z =  Flux::Player::getInstance()->getPosition().z+20;

			NodeTransform* pNodeTransform5 = new NodeTransform();
			pNodeTransform5->SetName("walle");
			pNodeTransform5->m_v3Translate = Vector3(x, pTerrain->getHeightf(x,z)-0.1f, z);
			int iNodeID = AddNode(pNodeTransform5, iNodeRoot);
			int iNodeMesh = SaveGeometry(pMesh4);

			NodeShape* pShapeMesh = new NodeShape(pMesh4);
			pShapeMesh->SetName("wallee");
			
			AssignShader(AddNode(pShapeMesh, iNodeID), ((ResourceShader*)resman->getResource("resources/shader/ppl")));

			//For some reason (which I don't know yet), we have to activate it here first
			//((ResourceShader*)resman->getResource("resources/shader/ppl"))->Activate();
		}
		*/

		
		GeometryMesh* pSkyDome = new GeometryMesh("resources/models/skydome.3ds");
		pNodeTransform4 = new NodeTransform();
		pNodeTransform4->m_v3Scale = Vector3(0.8, 0.8, 0.8);
		pNodeTransform4->m_v3Translate = Vector3(terrain_size/2, -200, terrain_size/2);

		//pNodeTransform4->m_v3RotateAngle = Vector3(-90,0,0);
		int iNodeID3 = AddNode(pNodeTransform4, iNodeRoot);
		SaveGeometry(pSkyDome);

		NodeShape* pSkyDomeShape = new NodeShape(pSkyDome);
		pSkyDomeShape->SetName("Skydome");
		pSkyDomeShape->ignoresFrustumCulling(true);

		int nodemesh2 = AddNode(pSkyDomeShape, iNodeID3);
		AssignShader(nodemesh2, ((ResourceShader*)resman->getResource("resources/shader/sky")));

		GeometryMesh* pMesh7 = new GeometryMesh("resources/models/mult.3ds");
		for(int i = 0; i < 1800; i++)
		{
			float x = (rand()%terrain_size);
			float z = (rand()%terrain_size);

			if(pTerrain->getHeightf(x,z) > 20) 
			{
				i--;
				continue;
			}
			NodeTransform* pNodeTransform5 = new NodeTransform();
			float rot = rand()%360;
			pNodeTransform5->m_v3Translate = Vector3(x, pTerrain->getHeightf(x,z)-0.0f, z);
			//pNodeTransform5->m_v3Scale = Vector3(0.5, 0.5, 0.5);
			int iNodeID = AddNode(pNodeTransform5, iNodeRoot);
			int iNodeMesh = SaveGeometry(pMesh7);

			NodeShape* pShapeMesh = new NodeShape(pMesh7);
			AssignShader(AddNode(pShapeMesh, iNodeID), ((ResourceShader*)resman->getResource("resources/shader/ppl")));

			//For some reason (which I don't know yet), we have to activate it here first
			//((ResourceShader*)resman->getResource("resources/shader/ppl"))->Activate();
		}



		/*GeometryFlatGrid* pWater = new GeometryFlatGrid(1024,1024,100,100);
		NodeTransform* pNodeTransform6 = new NodeTransform();
		pNodeTransform6->m_v3Translate = Vector3(0, 8.0f, 0);
		int iNodeID = AddNode(pNodeTransform6, iNodeRoot);
		SaveGeometry(pWater);

		NodeShape* pShapeMesh = new NodeShape(pWater);
		pShapeMesh->SetName("water");
		int iWater = AddNode(pShapeMesh, iNodeID);
		AssignShader(iWater, ((ResourceShader*)resman->getResource("resources/shader/water")));
		*/
		m_bSceneInitialized = true;
		return true;
	}



};




int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd) 
{
	/*Flux::Application::getInstance()->Create("Test", hInst);
	ResourceManager* resman = ResourceManager::getInstance();

	//resman->loadResource(resman->FLX_TEXTURE2D, "resources/textures/cursor.png");

	//Flux::Engine::getInstance()->ShowMouseCursor(false);
	s_LandScape = new Scene_LandScape;
	Flux::Renderer::getInstance()->SetScene(s_LandScape);
	Flux::Application::getInstance()->Run();
	*/

	gameapp->Create("Test", hInst);


	s_LandScape = new Scene_LandScape;
	renderer->SetScene(s_LandScape);
	flx->ShowMouseCursor(false);
	gameapp->Run();
	return 0;
}