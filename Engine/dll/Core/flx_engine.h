/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#ifndef ENGINE_H
#define ENGINE_H

#include "flx_core.h"
#include "flx_cpuinfo.h"
#include "flx_singleton.h"
#include "flx_renderer.h"
#include "flx_logger.h"
#include "flx_graph.h"
#include "ResourceManager/flx_resource_manager.h"



namespace Flux
{

	class ENGINE_API Engine : public Singleton<Engine>
	{
	public:
		Engine();
		virtual ~Engine();

		void	Initialize();
		bool	IsInitialized();
		void	Tick();

		int	GetWindowWidth() { return m_iWidth; }
		int	GetWindowHeight() { return m_iHeight; }

		const Vector3 &getMousePosition();
		const Vector3 &getMouseDistFromCenter();
		int getWindowCenterX();
		int getWindowCenterY();

		void setWindowCenter(int _x, int _y);
		float getFPS();

		void	SetWindowWidth(int w) { m_iWidth = w; }
		void	SetWindowHeight(int h) { m_iHeight = h; }

		void write_uv_byte_2_texture_mem(unsigned char *buffer, unsigned char byte, 
			unsigned int channel, unsigned int u, unsigned int v, unsigned int texture_size, 
			unsigned int texture_channels);

		int mouse3dx_2_uv(int texture_size, int object_size);
		int mouse3dy_2_uv(int texture_size, int object_size);

	public:
		void ShowMouseCursor(bool state);
		void HandleMouse();
		void HandleKeyboard(float fDeltaTime);
		void HandleMouseWorldPos();

		void set_mwheel_rot(int rot);
		int	get_mwheel_rot();

		float GetGameTime();

		bool	m_bKeys[256];
		bool	m_bLButtonDown;
		bool	m_bRButtonDown;
		int	m_iMW_Rotation;

		int m_iWidth;
		int m_iHeight;
		double		m_dTimeDelta;
		Vector3		m_mouseCoord3D;
		Ray			m_mouseRay;

		int64 m_iTickFrequency;
		int64 m_iStartTicks;
		int64 m_iCurrentTicks;
		double m_flTime;
		double m_flDeltaTime;

		int m_editor_mode;
		bool m_bterrainPaint;
		unsigned char m_iterrainPaintOpacity;
		unsigned int m_iterrainPaintLayer;

		Vector3 m_mousePosition;
	private:
		float *m_mouseHistory;
		int m_windowCenterX;
		int m_windowCenterY;

	public:
		float getTerrainBrushSize();
		void  setTerrainBrushSize(float brush_size);
	
		std::vector<float> mTPSHistory;
	private:
		Renderer	*m_Renderer;
		double		m_dTimeGame;
		double		m_dTimeScaleFactor;
		LONGLONG	m_lPrevTime;
		LONGLONG	m_lCurrentTime;
		float		m_fps;


	private:
		float		m_fTerrainBrushSize;
		bool		m_bShowMouseCursor;
		bool		m_bInitialized;
		bool		m_bFirstFrameDone;
	};
}

#define flx_core Engine::getInstance()

#endif
