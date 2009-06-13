/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#include "flx_application.h"
#include "wglew.h"

using namespace Flux;

Application::Application()
{
	m_pszTitle		= NULL;
	m_pszClassName	= NULL;

	m_bQuit		= false;
	m_bActive	= true;

	m_iWidth		= 1280;
	m_iHeight		= 1024;
	//m_iWidth		= 800;
	//m_iHeight		= 600;
	m_bIsFullScreen	= false;
	m_iRefreshRate	= 60;
	m_fps			= 60;
	m_b32Bit		= true;
	m_bZBuffer		= true;


}

Application::~Application()
{
	CloseHandle(hMutex);
}

bool Application::Create(char* sAppName, HINSTANCE hInst)
{
	WindowParameters WndParam(sAppName, hInst);
	WndParam.iWidth			= m_iWidth;
	WndParam.iHeight		= m_iHeight;
	WndParam.iRefreshRate	= 60;
	WndParam.b32Bit			= false;
	WndParam.bWindowed		= !m_bIsFullScreen;

	m_pWndParam = &WndParam;


	hMutex = CreateMutex(NULL, FALSE, WndParam.pszTitle);

	if(hMutex == NULL || GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hMutex);
		MessageBox(NULL, "Application has already been\nstarted in a different window.",
			"Application already running",
			MB_ICONWARNING | MB_OK);
		
		exit(1);
		return false;
	}

	m_pszTitle		= WndParam.pszTitle;
	m_pszClassName	= WndParam.pszTitle;
	m_b32Bit		= WndParam.b32Bit;
	m_bIsFullScreen = !(WndParam.bWindowed);
	m_hInst			= WndParam.hInst;
	m_iHeight		= WndParam.iHeight;
	m_iWidth		= WndParam.iWidth;
	m_fps			= (float)WndParam.iRefreshRate;

	/*if((m_iHeight <= 0) || (m_iWidth < 0) || (m_iRefreshRate < 0))
	{
		/*
		ErrorHandler::instance()->SetError(EC_Error, "Invalid Parameters\n"
			"Width = %d\n"
			"Height = %d\n"
			"Refresh = %d",
			m_iWidth,
			m_iHeight,
			m_iRefreshRate);
		*/

	/*
		MessageBox(NULL, "Bla", "Bla", 0);
		return false;

	}
*/
	m_WindowClassEx.cbClsExtra		= NULL;
	m_WindowClassEx.cbSize			= sizeof(WNDCLASSEX);
	m_WindowClassEx.cbWndExtra		= NULL;
	m_WindowClassEx.hbrBackground	= NULL;
	m_WindowClassEx.hCursor			= LoadCursor(NULL, IDC_ARROW);
	m_WindowClassEx.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
	m_WindowClassEx.hIconSm			= LoadIcon(NULL, IDI_WINLOGO);
	m_WindowClassEx.hInstance		= m_hInst;
	m_WindowClassEx.lpfnWndProc		= WindowProc;
	m_WindowClassEx.lpszClassName	= m_pszClassName;
	m_WindowClassEx.lpszMenuName	= NULL;
	m_WindowClassEx.style			= CS_HREDRAW | CS_VREDRAW | CS_CLASSDC | CS_DBLCLKS;

	if(!RegisterClassEx(&m_WindowClassEx))
	{
		m_hInst = NULL;
		FLX_LOGGER->Write(LOG_ERROR, "Cannot register %s with Windows.", m_pszClassName);
		MessageBox(NULL, "Cannot register class with Windows.", "Bla", 0);
		return false;
	}

	DWORD dwStyle, dwStyleEx;
	if(m_bIsFullScreen)
	{
		dwStyle = WS_POPUP | WS_VISIBLE | WS_SYSMENU | WS_MINIMIZEBOX;
		dwStyleEx = WS_EX_APPWINDOW;
	}
	else
	{
		dwStyle = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;
		dwStyleEx = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	}

	RECT rcWnd;
	rcWnd.left		= (long)0;
	rcWnd.right		= (long)m_iWidth;
	rcWnd.top		= (long)0;
	rcWnd.bottom	= (long)m_iHeight;
	AdjustWindowRectEx(&rcWnd, dwStyle, FALSE, dwStyleEx);

	m_hWnd = CreateWindowEx(dwStyleEx,
		m_pszClassName,
		m_pszClassName,
		dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		0, 0,
		rcWnd.right - rcWnd.left,
		rcWnd.bottom - rcWnd.top,
		NULL,
		NULL,
		m_hInst,
		(void*) this
		);

	if(m_hWnd == NULL)
	{
		DestroyWnd();
		FLX_LOGGER->Write(LOG_ERROR, "Cannot create window.");
		MessageBox(NULL, "Cannot create Window", "Bla", 0);
		return false;
	}

	WndParam.hWnd = m_hWnd;
	m_hDC = GetDC(m_hWnd);

	if(m_hDC == NULL)
	{
		DestroyWnd();
		FLX_LOGGER->Write(LOG_ERROR, "Could not get Window's DC.");
		MessageBox(NULL, "Could not get Window's DC", "Bla", 0);
		return false;
	}

	WndParam.hDC = m_hDC;

	if(WndParam.bWindowed)
	{
		int cx = GetSystemMetrics(SM_CXSCREEN);
		int cy = GetSystemMetrics(SM_CYSCREEN);
		int px = 0;
		int py = 0;

		if(cx > m_iWidth)
			px = (int)((cx - m_iWidth)/2);
		if(cy > m_iHeight)
			py = (int)((cy - m_iHeight)/2);

		MoveWindow(m_hWnd, px, py, m_iWidth, m_iHeight, FALSE);
	}

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, NULL, sizeof(pfd));
	pfd.nSize			= sizeof(pfd);
	pfd.nVersion		= 1;
	pfd.dwFlags			= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType		= PFD_TYPE_RGBA;
	pfd.cColorBits		= (m_b32Bit ? 32 : 16);
	pfd.cDepthBits		= 32;
	pfd.iLayerType		= PFD_MAIN_PLANE;

	HGLRC hRC = NULL;
	m_hDC = GetDC( m_hWnd );
	int iPixelFormat	= ChoosePixelFormat(m_hDC, &pfd);
	if(!iPixelFormat)
	{
		DestroyWnd();
		FLX_LOGGER->Write(LOG_ERROR, "Could not find suitable OpenGL pixelformat to use.");
		MessageBox(NULL, "Could not find suitable OpenGL pixelformat to use.", "Bla", 0);
		return false;
	}

	if(!SetPixelFormat(m_hDC, iPixelFormat, &pfd))
	{
		DestroyWnd();
		FLX_LOGGER->Write(LOG_ERROR, "Could not set OpenGL pixel format.");
			MessageBox(NULL, "Could not set OpenGL pixel format.", "Bla", 0);
		return false;
	}

	m_hGLRC = wglCreateContext(m_hDC);
	if(m_hGLRC == NULL)
	{
		DestroyWnd();
		FLX_LOGGER->Write(LOG_ERROR, "Could not create OpenGL context.");
		MessageBox(NULL, "Could not create OGL context", "Bla", 0);
		return false;
	}
	if(!wglMakeCurrent(m_hDC, m_hGLRC))
	{
		DestroyWnd();
		FLX_LOGGER->Write(LOG_ERROR, "Could not get information about selectex pixel format.");
		MessageBox(NULL, "Could not get information about selected pixel format", "Bla", 0);
		return false;
	}

	
	GLenum err = glewInit();

    if( err != GLEW_OK )
    {
		FLX_LOGGER->Write(LOG_DEFAULT, "ERROR: %s\n", glewGetErrorString(err));
	}

	  if( m_hGLRC != NULL )
	{
		wglMakeCurrent( NULL, NULL );
		wglDeleteContext( m_hGLRC );
		m_hGLRC = NULL;
	}

	int pf_attr[] =
	{
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,    // Our context will be used with OpenGL
		WGL_RED_BITS_ARB, 8,                // At least 8 bits for RED channel
		WGL_GREEN_BITS_ARB, 8,              // At least 8 bits for GREEN channel
		WGL_BLUE_BITS_ARB, 8,               // At least 8 bits for BLUE channel
		WGL_ALPHA_BITS_ARB, 8,              // At least 8 bits for ALPHA channel
		WGL_DEPTH_BITS_ARB, 32,             // At least 16 bits for depth buffer
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,     // We don require double buffering
		WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,    // Turn Multi-sampling on
		WGL_SAMPLES_ARB, 4,                 // Check For 4x Multi-sampling
		0                                   // Zero terminates the list
	};

	unsigned int count = 0;
	int nPixelFormat;
	wglChoosePixelFormatARB( m_hDC,(const int*)pf_attr, NULL, 1, &nPixelFormat, &count);

	if(count == 0)
		FLX_LOGGER->Write(LOG_DEFAULT, "Could not find an acceptable pixel format!\n");

	m_hDC = GetDC( m_hWnd );
	SetPixelFormat( m_hDC, nPixelFormat, NULL );
	m_hGLRC = wglCreateContext( m_hDC );
	wglMakeCurrent( m_hDC, m_hGLRC );


	m_b32Bit	= (pfd.cColorBits == 32 ? true : false);
	m_bZBuffer	= (pfd.cDepthBits > 0 ? true : false);

	/*if(m_bIsFullScreen)
	{
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));

		dmScreenSettings.dmSize			= sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth	= m_iWidth;
		dmScreenSettings.dmPelsHeight	= m_iHeight;
		dmScreenSettings.dmBitsPerPel	= (m_b32Bit ? 32 : 16);
		dmScreenSettings.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if(ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			DestroyWnd();
			FLX_LOGGER->Write(LOG_ERROR, "Failed to switch to %dx%d fullscreen mode.", m_iWidth, m_iHeight);
			MessageBox(NULL, "Failed to switch to fullscreen mode", "Bla", 0);
			return false;
		}
	}
*/

	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);
	UpdateWindow(m_hWnd);

	return true;
}

