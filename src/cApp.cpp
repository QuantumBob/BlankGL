#include "cApp.h"
#include "cWindow.h"
#include "cOpenGL.h"
#include "cGraphics.h"
#include "cInput.h"



///////////////////////////////////////////////////////////

cApp::cApp() : m_opengl(0), m_hinstance(0)
{
}

///////////////////////////////////////////////////////////

cApp::cApp(const cApp&)
{
}

///////////////////////////////////////////////////////////

cApp::~cApp()
{
}

///////////////////////////////////////////////////////////

cApp* cApp::App()
{
	if (application_handle == 0)
	{
		application_handle = new cApp;
	}
	return application_handle;
}

///////////////////////////////////////////////////////////

void cApp::ShowMessage(LPCSTR message, LPCSTR location) {

	MessageBox(0, message, location, MB_ICONERROR);
}

///////////////////////////////////////////////////////////

bool cApp::Initialize(HINSTANCE in_hInstance)
{
	DEVMODE dm_screen_settings;
	int screenWidth, screenHeight;
	bool result;
	int pos_x, pos_y;

	m_hinstance = in_hInstance;

	// Initialize the width and height of the screen to zero.
	screenWidth = 0;
	screenHeight = 0;

	// Create the window object
	m_window = new cWindow;
	if (!m_window)
	{
		return false;
	}

	HWND fake_hwnd = m_window->CreateFake(m_hinstance);
	if (fake_hwnd == NULL)
	{
		return false;
	}
	else
	{
		ShowWindow(fake_hwnd, SW_HIDE); // Don't show the window
	}

	// Create the OpenGL object.
	m_opengl = new cOpenGL;
	if (!m_opengl)
	{
		return false;
	}

	result = m_opengl->InitializeExtensions(fake_hwnd);
	if (!result)
	{
		MessageBox(m_hwnd, "Could not initialize the OpenGL extensions.", "Error", MB_OK);
		return false;
	}

	DestroyWindow(fake_hwnd);
	fake_hwnd = nullptr;

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (FULL_SCREEN)
	{
		// Determine the resolution of the clients desktop screen.
		screenWidth = GetSystemMetrics(SM_CXSCREEN);
		screenHeight = GetSystemMetrics(SM_CYSCREEN);

		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dm_screen_settings, 0, sizeof(dm_screen_settings));
		dm_screen_settings.dmSize = sizeof(dm_screen_settings);
		dm_screen_settings.dmPelsWidth = (unsigned long)screenWidth;
		dm_screen_settings.dmPelsHeight = (unsigned long)screenHeight;
		dm_screen_settings.dmBitsPerPel = 32;
		dm_screen_settings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dm_screen_settings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		pos_x = pos_y = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth = m_window->GetWindowWidth();
		screenHeight = m_window->GetWindowHeight();

		// Place the window in the middle of the screen.
		pos_x = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		pos_y = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	m_hwnd = m_window->Create(m_hinstance, SW_SHOWDEFAULT);

	// Initialize OpenGL now that the window has been created.
	result = m_opengl->InitializeOpenGL(m_hwnd, m_window, SCREEN_FAR, SCREEN_NEAR, VSYNC_ENABLED);
	if (!result)
	{
		ShowMessage("Could not initialize OpenGL, check if video card supports OpenGL 4.0.", " in cOpenGL::InitializeOpenGL(...)");
		return false;
	}

	SetWindowText(m_hwnd, reinterpret_cast<LPCSTR>(glGetString(GL_VERSION)));

	m_window->Show();

	// Create the input object.  This object will be used to handle reading the input from the user.
	m_input = new cInput;
	if (!m_input)
	{
		return false;
	}

	// Initialize the input object.
	m_input->Initialize();

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	m_graphics = new cGraphics(m_opengl);
	if (!m_graphics)
	{
		return false;
	}

	float screen_aspect = (float)screenWidth / (float)screenHeight;

	// Initialize the graphics object.
	result = m_graphics->Initialize(m_hwnd, screen_aspect);
	if (!result)
	{
		return false;
	}

	return true;
}

///////////////////////////////////////////////////////////

void cApp::Shutdown()
{
	// Release the OpenGL object.
	if (m_opengl)
	{
		delete m_opengl;
		m_opengl = 0;
	}

	// Shutdown the window.
	ShutdownWindows();

	return;
}

///////////////////////////////////////////////////////////

void cApp::Run()
{
	MSG msg;
	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	bool active = true;
	while (active)
	{
		// Handle the windows messages.
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				active = false;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}		
		// Do the frame processing.
		if (!Frame())
		{
			active = false;
		}
	}
	return;
}

///////////////////////////////////////////////////////////


bool cApp::Frame()
{
	// Check if the user pressed escape and wants to exit the application.
	if (m_input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	// Do the frame processing for the graphics object.
	if (!m_graphics->Frame())
	{
		return false;
	}
	return true;
}

///////////////////////////////////////////////////////////

bool cApp::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_application_name, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	application_handle = NULL;

	return true;
}
