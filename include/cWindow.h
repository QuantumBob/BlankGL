#ifndef _RWK_WINDOW_H_
#define _RWK_WINDOW_H_

#include <windows.h>

class cWindow {

private:
	
	struct Config {
		int width;
		int height;
		int posX;
		int posY;
		bool windowed;
	} config;

public:

	cWindow();
	~cWindow();

	HWND CreateFake(HINSTANCE);
	HWND Create(HINSTANCE hInstance, int nCmdShow);
	void Show();
	ATOM RegisterWindowClass(HINSTANCE hInstance);
	HDC GetWindowDC();
	void AdjustSize();
	void Center();
	void Destroy();
	static LRESULT CALLBACK s_WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	inline int GetWindowWidth() { return config.width; }
	inline int GetWindowHeight() { return config.height; }

private:
	LPTSTR m_window_class;// Window Class
	HWND m_hwnd;		// Handle to window
	HDC m_DC;			// Device Context
	HGLRC m_RC;			// Rendering Context
	DWORD style;
};
#endif

