/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

//Everythings chaotic! D:

#include "flx_engine.h"
#include "flx_player.h"
#include "flx_application.h"
#include <process.h>

using namespace Flux;

int m_historyBufferSize = 10;
bool thread_started = false;
bool	renderedToTexture = false;
Engine::Engine()
{
	m_bFirstFrameDone	= false;
	m_bShowMouseCursor	= true;
	m_bInitialized		= false;

	m_dTimeGame	= 0.0f;

	QueryPerformanceFrequency	( (LARGE_INTEGER*)&m_iTickFrequency	);
	QueryPerformanceCounter		(( LARGE_INTEGER*)&m_iStartTicks	);
	m_flTime			= m_flDeltaTime = 0;

	for(int i = 0; i < 256; i++)
	{
		m_bKeys[i] = false;
	}

	m_bLButtonDown = false;
	m_bRButtonDown = false;

	m_iWidth = Application::getInstance()->GetWindowWidth();
	m_iHeight = Application::getInstance()->GetWindowHeight();

	m_editor_mode = 0;
	m_bterrainPaint = false;
	m_iterrainPaintOpacity = 0;
	m_iterrainPaintLayer = 0;
	m_fTerrainBrushSize = 4.0f;

	m_iMW_Rotation = 0;

	m_windowCenterX = GetWindowWidth() >> 1;
	m_windowCenterY = GetWindowHeight() >> 1;
}

Engine::~Engine()
{
}

void Engine::Initialize()
{
	if(m_bInitialized) return;

	m_bInitialized = true;

	FLX_LOGGER->Write(LOG_DEFAULT, "System: %s\n",	GetCPUInfo().S_VENDOR_ID);
	FLX_LOGGER->Write(LOG_DEFAULT, "\t%s\n",			GetCPUInfo().S_MODEL_NAME);
	FLX_LOGGER->Write(LOG_DEFAULT, "\tCPU Features: %s %s %s %s %s %s %s %s\n", 
		GetCPUInfo().B_SUPPORTS_3DNOW		?"3DNow! "	: "",
		GetCPUInfo().B_SUPPORTS_3DNOW_EX	?"(+) "		: "",
		GetCPUInfo().B_SUPPORTS_MMX			?"MMX "		: "",
		GetCPUInfo().B_SUPPORTS_MMX_EX		?"(+) "		: "",
		GetCPUInfo().B_SUPPORTS_SSE			?"SSE "		: "",
		GetCPUInfo().B_SUPPORTS_SSE2		?"SSE2 "	: "",
		GetCPUInfo().B_SUPPORTS_SSE3		?"SSE3 "	: "",
		GetCPUInfo().B_SUPPORTS_FPU			?"FPU "		: ""
		);


	m_Renderer->getInstance()->Initialize();
	

	FLX_LOGGER->Write(LOG_DEFAULT, "Initializing Core Engine [DONE]\n");
}

bool Engine::IsInitialized()
{
	return m_bInitialized;
}

float Engine::GetGameTime()
{
	return m_dTimeGame;
}

inline void Engine::Tick()
{
	HandleMouse();
	HandleKeyboard(m_flDeltaTime);


	QueryPerformanceCounter((LARGE_INTEGER*)&m_iCurrentTicks);
	int64 total_ticks	= m_iCurrentTicks - m_iStartTicks;
	double new_time		= (double)total_ticks / (double)m_iTickFrequency;

	m_flDeltaTime		= (new_time - m_flTime)*2;
	m_fps				= m_iCurrentTicks / m_flDeltaTime;
	m_flTime			= new_time;


	m_Renderer->getInstance()->GetCurrentScene()->PlayerUpdate((float)m_flDeltaTime);


	Flux::Renderer::getInstance()->m_bCurrentlyRendering = true;
	m_Renderer->getInstance()->RenderPre();

	//render stuff
	if(!m_Renderer->getInstance()->IsInitialized())
		m_Renderer->getInstance()->Initialize();

	if(m_Renderer->getInstance()->GetCurrentScene() != NULL)
	{
		if(m_Renderer->getInstance()->GetCurrentScene()->m_bSceneInitialized)
		{
			m_Renderer->getInstance()->GetCurrentScene()->Update((float)m_flDeltaTime);


	/*if(m_bFirstFrameDone && !renderedToTexture)
	{
		ResourceManager::getInstance()->getTexture2D("resources/textures/water_reflection.tga")->bind(0);
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0,0,0,0,0, 512,512);

		ResourceManager::getInstance()->getTexture2D("resources/textures/water_refraction.tga")->bind(1);
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0,0,0,0,0, 512,512);

		ResourceManager::getInstance()->getTexture2D("resources/textures/water_depthmap.tga")->bind(4);
		glCopyTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT,0,0,512,512, 0);
		renderedToTexture = true;
	}*/

			HandleMouseWorldPos();
		}
	}

	

	m_Renderer->SetOrthoMatrix();
	m_Renderer->getInstance()->GetCurrentScene()->HUD();

	m_Renderer->getInstance()->RenderPost();

	Flux::Renderer::getInstance()->m_bCurrentlyRendering = false;

	
	if(m_Renderer->getInstance()->GetCurrentScene() != NULL)
	{
		if(!m_Renderer->getInstance()->GetCurrentScene()->m_bSceneInitialized)
		{
			if(!thread_started)
			{
				thread_started = true;
				//_beginthread(Scene::RunThread,0,m_Renderer->getInstance()->GetCurrentScene());
				m_Renderer->getInstance()->GetCurrentScene()->Initialize();
			}
		}
	}




	m_bFirstFrameDone = true;
	//Sleep(1);
}

