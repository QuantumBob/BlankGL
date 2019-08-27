#include "cWindow.h"
#include "cApp.h"
//#include <gl/gl.h>
//#include "gl/glext.h"
//#include "gl/wglext.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

///////////////////////////////////////////////////////////

cWindow::cWindow() : m_RC(0), m_hwnd(0), m_DC(0), m_window_class(nullptr) {

	config.width = 720;
	config.height = 720;
	config.posX = CW_USEDEFAULT;
	config.posY = 0;
	config.windowed = true;
	style = CS_OWNDC | WS_CAPTION | WS_SYSMENU | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SIZEBOX;
}

///////////////////////////////////////////////////////////

cWindow::~cWindow() {
}

///////////////////////////////////////////////////////////

HWND cWindow::CreateFake(HINSTANCE hInstance)
{
	m_window_class = MAKEINTATOM(RegisterWindowClass(hInstance));
	if (m_window_class == 0) {
		cApp::App()->ShowMessage("RegisterClassEx() failed.", " in cWindow::CreateFake()");
		return NULL;
	}

	// create temporary window
	HWND fakeWND = CreateWindow(m_window_class,		// class is set above
		"Fake Window",		// title of fake window
		style,				// set in constructor
		0, 0,				// position x, y
		1, 1,				// width, height
		NULL, NULL,			// parent window, menu
		hInstance, NULL);	// instance, param

	return fakeWND;
}

///////////////////////////////////////////////////////////

HWND cWindow::Create(HINSTANCE hInstance, int nCmdShow) {

	if (m_window_class == 0) {
		m_window_class = MAKEINTATOM(RegisterWindowClass(hInstance));
	}
	if (m_window_class == 0) {
		cApp::App()->ShowMessage("RegisterClassEx() failed.", " in cWindow::Create()");
		return NULL;
	}
	
	if (config.windowed == true) {
		AdjustSize();
		Center();
	}

	// create a new window and context

	m_hwnd = CreateWindow(
		m_window_class, "OpenGL Window",	// class name, window name
		style,							// styles
		config.posX, config.posY,		// posx, posy. If x is set to CW_USEDEFAULT y is ignored
		config.width, config.height,	// width, height
		NULL, NULL,						// parent window, menu
		hInstance, this);				// instance, param

	if (m_hwnd == NULL)
	{
		return NULL;
	}

	return m_hwnd;
}

///////////////////////////////////////////////////////////

void cWindow::Show()
{
	ShowWindow(m_hwnd, SW_SHOWDEFAULT); // WM_SIZE called here
}

///////////////////////////////////////////////////////////

ATOM cWindow::RegisterWindowClass(HINSTANCE hInstance) {

	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = s_WindowProcedure;
	wcex.hInstance = hInstance;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpszClassName = "Core";

	return RegisterClassEx(&wcex);
}

///////////////////////////////////////////////////////////

HDC cWindow::GetWindowDC()
{
	return m_DC;
}

///////////////////////////////////////////////////////////
// Adjust window's size for non-client area elements like border and title bar
void cWindow::AdjustSize() {

	RECT rect = { 0, 0, config.width, config.height };
	AdjustWindowRect(&rect, style, false);
	config.width = rect.right - rect.left;
	config.height = rect.bottom - rect.top;
}

///////////////////////////////////////////////////////////

void cWindow::Center() {

	RECT primaryDisplaySize;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &primaryDisplaySize, 0);	// system taskbar and application desktop toolbars not included
	config.posX = (primaryDisplaySize.right - config.width) / 2;
	config.posY = (primaryDisplaySize.bottom - config.height) / 2;
}

///////////////////////////////////////////////////////////

void cWindow::Destroy() {

	wglMakeCurrent(NULL, NULL);
	if (m_RC) {
		wglDeleteContext(m_RC);
	}
	if (m_DC) {
		ReleaseDC(m_hwnd, m_DC);
	}
	if (m_hwnd) {
		DestroyWindow(m_hwnd);
	}
}

///////////////////////////////////////////////////////////

LRESULT cWindow::s_WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	cWindow* p_this;
	if (message == WM_NCCREATE)
	{
		LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		p_this = static_cast<cWindow*>(lpcs->lpCreateParams);
		// Put the value in a safe place for future use
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(p_this));
	}
	else
	{
		p_this = reinterpret_cast<cWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	}

	if (p_this)
	{
		return p_this->WindowProcedure(hWnd, message, wParam, lParam);
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

///////////////////////////////////////////////////////////

LRESULT CALLBACK cWindow::WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	// use win32 to process controls using handler below
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message) {
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			PostQuitMessage(0);
		}
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		if (wParam != SIZE_MINIMIZED)
		{
			config.width = LOWORD(lParam);
			config.height = HIWORD(lParam);
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;		// message handled
}

