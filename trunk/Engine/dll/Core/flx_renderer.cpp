/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#include "flx_renderer.h"
#include "flx_application.h"
#include "flx_logger.h"

using namespace Flux;

Renderer::Renderer()
{
	m_rendererType		= R_OPENGL;
	m_CurrentScene		= NULL;
	m_bZBuffer			= true;
	m_bInitialized		= false;
	m_bRequestedPause	= false;
	mNumVisibleVertices = 0;
	graphVertices		= new Graph(100, 0, 100, 2, 0.00009f);
	graphTTR			= new Graph(100, 0, 100, 2, 300);
	graphTTR->setColor(1.0f, 0, 0);
}

Renderer::~Renderer()
{
	delete graphVertices;
}

void Renderer::Initialize()
{
	if(m_bInitialized || !Flux::Engine::getInstance()->IsInitialized()) 
		return;

	FLX_LOGGER->Write(LOG_DEFAULT, "Initializing Renderer...");

	glShadeModel(GL_SMOOTH);
	//glClearColor(0.47f, 0.79f, 1.0f, 1.0f);
	glClearColor(0.64f, 0.74f, 0.9f, 1.0f);
	//glClearColor(0.5f,0.5f,0.5f,1.0f);
	glClearDepth(1.0f);
	glDepthMask(GL_TRUE);

	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	//glEnable(GL_LINE_SMOOTH);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint (GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	glDepthFunc(GL_LEQUAL);
	glAlphaFunc(GL_GREATER,0.9f); // 0.1f creates problems at alpha mask borders
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); 

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL); 
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY); 
	

	
	glEnable(GL_MULTISAMPLE_ARB);
    glEnable( GL_SAMPLE_ALPHA_TO_COVERAGE_ARB );
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	GLuint	fogMode[]= { GL_EXP, GL_EXP2, GL_LINEAR };	// Storage For Three Types Of Fog
	GLuint	fogfilter = 2;								// Which Fog Mode To Use 
	GLfloat	fogColor[4] = {0.4f,0.4f,0.5f,1.0f};		// Fog Color

	//glFogi(GL_FOG_MODE, fogMode[fogfilter]);			// Fog Mode
	glFogfv(GL_FOG_COLOR, fogColor);					// Set Fog Color
	glFogf(GL_FOG_DENSITY, 0.00612f);						// How Dense Will The Fog Be
	glHint(GL_FOG_HINT, GL_DONT_CARE);					// Fog Hint Value
	glFogf(GL_FOG_START, 250.0f);							// Fog Start Depth
	glFogf(GL_FOG_END, 500.0f);							// Fog End Depth
	glEnable(GL_FOG);									// Enables GL_FOG

	m_bInitialized = true;
	FLX_LOGGER->Write(LOG_DEFAULT, "[DONE]\n");
}

bool Renderer::IsInitialized()
{ return m_bInitialized; }

void Renderer::SetProjectionMatrix()
{
	glViewport(0, 0, Engine::getInstance()->GetWindowWidth(), Engine::getInstance()->GetWindowHeight());

	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluPerspective( 45, (float)Engine::getInstance()->GetWindowWidth() / (float)Engine::getInstance()->GetWindowHeight(), 0.1f, 1000.0f);
}

inline void Renderer::SetOrthoMatrix()
{
	glViewport(0, 0, Engine::getInstance()->GetWindowWidth(), Engine::getInstance()->GetWindowHeight());
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, Engine::getInstance()->GetWindowWidth(), 0, Engine::getInstance()->GetWindowHeight());
}


inline void Renderer::RenderPre()
{
	if(m_bZBuffer)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	else
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(&Flux::Player::getInstance()->getProjectionMatrix()._m[0][0]);

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(&Flux::Player::getInstance()->getViewMatrix()._m[0][0]);

	m_numRenderCalls = 0;


	return;
}

void Renderer::SetScene(Scene* current_scene)
{
	/*if(!Flux::Engine::getInstance()->IsInitialized()) 
		Flux::Engine::getInstance()->Initialize();
	if(!m_bInitialized) 
		Initialize();
	*/
	

	/*if(current_scene != NULL)
	{
		m_CurrentScene = current_scene;
		if(!current_scene->m_bSceneInitialized)
			current_scene->m_bSceneInitialized = current_scene->Initialize();
	}*/
	m_CurrentScene = current_scene;
}



inline bool Renderer::RenderPost()
{
	graphVertices->update(mNumVisibleVertices);
	graphVertices->render();
	graphTTR->update(Flux::Engine::getInstance()->m_flDeltaTime);
	graphTTR->render();

	//glFlush();
	return ( SwapBuffers( GetDC(Application::getInstance()->GetWnd()) ) ? true : false);
}

void Renderer::Render()
{

}

E_RENDERER Renderer::GetRendererType()
{
	return m_rendererType;
}

bool Renderer::IsExtensionSupported( char* szTargetExtension )
{
	const unsigned char *pszExtensions = NULL;
	const unsigned char *pszStart;
	unsigned char *pszWhere, *pszTerminator;

	// Extension names should not have spaces
	pszWhere = (unsigned char *) strchr( szTargetExtension, ' ' );
	if( pszWhere || *szTargetExtension == '\0' )
		return false;

	// Get Extensions String
	pszExtensions = glGetString( GL_EXTENSIONS );

	// Search The Extensions String For An Exact Copy
	pszStart = pszExtensions;
	for(;;)
	{
		pszWhere = (unsigned char *) strstr( (const char *) pszStart, szTargetExtension );
		if( !pszWhere )
			break;
		pszTerminator = pszWhere + strlen( szTargetExtension );
		if( pszWhere == pszStart || *( pszWhere - 1 ) == ' ' )
			if( *pszTerminator == ' ' || *pszTerminator == '\0' )
				return true;
		pszStart = pszTerminator;
	}
	return false;
}

void Renderer::setClearColor(ColorOGL _color)
{
	m_clearColor = _color;
}

ColorOGL Renderer::getClearColor()
{ return m_clearColor; }

void Renderer::incNumRenderCalls()
{
	m_numRenderCalls++;
}

unsigned int Renderer::getNumRenderCalls()
{ return m_numRenderCalls; }