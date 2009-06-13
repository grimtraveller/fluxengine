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


NodeTransform*			pNodeTransform6;
NodeTransform*			pSkyDomeTransform;
float d_raise =0;
Vector3 old_pos(0,0,0);
Vector3 old_vel(0,0,0);
float lrot = 0;

bool moveForwardPressed = false;
bool moveBackwardPressed = false;
bool jumpPressed = false;

Node* pTerrain;
GeometryTerrain* terrain;


class Scene_LandScape : public Flux::Scene
{
public:
	Scene_LandScape() { };
	virtual ~Scene_LandScape() { };

	inline void Update()
	{
		Flux::Player::getInstance()->Update((float)Flux::Engine::getInstance()->m_flDeltaTime);

		//get the ground height at player position
		float terrainHeight = 3 + terrain->getHeightf(Flux::Player::getInstance()->getPosition().x, Flux::Player::getInstance()->getPosition().z);
		
		//if it's underneath it, set it to terrainHeight
		if(Flux::Player::getInstance()->getPosition().y <= terrainHeight)
			Flux::Player::getInstance()->setPosition(Vector3(Flux::Player::getInstance()->getPosition().x, terrainHeight, Flux::Player::getInstance()->getPosition().z));

		//let the skydome follow the player
		pSkyDomeTransform->m_v3Translate.x = Flux::Player::getInstance()->getPosition().x;
		pSkyDomeTransform->m_v3Translate.z = Flux::Player::getInstance()->getPosition().z;

		//Actually not necessary. Should be hardcoded in engine
		//it's setting the current alpha map texture channel for painting
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

		//pressed P for painting?
		//should also not be here
		if(Flux::Engine::getInstance()->m_bKeys[VK_KEYP])
		{
			if(Flux::Engine::getInstance()->m_bLButtonDown)
				Flux::Engine::getInstance()->m_bterrainPaint = true;
			else
				Flux::Engine::getInstance()->m_bterrainPaint = false;
		}

		//pressed R for terrain sculpting (raise/lower)
		//should also not be in game client code
		if(Flux::Engine::getInstance()->m_bKeys[VK_KEYR])
		{
			d_raise += (float)Flux::Engine::getInstance()->get_mwheel_rot()/4;
			terrain->Edit(Flux::Engine::getInstance()->m_mouseCoord3D.x, Flux::Engine::getInstance()->m_mouseCoord3D.z, Flux::Engine::getInstance()->getTerrainBrushSize(), d_raise);
			terrain->buildPatches(1);
			terrain->computeNormals();
		}

		//pressed T for terrain smoothing?
		//should also not be in game client code
		if(Flux::Engine::getInstance()->m_bKeys[VK_KEYT])
		{
			terrain->smoothTerrain(0.5, (int)Flux::Engine::getInstance()->m_mouseCoord3D.x, (int)Flux::Engine::getInstance()->m_mouseCoord3D.z, (int)Flux::Engine::getInstance()->getTerrainBrushSize());
			terrain->buildPatches(1);
		}

		//pressed L to recompute lightmap?
		//should not be here...
		//lightmap's not working correct atm
		if(Flux::Engine::getInstance()->m_bKeys[VK_KEYL])
			terrain->computeLightmap(Flux::Renderer::getInstance()->GetCurrentScene()->m_vLights[0]->GetPosition(), true);

		//pressed F for brush size change?
		if(Flux::Engine::getInstance()->m_bKeys[VK_KEYF])
			Flux::Engine::getInstance()->setTerrainBrushSize(Flux::Engine::getInstance()->getTerrainBrushSize() + (float)Flux::Engine::getInstance()->get_mwheel_rot()/2);

		//pressed 9 to save terrain data?
		if(Flux::Engine::getInstance()->m_bKeys[VK_KEY9]) {
			terrain->saveHeightmapToFile("terrain.txt");
		}

		//pressed 0 to recompute terrain normals?
		if(Flux::Engine::getInstance()->m_bKeys[VK_KEY0]) {
			terrain->computeNormals();
			//terrain->computeLightmap(Flux::Renderer::getInstance()->GetCurrentScene()->m_vLights[0]->GetPosition(), true);
		}

		//show some info in window caption
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
		//preload textures
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

		//load shaders
		resman->loadResource(resman->FLX_SHADER,	"resources/shader/ppl");
		resman->loadResource(resman->FLX_SHADER,	"resources/shader/terrain");
		resman->loadResource(resman->FLX_SHADER,	"resources/shader/sky");
		resman->loadResource(resman->FLX_SHADER,	"resources/shader/water");

		//set player properties
		Flux::Player::getInstance()->perspective(90.0f, 4.0f / 3.0f, 0.1f, 520.0f);
		Flux::Player::getInstance()->setBehavior(Camera::CAMERA_BEHAVIOR_SPECTATOR);
		Flux::Player::getInstance()->setPosition(Vector3(128,3,128));
		Flux::Player::getInstance()->setAcceleration(Vector3(50.0, 28.0f, 50.0f));
		Flux::Player::getInstance()->setVelocity(12.0f, 8.0f, 12.0f);
		Flux::Player::getInstance()->setRotationSpeed(0.1f);

		//create a light source
		pLight = new LightPoint;
		pLight->SetDiffuseColor(ColorOGL(255,245,245));
		pLight->SetAmbientColor(ColorOGL(200,200,200));
		pLight->SetSpecularColor(ColorOGL(255,245,200));
		pLight->SetPosition(Vector3(100,500,300));
		AddLight(pLight);

		//this is the ROOT node in the scene graph
		NodeTransform* pNodeTransform = new NodeTransform();
		pNodeTransform->m_v3Translate = Vector3(0, 0, 0);
		pNodeTransform->SetName("ROOT_TRANSFORM");
		unsigned int iNodeRoot = AddNode(pNodeTransform, 0);

		//add a terrain node to it
		int terrain_size = 1024;
		//bug: skybox disappears e.g. when terrain_size is 256 and patch size > 16 (=32)
		//GeometryTerrain* pTerrain = new GeometryTerrain(terrain_size, 16, 0, 0.1, 0);
		GeometryTerrain* pTerrain = new GeometryTerrain(terrain_size, 32, 0, 0.9, 0);
		pTerrain->loadHeightmapFromFile("terrain.txt");
		NodeShape* pShape = new NodeShape(pTerrain);
		pShape->SetName("terrain");
		pShape->ignoresFrustumCulling(true);
		SaveGeometry(pTerrain);
		int iShape = AddNode(pShape, iNodeRoot);
		AssignShader(iShape, ((ResourceShader*)resman->getResource("resources/shader/terrain")));



		terrain = pTerrain;
		int nodemesh=0;

		//add some weird trees
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


		//add a skydome
		GeometryMesh* pSkyDome = new GeometryMesh("resources/models/skydome.3ds");
		pSkyDomeTransform = new NodeTransform();
		pSkyDomeTransform->m_v3Scale = Vector3(0.8, 0.8, 0.8);
		pSkyDomeTransform->m_v3Translate = Vector3(terrain_size/2, -200, terrain_size/2);

		int iNodeID3 = AddNode(pSkyDomeTransform, iNodeRoot);
		SaveGeometry(pSkyDome);

		NodeShape* pSkyDomeShape = new NodeShape(pSkyDome);
		pSkyDomeShape->SetName("Skydome");
		pSkyDomeShape->ignoresFrustumCulling(true);

		int nodemesh2 = AddNode(pSkyDomeShape, iNodeID3);
		AssignShader(nodemesh2, ((ResourceShader*)resman->getResource("resources/shader/sky")));


		//add trees
		GeometryMesh* pMeshTree = new GeometryMesh("resources/models/mult.3ds");
		for(int i = 0; i < 1800; i++)
		{
			float x = (rand()%terrain_size);
			float z = (rand()%terrain_size);

			if(pTerrain->getHeightf(x,z) > 20) 
			{
				i--;
				continue;
			}

			NodeTransform* pTreeTransform = new NodeTransform();
			pTreeTransform->m_v3Translate = Vector3(x, pTerrain->getHeightf(x,z)-0.0f, z);
			int iNodeID = AddNode(pTreeTransform, iNodeRoot);
			int iNodeMesh = SaveGeometry(pMeshTree);

			NodeShape* pShapeMesh = new NodeShape(pMeshTree);
			AssignShader(AddNode(pShapeMesh, iNodeID), ((ResourceShader*)resman->getResource("resources/shader/ppl")));
		}

		m_bSceneInitialized = true;
		return true;
	}
};




int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd) 
{
	gameapp->Create("Test", hInst);

	s_LandScape = new Scene_LandScape;
	renderer->SetScene(s_LandScape);
	flx->ShowMouseCursor(false);
	gameapp->Run();
	return 0;
}