void Engine::ShowMouseCursor(bool state)
{
	ShowCursor(state);
}

void Engine::HandleKeyboard(float fDeltaTime)
{

	if(m_bKeys[VK_KEYZ])
	{
		static double dTimeDelay = 0;
		if(m_dTimeGame > (dTimeDelay + 0.3))
		{
		Flux::Renderer::getInstance()->GetCurrentScene()->ToggleHeadLight();
			dTimeDelay = m_dTimeGame;
		}
	}

	if(m_bKeys[VK_KEYM])
	{
			static double dTimeDelay = 0;
		if(m_flTime > (dTimeDelay + 0.3))
		{
			m_editor_mode++;
			if(m_editor_mode > 2)
				m_editor_mode = 0;

			dTimeDelay = m_flTime;
		}
	}

	if(m_bKeys[VK_KEYP] && !m_bterrainPaint)
	{
		m_bterrainPaint = true;
	}
	if(!m_bKeys[VK_KEYP])
	{
		m_bterrainPaint = false;
	}

	if(m_bKeys[VK_KEYB])
	{
		static double dTimeDelay = 0;
		if(m_flTime > (dTimeDelay + 0.3))
		{
			 Flux::Renderer::getInstance()->GetCurrentScene()->m_Frustum.EnableFrustum(!Flux::Renderer::getInstance()->GetCurrentScene()->m_Frustum.IsActive());
			dTimeDelay = m_flTime;
		}
	}

	if(m_bKeys[VK_KEY0])
		this->m_iterrainPaintLayer = 0;

	if(m_bKeys[VK_KEY1])
		this->m_iterrainPaintLayer = 1;

	if(m_bKeys[VK_KEY2])
		this->m_iterrainPaintLayer = 2;

	if(m_bKeys[VK_KEY3])
		this->m_iterrainPaintLayer = 3;

	if(m_bKeys[VK_KEY4])
		this->m_iterrainPaintLayer = 4;

	if(m_bKeys[VK_KEY5])
		this->m_iterrainPaintLayer = 5;

	if(m_bKeys[VK_KEY6])
		this->m_iterrainPaintLayer = 6;

	if(m_bKeys[VK_KEY7])
		this->m_iterrainPaintLayer = 7;

	if(m_bKeys[VK_KEY8])
		this->m_iterrainPaintLayer = 8;
	if(m_bKeys[VK_KEY9])
		this->m_iterrainPaintLayer = 9;
}

inline void Engine::HandleMouse()
{
	
	static int iMouseX = GetWindowWidth() >> 1;
	static int iMouseY = GetWindowHeight() >> 1;

	static float fScaleFactor = 0.001f;

	POINT mousePosition;
	GetCursorPos(&mousePosition);
	//ScreenToClient(Flux::Application::getInstance()->GetWnd(), &mousePosition);
	m_mousePosition.x = mousePosition.x;
	m_mousePosition.y = mousePosition.y;
}

inline void Engine::HandleMouseWorldPos()
{
	double dx, dy, dz;
	double dx2, dy2, dz2;
	GLint viewport[4]; 
	GLdouble modelview[16], projection[16];
	glGetIntegerv( GL_VIEWPORT, viewport );
	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );

	GLint screenX = m_mousePosition.x;
	GLint screenY = viewport[3] - m_mousePosition.y;	
	
	
	float depth = 0;
	glReadPixels(screenX,  screenY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
	gluUnProject(screenX , screenY, depth, modelview, projection, viewport, &dx, &dy, &dz);

	m_mouseCoord3D =  Vector3((float)dx, (float)dy, (float)dz);

	if(m_bLButtonDown)
	{
		//Mouse picking ray
		Vector3 r1(Flux::Player::getInstance()->getPosition());
		//gluUnProject(screenX , screenY, 0.0f, modelview, projection, viewport, &dx, &dy, &dz);
		gluUnProject(screenX , screenY, depth, modelview, projection, viewport, &dx2, &dy2, &dz2);
		Vector3 r2(dx2,dy2,dz2);
		m_mouseRay = Ray(r1, r2);
	}
}

float Engine::getTerrainBrushSize()
{ return m_fTerrainBrushSize; }

const Vector3 &Engine::getMousePosition()
{ return m_mousePosition; }

inline int Engine::getWindowCenterX()
{ return m_windowCenterX; }

inline int Engine::getWindowCenterY()
{ return m_windowCenterY; }

void Engine::setWindowCenter(int _x, int _y)
{
	m_windowCenterX = _x;
	m_windowCenterY = _y;
}

