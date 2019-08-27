#ifndef _APP_H_
#define _APP_H_

// Including SDKDDKVer.h defines the highest available Windows platform.
// If you wish to build your application for a previous Windows platform, include WinSDKVer.h and
// set the _WIN32_WINNT macro to the platform you wish to support before including SDKDDKVer.h.

#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include "windows.h"

class cOpenGL;
class cInput;
class cGraphics;
class cWindow;

class cApp
{
public:
	cApp();
	cApp(const cApp&);
	~cApp();

	static cApp* App();

	void ShowMessage(LPCSTR message, LPCSTR location = "");
	bool Initialize(HINSTANCE hInstance);
	void Shutdown();
	void Run();

private:

	bool Frame();
	bool ShutdownWindows();

	LPCSTR m_application_name;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	cWindow* m_window;
	cOpenGL* m_opengl;
	cInput* m_input;
	cGraphics* m_graphics;

};

// FUNCTION PROTOTYPES 
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


// GLOBALS 
static cApp* application_handle = 0;


#endif // !_APP_H_