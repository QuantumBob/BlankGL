#include "WinMain.h"
#include "MyGL.h"

MyGL mygl;

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {

	// create the main class
	
	mygl.Init();
	// use main class to create the window
	// if it fails quit
	if (mygl.getWindow()->create(hInstance, nCmdShow) != 0) {
		PostQuitMessage(1);
	}

	MSG msg;
	bool active = true;
	// windows loop until WM_QUIT then acitve is false
	while (active) {
		// do windows messaging here
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				active = false; // kills the windows loop
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// do graphics rendering and input here
		mygl.getRenderer()->render();
		mygl.getRenderer()->swapBuffers(mygl.getWindow()->DC);
	}
	// program has quit so destroy window
	mygl.getWindow()->destroy();

	return msg.wParam;
}