inline const Vector3& Engine::getMouseDistFromCenter()
{
	return Vector3(
		getMousePosition().x - getWindowCenterX(), 
		getMousePosition().y - getWindowCenterY(), 
		0);

}

void Engine::setTerrainBrushSize(float brush_size)
{ m_fTerrainBrushSize = brush_size; }

void Engine::set_mwheel_rot(int rot)
{ m_iMW_Rotation = rot; }

int Engine::get_mwheel_rot()
{
	int t = m_iMW_Rotation;
	m_iMW_Rotation = 0;
	return t;
}


inline void Engine::write_uv_byte_2_texture_mem(unsigned char *buffer, unsigned char byte, 
			unsigned int channel, unsigned int u, unsigned int v, unsigned int texture_size, 
			unsigned int texture_channels)
{
	if(((u+v*texture_size)*texture_channels) <= texture_size*texture_size*texture_channels)
	{

		/*
		if(channel == 1)
		{
			if(buffer[(u+v*texture_size)*texture_channels + 0] + byte/4 <= 255 &&
			buffer[(u+v*texture_size)*texture_channels + 0] + byte/4 >=0 )
			{
				buffer[(u+v*texture_size)*texture_channels + 0] += byte/4;
			}

			if(buffer[(u+v*texture_size)*texture_channels + 1] - byte/4 >=0)
				buffer[(u+v*texture_size)*texture_channels + 1] -= byte/4;

			if(buffer[(u+v*texture_size)*texture_channels + 2] - byte/4 >=0)
				buffer[(u+v*texture_size)*texture_channels + 2] -= byte/4;

			if(buffer[(u+v*texture_size)*texture_channels + 3] - byte/4 >=0)
				buffer[(u+v*texture_size)*texture_channels + 3] -= byte/4;
		}

		if(channel == 2)
		{
			if(buffer[(u+v*texture_size)*texture_channels + 1] + byte/4 <= 255 &&
				buffer[(u+v*texture_size)*texture_channels + 1] + byte/4 >=0)
			{
				

				buffer[(u+v*texture_size)*texture_channels + 1] += byte/4;

				//if(buffer[(u+v*texture_size)*texture_channels + 2] - byte/4 >=0)
				//	buffer[(u+v*texture_size)*texture_channels + 2] -= byte/4;
			}
		}

		if(channel == 3)
		{
			if(buffer[(u+v*texture_size)*texture_channels + 2] + byte/4 <= 255 &&
				buffer[(u+v*texture_size)*texture_channels + 2] + byte/4 >=0)
			{
				//if(buffer[(u+v*texture_size)*texture_channels + 0] - byte/4 >=0)
				//	buffer[(u+v*texture_size)*texture_channels + 0] -= byte/4;

				//if(buffer[(u+v*texture_size)*texture_channels + 1] - byte/4 >=0)
				//	buffer[(u+v*texture_size)*texture_channels + 1] -= byte/4;

				buffer[(u+v*texture_size)*texture_channels + 2] += byte/4;


			}
		}
		*/




		if(channel == 1)
		{
			if(buffer[(u+v*texture_size)*texture_channels + 0] + byte/4 <= 255 &&
			buffer[(u+v*texture_size)*texture_channels + 0] + byte/4 >=0 )
			{
				buffer[(u+v*texture_size)*texture_channels + 0] += byte/4;
			}
		}

		if(channel == 2)
		{
			if(buffer[(u+v*texture_size)*texture_channels + 1] + byte/4 <= 255 &&
			buffer[(u+v*texture_size)*texture_channels + 1] + byte/4 >=0 )
			{
				buffer[(u+v*texture_size)*texture_channels + 1] += byte/4;
			}
		}

		if(channel == 3)
		{
			if(buffer[(u+v*texture_size)*texture_channels + 2] + byte/4 <= 255 &&
			buffer[(u+v*texture_size)*texture_channels + 2] + byte/4 >=0 )
			{
				buffer[(u+v*texture_size)*texture_channels + 2] += byte/4;
			}
		}

		if(channel == 4)
		{
			if(buffer[(u+v*texture_size)*texture_channels + 3] + byte/4 <= 255 &&
			buffer[(u+v*texture_size)*texture_channels + 3] + byte/4 >=0 )
			{
				buffer[(u+v*texture_size)*texture_channels + 3] += byte/4;
			}

		}

		if(channel == 5)
		{
			buffer[(u+v*texture_size)*texture_channels + 0] = 1;
			buffer[(u+v*texture_size)*texture_channels + 1] = 1;
			buffer[(u+v*texture_size)*texture_channels + 2] = 1;
			buffer[(u+v*texture_size)*texture_channels + 3] = 1;
		}
	}

}

inline int Engine::mouse3dx_2_uv(int texture_size, int object_size)
{
	return (texture_size/object_size) * (int)this->m_mouseCoord3D.x;
}

inline int Engine::mouse3dy_2_uv(int texture_size, int object_size)
{
	return (texture_size/object_size) * (int)(fabs(this->m_mouseCoord3D.z ));
}

float Engine::getFPS()
{ return m_fps; }