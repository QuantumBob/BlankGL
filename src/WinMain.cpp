#include <windows.h>
#include "cApp.h"

///////////////////////////////////////////////////////////

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	cApp* _app;
	bool result;

	// Create the system object.
	_app = new cApp();
	if (!_app)
	{
		return 0;
	}

	// Initialize and run the system object.
	result = _app->Initialize(hInstance);
	if (result)
	{
		_app->Run();
	}

	// Shutdown and release the system object.
	_app->Shutdown();
	delete _app;
	_app = 0;

	return 0;
}