LRESULT CALLBACK Application::WindowProc(HWND hWindow, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	Application* pWnd = NULL;
	static bool bProcessed = false;

	switch(iMessage)
	{
	case WM_NCCREATE:
		{
			SetWindowLong(hWindow, GWL_USERDATA, (long)((LPCREATESTRUCT(lParam))->lpCreateParams));
			break;
		}
	default:
		{
			pWnd = (Application*) (LONG_PTR)GetWindowLong(hWindow, GWL_USERDATA);
			if(NULL != pWnd)
			{
				bProcessed = pWnd->MessageHandler(iMessage, wParam, lParam);
			}
		}
	}

	if(!bProcessed)
		return DefWindowProc(hWindow, iMessage, wParam, lParam);

	return 0;
}

bool Application::MessageHandler(UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch(iMessage)
	{
	case WM_CLOSE:
		{
			DestroyWindow(m_hWnd);
			return true;
		}

	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return true;
		}

	case WM_ACTIVATE:
		{
			if(LOWORD(wParam) == WA_INACTIVE)
			{
				m_bActive = false;
				WindowFocusLost();
			}
			else
			{
				m_bActive = true;
				WindowFocusReceived();
			}
			return true;
		}
	case WM_SYSCOMMAND:
		{
			switch(wParam)
			{
				case SC_SCREENSAVE:
				case SC_MONITORPOWER: return true;
				default: return false;
			}
		}
	case WM_KEYDOWN:
		Engine::getInstance()->m_bKeys[wParam] = true;
		return true;
	case WM_KEYUP:
		Engine::getInstance()->m_bKeys[wParam] = false;
		return true;
	case WM_LBUTTONDOWN:
		Engine::getInstance()->m_bLButtonDown = true;
		return true;
	case WM_LBUTTONUP:
		Engine::getInstance()->m_bLButtonDown = false;
		return true;
	case WM_RBUTTONDOWN:
		Engine::getInstance()->m_bRButtonDown = true;
		return true;
	case WM_RBUTTONUP:
		Engine::getInstance()->m_bRButtonDown = false;
		return true;
	case WM_MOUSEWHEEL:
		if((short)HIWORD(wParam) > 0)
			Engine::getInstance()->set_mwheel_rot(1);
		if((short)HIWORD(wParam) < 0)
			Engine::getInstance()->set_mwheel_rot(-1);
		return true;
	default: return false;
	}
}

int Application::Run()
{
	Initialize();

	while(!m_bQuit)
	{
		if(PeekMessage(&m_Message, NULL, 0, 0, PM_REMOVE))
		{
			if(WM_QUIT == m_Message.message)
				m_bQuit = true;
			else
			{
				TranslateMessage(&m_Message);
				DispatchMessage(&m_Message);
			}
		}
		else
		{
			Flux::Engine::getInstance()->Tick();
		}
	}

	return (int)m_Message.wParam;
}

void Application::Initialize()
{
	Flux::Engine::getInstance()->Initialize();
}

void Application::Update()
{

}

void Application::WindowFocusReceived()
{

}
void Application::WindowFocusLost()
{

}



void Application::DestroyWnd()
{
	if(m_bIsFullScreen)
		ChangeDisplaySettings(NULL, 0);

	if(m_hDC)
		wglMakeCurrent(m_hDC, NULL);

	if(m_hGLRC)
	{
		wglDeleteContext(m_hGLRC);
		m_hGLRC = NULL;
	}

	if(m_hWnd)
	{
		UnregisterClass(m_pszClassName, m_hInst);
		m_hInst = NULL;
	}

}
