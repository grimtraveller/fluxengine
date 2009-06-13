/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#include "flx_core.h"
#include "flx_singleton.h"
#include "flx_engine.h"

namespace Flux
{

	struct WindowParameters 
	{
		char* pszTitle;
		int iWidth;
		int iHeight;
		int iRefreshRate;
		bool bWindowed;
		bool b32Bit;

		HWND		hWnd;
		HDC			hDC;
		HINSTANCE	hInst;

		//Default COnstuctor
		WindowParameters(char* _szTitle, HINSTANCE _hInst)
		{
			pszTitle	= _szTitle;
			iWidth		= 0;
			iHeight		= 0;
			iRefreshRate= 0;
			bWindowed	= true;
			b32Bit		= false;

			hWnd	= NULL;
			hDC		= NULL;
			hInst	= _hInst;
		}
	};

	class ENGINE_API Application : public Singleton<Application>
	{
	public:
		Application();
		virtual ~Application();

		void CreateDevice()
		{
			
		}

		int		Run();
		void	Initialize();
		void	Update();

		int		GetWindowWidth() { return m_iWidth; }
		int		GetWindowHeight() { return m_iHeight; }

		void	WindowFocusReceived();
		void	WindowFocusLost();
		void	DestroyWnd();

		HWND	GetWnd()			{ return m_hWnd; }
		void	SetDC(HDC hdc)		{ m_hDC = hdc; }
		HDC		getDC()				{ return m_hDC; }
		HGLRC	getGLRC()			{ return m_hGLRC; }
		void	SetWnd(HWND hwnd)	{ m_hWnd = hwnd; }

		bool Create(char* sAppName, HINSTANCE hInst);
		virtual bool MessageHandler(UINT iMessage, WPARAM wParam, LPARAM lParam);

	private:
		static LRESULT CALLBACK WindowProc(HWND hWindow, UINT iMessage, WPARAM wParam, LPARAM lParam);


		char*		m_pszTitle;
		char*		m_pszClassName;

		bool		m_bQuit;
		bool		m_bActive;

		int			m_iWidth;
		int			m_iHeight;
		bool		m_bIsFullScreen;
		int			m_iRefreshRate;
		bool		m_b32Bit;
		bool		m_bZBuffer;
		float		m_fps;

		WindowParameters*	m_pWndParam;


		HWND		m_hWnd;
		WNDCLASSEX	m_WindowClassEx;
		HINSTANCE	m_hInst;
		MSG			m_Message;
		HDC			m_hDC;
		HGLRC		m_hGLRC;
		HANDLE		hMutex;

	};

}
//extern DLL_API int ndll;
//DLL_API int fndll(void);
