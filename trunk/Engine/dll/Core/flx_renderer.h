/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#ifndef RENDERER_H
#define RENDERER_H


#include "flx_core.h"
#include "flx_singleton.h"
#include "flx_scene.h"
#include "flx_graph.h"


enum E_RENDERER
{
	R_OPENGL,
	R_DIRECTX,
	R_SOFTWARE
};

namespace Flux
{
	
	class ENGINE_API Renderer : public Singleton<Renderer>
	{
	public:

		Renderer();
		virtual ~Renderer();

		void Initialize();
		bool IsInitialized();

		bool IsExtensionSupported( char* szTargetExtension );

		void RenderPre();
		bool RenderPost();
		void Render();

		void SetScene(Scene* current_scene);
		void SetShaders();
		void incNumRenderCalls();
		unsigned int getNumRenderCalls();

		E_RENDERER GetRendererType();
		void SetProjectionMatrix();
		void SetOrthoMatrix();
		void setClearColor(ColorOGL _color);
		ColorOGL getClearColor();

		Scene*	GetCurrentScene() { return m_CurrentScene; }

		bool			m_bZBuffer;
		unsigned int	mNumVisibleVertices;
		Graph			*graphVertices;
		Graph			*graphTTR;

		bool			m_bCurrentlyRendering;
		bool			m_bRequestedPause;
	private:
		Scene			*m_CurrentScene;
		bool			m_bInitialized;
		ColorOGL		m_clearColor;
		unsigned int	m_numRenderCalls;
		

		E_RENDERER m_rendererType;

		
	};
}

#endif