#include "grWindows.h"
#include <semaphore.h>

using namespace std;
using namespace Graphonichk;

float Screen::dpi = 0;
unsigned short Screen::width = 0;
unsigned short Screen::height = 0;

Device* Device::device = nullptr;
EventDeviceMouse::EventDeviceMouse(int type) :Event(type) {
}
EventDeviceKeyboard::EventDeviceKeyboard(int type) :Event(type) {
}
void* Device::threadUpdateDevices (void* data) {
	while (true) {
		puts("Device::threadUpdateDevices");
		Sleep(10);
	}
	return 0;
}

#if defined(WIN32)
Device::Device() :_dinput(nullptr), _mouseDI(nullptr), _keyboardDI(nullptr) {
	Device::device = this;
	DirectInput8Create(System::hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&this->_dinput, NULL);
	printf("<DirectInput8Create ptr='%i'/>\n", this->_dinput);
	if ( FAILED(DirectInput8Create(System::hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&this->_dinput, NULL)) ) TerminateProcess(System::hInstance, 1);
	TerminateProcess(System::hInstance, 1);
	this->_dinput->EnumDevices(DI8DEVCLASS_ALL, Device::DIEnumDevicesProc, this, DIEDFL_ATTACHEDONLY);
	pthread_create(&this->updateDevicesThread, NULL, Windows::threadRender, nullptr);
}
Device::~Device() {
	pthread_cancel(this->updateDevicesThread);
	Device::device = nullptr;
	if (this->_mouseDI!=nullptr) this->_mouseDI->Unacquire();
	if (this->_keyboardDI!=nullptr) this->_keyboardDI->Unacquire();
	//delete this->_mouseDI;
	//delete this->_keyboardDI;
	//delete this->_dinput;
}
BOOL CALLBACK Device::DIEnumDevicesProc(LPCDIDEVICEINSTANCE inst, LPVOID data) {
	Device *dvc = (Device*)data;
	switch ((char)inst->dwDevType) {
		case DI8DEVTYPE_KEYBOARD:
			puts("DI8DEVTYPE_KEYBOARD ");
			dvc->_dinput->CreateDevice(GUID_SysKeyboard, &dvc->_keyboardDI, NULL);
			if (dvc->_keyboardDI==nullptr) break;
			dvc->_keyboardDI->SetDataFormat(&c_dfDIKeyboard);
			dvc->_keyboardDI->SetCooperativeLevel(Windows::window->hWnd, DISCL_NONEXCLUSIVE);
			dvc->_keyboardDI->Acquire();
			break;
		case DI8DEVTYPE_MOUSE:
			puts("DI8DEVTYPE_MOUSE ");
			dvc->_dinput->CreateDevice(GUID_SysMouse, &dvc->_mouseDI, NULL);
			if (dvc->_mouseDI==nullptr) break;
			dvc->_mouseDI->SetDataFormat(&c_dfDIMouse);
			dvc->_mouseDI->SetCooperativeLevel(Windows::window->hWnd, DISCL_NONEXCLUSIVE);
			dvc->_mouseDI->Acquire();
			break;
	}
	
	printf("DIEnumDevicesProc %s\n", inst->tszInstanceName);
	return DIENUM_CONTINUE;
}


void Windows::regFirstWin() {
	WNDCLASSEX wcx;
	memset(&wcx, 0, sizeof(WNDCLASSEX));
	wcx.cbSize        = sizeof(WNDCLASSEX);
	wcx.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcx.lpfnWndProc   = reinterpret_cast<WNDPROC>(Windows::WndProc);
	wcx.hInstance     = System::hInstance;
	wcx.lpszClassName = WIN_CLASS_NAME;
	wcx.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wcx.hCursor       = LoadCursor(NULL, IDC_ARROW);
	if (!RegisterClassEx(&wcx)) {
		printf("RegisterClassEx fail (%d)\n", GetLastError());
		return;
	}
}
void Windows::show() {
	puts("<Windows message='show'/>");
	SetWindowPos(this->hWnd, HWND_TOP, this->x, this->y, this->width+this->x, this->height+this->y, SWP_SHOWWINDOW );
	ShowWindow(this->hWnd, SW_SHOWNORMAL);
	this->visible = true;
	pthread_mutex_unlock(&this->renderThreadLock);
	ResumeThread(this->renderThread);
	SetForegroundWindow(this->hWnd);
	SetFocus(this->hWnd);
	UpdateWindow(this->hWnd);
	EventWindow *e = new EventWindow( EventWindow::WIN_SHOW, this );
	this->callEvent(e);
	delete e;
}
void Windows::hide() {
	puts("<Windows message='hide'/>");
	ShowWindow(this->hWnd, SW_MINIMIZE);
	pthread_mutex_lock(&this->renderThreadLock);
	this->visible = false;
	EventWindow *e = new EventWindow( EventWindow::WIN_HIDE, this );
	this->callEvent(e);
	delete e;
}
void Windows::close() {
	pthread_cancel(this->renderThread);
	pthread_cancel(this->winThread);
	delete Device::device;
	delete Windows::window->root;
	wglDeleteContext(this->hRC);
	PostQuitMessage(0);
	DestroyWindow(this->hWnd);
	Windows::window = nullptr;
	puts("<window close/>");
	delete this;
}
void Windows::resize(short width, short height) {
	this->width = width;
	this->height = height;
	RECT rect;
	rect.left = this->x;
	rect.top = this->y;
	rect.bottom = this->y+this->height;
	rect.right = this->x+this->width;
	AdjustWindowRectEx (&rect, WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX, FALSE, WS_EX_COMPOSITED|WS_EX_APPWINDOW|WS_EX_TOPMOST);
	GetClientRect(this->hWnd, &rect);
	this->width  = rect.right-rect.left;
	this->height = rect.bottom-rect.top;

	OpenGL::viewMatrixBuffer[0] = Matrix3D::ViewOrtho(0, this->width, 0, this->height, -1, 1);
	this->root->setRect(this->width, this->height);
	printf("setRect %i %i\n", this->width, this->height);
	EventWindow *e = new EventWindow( EventWindow::WIN_SIZE, this );
	this->callEvent(e);
	delete e;
}
void Windows::fullScreen() {
	this->x = 0;
	this->y = 0;
	this->width = Screen::width;
	this->height = Screen::height;
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.bottom = this->height;
	rect.right = this->width;
	SetWindowLong(this->hWnd, GWL_STYLE, WS_POPUP);
	SetWindowLong(this->hWnd, GWL_EXSTYLE, WS_EX_TOPMOST);
	ShowWindow(this->hWnd,SW_SHOWMAXIMIZED);
	SetWindowPos(this->hWnd, HWND_TOPMOST, 0, 0, Screen::width, Screen::height, SWP_SHOWWINDOW);
	SetCapture(this->hWnd);
	
	OpenGL::viewMatrixBuffer[0] = Matrix3D::ViewOrtho(0, this->width, 0, this->height, -1, 1);
	EventWindow *e = new EventWindow( EventWindow::WIN_SIZE );
	e->window = this;
	this->callEvent(e);
	delete e;
}

void* Windows::threadWindow (void* sys) {
	Windows::regFirstWin();
	Windows *win = Windows::window;
	printf("windowThread\n");
	win->hWnd = CreateWindowEx(WS_EX_COMPOSITED|WS_EX_APPWINDOW, WIN_CLASS_NAME, "111", WS_CAPTION|WS_SYSMENU|WS_SIZEBOX|WS_MINIMIZEBOX|WS_VISIBLE , win->x, win->y, win->width, win->height, NULL, NULL, System::hInstance, NULL);
	if (!Windows::window->hWnd) {
		printf("<Error str='CreateWindowEx fail (%d)'/>\n", GetLastError());
		return 0;
	}
	if ( !(win->hDC = GetDC(win->hWnd)) ) {
		printf("<Error str='GetDC fail (%d)'/>\n", GetLastError());
		return 0;
	}
	
	sem_t semaphore;
	sem_init(&semaphore, 0, 0);
	pthread_create(&win->renderThread, NULL, Windows::threadRender, &semaphore);
	sem_wait(&semaphore);
	sem_destroy(&semaphore);
	
	ShowWindow(win->hWnd, SW_SHOW);
	SetForegroundWindow(win->hWnd);
	UpdateWindow(win->hWnd);
	SetFocus(win->hWnd);
	sem_post((sem_t*)sys);
	
	
	Device *device = new Device();
	MSG msg;
	while (IsWindow(win->hWnd)) {
		//puts("Windows::threadWindow");
		while(PeekMessage(&msg, win->hWnd, 0, 0, PM_REMOVE)) DispatchMessage(&msg);
		Sleep(4);
	}
	return 0;
}
void* Windows::threadRender (void* sys) {
	if (sys==nullptr) return 0;
	int format;
	PIXELFORMATDESCRIPTOR pfd;
	HGLRC hRCTemp;
	Windows *win = Windows::window;
	printf("<RenderThread hDC='%i' hRC='%i'/>\n", win->hDC, win->hRC);
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion   = 1;
	pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	format = ChoosePixelFormat(win->hDC, &pfd);
	if (!format || !SetPixelFormat(win->hDC, format, &pfd)) {
		printf("<Error str='Setting pixel format fail (%d)'/>\n", GetLastError());
		return 0;
	}
	if (!(hRCTemp = wglCreateContext(win->hDC)) || !wglMakeCurrent(win->hDC, hRCTemp)) {
		printf("<Error str='reating temp render context fail (%d)'/>\n", GetLastError());
		return 0;
	}
	
	
	/*pfd = {
			sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
			1,											// Version Number
			PFD_DRAW_TO_WINDOW |						// Format Must Support Window
			PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
			PFD_DOUBLEBUFFER,							// Must Support Double Buffering
			PFD_TYPE_RGBA,								// Request An RGBA Format
			32,											// Select Our Color Depth
			0, 0, 0, 0, 0, 0,							// Color Bits Ignored
			8,											// 8-bit alpha
			0,											// Shift Bit Ignored
			0,											// No Accumulation Buffer
			0, 0, 0, 0,									// Accumulation Bits Ignored
			24,											// 24Bit Z-Buffer (Depth Buffer)  
			8,											// 8-bit Stencil Buffer
			0,											// No Auxiliary Buffer
			PFD_MAIN_PLANE,								// Main Drawing Layer
			0,											// Reserved
			0, 0, 0										// Layer Masks Ignored
		};
	int pixelFormat=0;
    bool valid;
    UINT numFormats = 0;
	float fAttributes[] = {0,0};
	int attributes[] = {
		WGL_SUPPORT_OPENGL_ARB, 1, // Must support OGL rendering
		WGL_DRAW_TO_WINDOW_ARB, 1, // pf that can run a window
		WGL_ACCELERATION_ARB,   1, // must be HW accelerated
		WGL_COLOR_BITS_ARB,     24, // 8 bits of each R, G and B
		WGL_DEPTH_BITS_ARB,     16, // 16 bits of depth precision for window
		WGL_DOUBLE_BUFFER_ARB,  GL_TRUE, // Double buffered context
		WGL_SAMPLE_BUFFERS_ARB, GL_TRUE, // MSAA on
		WGL_SAMPLES_ARB,        8, // 8x MSAA 
		WGL_PIXEL_TYPE_ARB,      WGL_TYPE_RGBA_ARB, // pf should be RGBA type
		0}; // NULL termination};
	const int cca_list[] = {
		0,0,
        WGL_CONTEXT_MAJOR_VERSION_ARB,  3,
	    WGL_CONTEXT_MINOR_VERSION_ARB,  3,
	    WGL_CONTEXT_FLAGS_ARB,          WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
	    WGL_CONTEXT_PROFILE_MASK_ARB,   WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
        0,0
    };
	PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribivARB = (PFNWGLGETPIXELFORMATATTRIBIVARBPROC)wglGetProcAddress("wglGetPixelFormatAttribivARB");
	PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
	valid = wglChoosePixelFormatARB(win->hDC, attributes, fAttributes, 1, &pixelFormat, &numFormats);
	
	int attrib[] = { WGL_SAMPLES_ARB };
	int nResults = 0;
	bool resas = wglGetPixelFormatAttribivARB(win->hDC, pixelFormat, 0, 1, attrib, &nResults);
	printf("valid %i %i %i %i %i %i %i\n", valid, pixelFormat, format, numFormats, nResults, resas, GetPixelFormat(win->hDC) );
	printf("1 (%d)\n", GetLastError());
	if ( !valid ) {
		MessageBox( NULL, "Invalid Pixel Format", "Error! (SetupWGLPixelFormat)", MB_OK );
		return false;
	}
	DescribePixelFormat(win->hDC, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
	format = ChoosePixelFormat(win->hDC, &pfd);
	//DescribePixelFormat(win->hDC, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
	printf("2 (%d)\n", GetLastError());
	SetPixelFormat (win->hDC, format, &pfd);
	printf("valid %i %i %i %i %i %i %i\n", valid, pixelFormat, format, numFormats, nResults, resas, GetPixelFormat(win->hDC) );
	printf("3 (%d)\n", GetLastError());
	
	
	if (!wglCreateContextAttribsARB) {
		printf("wglCreateContextAttribsARB fail (%d)\n", GetLastError());
		return 0;
	}
	printf("4 (%d)\n", GetLastError());
	win->hRC = wglCreateContextAttribsARB(win->hDC, 0, cca_list);
	if (!win->hRC || !wglMakeCurrent(win->hDC, win->hRC)) {
		printf("Creating render context fail (%d)\n", GetLastError());
		return 0;
	}
	wglDeleteContext(hRCTemp);
	wglMakeCurrent(win->hDC, win->hRC);
	printf("5 (%d)\n", GetLastError());*/
	win->hRC = hRCTemp;
	Screen::width = GetDeviceCaps(win->hDC,HORZRES);
	Screen::height = GetDeviceCaps(win->hDC,VERTRES);
	Screen::dpi = ( Screen::width/(float)GetDeviceCaps(win->hDC,HORZSIZE) )*25.4;
	printf("<LCD res='%i %i' dpi='%f'/>\n", Screen::width, Screen::height, Screen::dpi );
	OpenGL::init(OGL_VER);
	win->root = new ShapeMain();
	win->root->chengeRect = false;
	win->resize(win->width, win->height);
	sem_post((sem_t*)sys);
	
	LARGE_INTEGER frequencyStruct;
	float frequency, time;
	QueryPerformanceFrequency(&frequencyStruct);
	frequency = (float)frequencyStruct.QuadPart;
	while (IsWindow(win->hWnd)) {
		pthread_mutex_lock(&win->renderThreadLock);
		LARGE_INTEGER time1, time2;
		float lastTime, fps;
		QueryPerformanceCounter(&time1);
		#ifdef REDRAWN_BY_THE_ACTION
		if (!win->renderComplete) {
			win->redraw();
		}
		#endif
		win->redraw();
		win->eachFrame.performTasks();
		Bitmap::updateBitmaps();
		QueryPerformanceCounter(&time2);
		time = (float)(time2.QuadPart-time1.QuadPart)/frequency;
		win->suspendProcess.setTime( TIME_IN_FRAME_MS-time );
		win->suspendProcess.performTasks();
		lastTime = win->suspendProcess.getTime();
		fps = 1/lastTime;
		
		//printf("fps %f\n", fps );
		if ( lastTime > 0 ) {
			Sleep( (int)( 1000*lastTime ) );
		}
		pthread_mutex_unlock(&win->renderThreadLock);
	}
	return 0;
}
LRESULT CALLBACK Windows::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	EventDeviceKeyboard	*eventDKey			= nullptr;
	EventDeviceMouse	*eventDMouse		= nullptr;
	EventKeyboard		*eventKey			= nullptr;
	EventMouse			*eventMouse			= nullptr;
	EventMouseShape		*eventMouseShape	= nullptr;
	EventWindow			*eventWin			= nullptr;
	
	Windows *win = Windows::window;
	
	if (win->hWnd == hWnd) {
		switch (msg) {
			/*	case WM_KEYDOWN:
				case WM_SETFOCUS:
				case WM_KILLFOCUS:
				case WM_ACTIVATE:
				case WA_CLICKACTIVE:
				case WM_CLOSE:
				fprintf(stdout, "WndProc CLOSE\n");
				PostQuitMessage(0);
				return 0;
			case WM_DESTROY:
				fprintf(stdout, "WndProc WM_DESTROY\n");
				PostQuitMessage(0);
				return 0;*/
			case WM_CHAR:
				eventKey = new EventKeyboard( EventKeyboard::CHAR_INPUT );
				eventKey->charCode = wParam;
				win->events.keyboard.callEvent(eventKey);
				delete eventKey;
				return 0;
			case WM_KEYUP:
				eventDKey = new EventDeviceKeyboard( EventDeviceKeyboard::DEVICE_KEYBOARD_UP );
				Device::device->events.keyboard.callEvent(eventDKey);
				eventKey = new EventKeyboard( EventKeyboard::KEY_UP );
				eventKey->keyCode = (char)wParam;
				win->events.keyboard.callEvent(eventKey);
				delete eventDKey;
				delete eventKey;
				return 0;
			case WM_KEYDOWN:
				eventDKey = new EventDeviceKeyboard( EventDeviceKeyboard::DEVICE_KEYBOARD_DOWN );
				Device::device->events.keyboard.callEvent(eventDKey);
				eventKey = new EventKeyboard( EventKeyboard::KEY_DOWN );
				eventKey->keyCode = (char)wParam;
				win->events.keyboard.callEvent(eventKey);
				delete eventDKey;
				delete eventKey;
				MSG message;
				message.hwnd = hWnd;
				message.message = msg;
				message.wParam = wParam;
				message.lParam = lParam;
				TranslateMessage(&message);
				return 0;
			
			case WM_MOUSEMOVE:
				eventDMouse = new EventDeviceMouse( EventDeviceMouse::DEVICE_MOUSE_MOVE );
				Device::device->events.mouse.callEvent(eventDMouse);
				
				eventMouse = new EventMouse( EventMouse::MOUSE_MOVE );
				eventMouse->x = GET_X_LPARAM(lParam); 
				eventMouse->y = GET_Y_LPARAM(lParam);
				eventMouse->ctrlKey = (wParam&MK_CONTROL);
				eventMouse->shiftKey = (wParam&MK_SHIFT);
				win->events.mouse.callEvent(eventMouse);
				
				eventMouseShape = new EventMouseShape( EventMouseShape::MOUSE_MOVE );
				eventMouseShape->globalx = GET_X_LPARAM(lParam);
				eventMouseShape->globaly = GET_Y_LPARAM(lParam);
				eventMouseShape->ctrlKey = (wParam&MK_CONTROL);
				eventMouseShape->shiftKey = (wParam&MK_SHIFT);
				win->root->callEvent(eventMouseShape);
				
				delete eventDMouse;
				delete eventMouse;
				delete eventMouseShape;
				return 0;
			case WM_MOUSEWHEEL:
				eventDMouse = new EventDeviceMouse( EventDeviceMouse::DEVICE_MOUSE_MOVE );
				Device::device->events.mouse.callEvent(eventDMouse);
				
				eventMouse = new EventMouse( EventMouse::MOUSE_WHEEL );
				eventMouse->wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
				eventMouse->ctrlKey = (wParam&MK_CONTROL);
				eventMouse->shiftKey = (wParam&MK_SHIFT);
				win->events.mouse.callEvent(eventMouse);
				
				delete eventDMouse;
				delete eventMouse;
				return 0;
			case WM_LBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case 0x020B://WM_XBUTTONDOWN
				eventDMouse = new EventDeviceMouse( EventDeviceMouse::DEVICE_MOUSE_KEYDOWN );
				Device::device->events.mouse.callEvent(eventDMouse);
				
				eventMouse = new EventMouse( EventMouse::MOUSE_DOWN );
				eventMouse->ctrlKey = (wParam&MK_CONTROL);
				eventMouse->shiftKey = (wParam&MK_SHIFT);
				eventMouse->leftKey = (wParam&MK_LBUTTON);
				eventMouse->middleKey = (wParam&MK_MBUTTON);
				eventMouse->rightKey = (wParam&MK_RBUTTON);
				eventMouse->firstX = (wParam&0x0020);//MK_XBUTTON1
				eventMouse->secondX = (wParam&0x0040);//MK_XBUTTON2
				eventMouse->x = GET_X_LPARAM(lParam);
				eventMouse->y = GET_Y_LPARAM(lParam);
				win->events.mouse.callEvent(eventMouse);
				
				eventMouseShape = new EventMouseShape( EventMouseShape::MOUSE_DOWN );
				eventMouseShape->ctrlKey = (wParam&MK_CONTROL);
				eventMouseShape->shiftKey = (wParam&MK_SHIFT);
				eventMouseShape->leftKey = (wParam&MK_LBUTTON);
				eventMouseShape->middleKey = (wParam&MK_MBUTTON);
				eventMouseShape->rightKey = (wParam&MK_RBUTTON);
				eventMouseShape->firstX = (wParam&0x0020);
				eventMouseShape->secondX = (wParam&0x0040);
				eventMouseShape->globalx = GET_X_LPARAM(lParam);
				eventMouseShape->globaly = GET_Y_LPARAM(lParam);
				win->root->callEvent(eventMouseShape);
				
				delete eventDMouse;
				delete eventMouse;
				delete eventMouseShape;
				return 0;
			case WM_LBUTTONUP:
			case WM_MBUTTONUP:
			case WM_RBUTTONUP:
			case 0x020C://WM_XBUTTONUP
				eventDMouse = new EventDeviceMouse( EventDeviceMouse::DEVICE_MOUSE_KEYUP );
				Device::device->events.mouse.callEvent(eventDMouse);
				
				eventMouse = new EventMouse( EventMouse::MOUSE_UP );
				eventMouse->ctrlKey = (wParam&MK_CONTROL);
				eventMouse->shiftKey = (wParam&MK_SHIFT);
				eventMouse->leftKey = (wParam&MK_LBUTTON);
				eventMouse->middleKey = (wParam&MK_MBUTTON);
				eventMouse->rightKey = (wParam&MK_RBUTTON);
				eventMouse->firstX = (wParam&0x0020);//MK_XBUTTON1
				eventMouse->secondX = (wParam&0x0040);//MK_XBUTTON2
				eventMouse->x = GET_X_LPARAM(lParam); 
				eventMouse->y = GET_Y_LPARAM(lParam);
				win->events.mouse.callEvent(eventMouse);
				
				eventMouseShape = new EventMouseShape( EventMouseShape::MOUSE_UP );
				eventMouseShape->ctrlKey = (wParam&MK_CONTROL);
				eventMouseShape->shiftKey = (wParam&MK_SHIFT);
				eventMouseShape->leftKey = (wParam&MK_LBUTTON);
				eventMouseShape->middleKey = (wParam&MK_MBUTTON);
				eventMouseShape->rightKey = (wParam&MK_RBUTTON);
				eventMouseShape->firstX = (wParam&0x0020);
				eventMouseShape->secondX = (wParam&0x0040);
				eventMouseShape->globalx = GET_X_LPARAM(lParam);
				eventMouseShape->globaly = GET_Y_LPARAM(lParam);
				win->root->callEvent(eventMouseShape);
				
				delete eventDMouse;
				delete eventMouse;
				delete eventMouseShape;
				return 0;
			case WM_PAINT:
				if (win->visible) win->renderComplete = false;
				return 0;
			case WM_MOVE:
				win->x = (short) LOWORD(lParam);
				win->y = (short) HIWORD(lParam);
				
				eventWin = new EventWindow( EventWindow::WIN_MOVE, win );
				win->callEvent(eventWin);
				delete eventWin;
				
				return 0;
			case WM_SIZING:
				RECT rect;
				rect.left	= (short)((RECT*)lParam)->left;
				rect.top	= (short)((RECT*)lParam)->top;
				rect.bottom = (short)((RECT*)lParam)->bottom;
				rect.right	= (short)((RECT*)lParam)->right;
				AdjustWindowRectEx (&rect, WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX, FALSE, WS_EX_COMPOSITED|WS_EX_APPWINDOW|WS_EX_TOPMOST);
				GetClientRect(win->hWnd, &rect);
				win->width  = rect.right-rect.left;
				win->height = rect.bottom-rect.top;
				//Matrix3D vm = 
				OpenGL::viewMatrixBuffer[0] = Matrix3D::ViewOrtho(0, win->width, 0, win->height, -1, 1);
				win->root->setRect(win->width, win->height);
				
				eventWin = new EventWindow( EventWindow::WIN_SIZE, win );
				win->callEvent(eventWin);
				delete eventWin;
				
				return TRUE;
			case WM_SYSCOMMAND:
				printf("\tWM_SYSCOMMAND %i\n", wParam&0xFFF0);
				switch (wParam & 0xFFF0) {
					case SC_CLOSE :
						win->close();
						return 0;
					case SC_MINIMIZE:
						win->hide();
						return 0;
					case SC_RESTORE:
						win->show();
						return 0;
				}
			case WM_GETICON:
				switch (wParam) {
					case ICON_BIG:
					case ICON_SMALL:
					//case ICON_SMALL2:
						return DefWindowProc(hWnd, msg, wParam, lParam);
				}
			case WM_CREATE:
				break;
		}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
#elif defined(X11)
void Windows::close() {
	pthread_exit(&this->renderThread);
	pthread_exit(&this->winThread);
	glXMakeCurrent( this->x11display, 0, 0 );
	glXDestroyContext( this->x11display, this->x11context );
	XDestroyWindow( this->x11display, this->x11window );
	//XFreeColormap( this->x11display, cmap );
	XCloseDisplay( this->x11display );
	Windows::window = nullptr;
	puts("<Windows message='close'/>");
	delete this;
}
void Windows::resize(short width, short height) {
	this->width = width;
	this->height = height;
	XResizeWindow(this->x11display, this->x11window, this->width, this->height);

	OpenGL::viewMatrixBuffer[0] = Matrix3D::ViewOrtho(0, this->width, 0, this->height, -1, 1);
	this->root->setRect(this->width, this->height);
	EventWindow *e = new EventWindow( EventWindow::WIN_SIZE, this );
	this->callEvent(e);
	delete e;
}

#endif

Windows *Windows::window = nullptr;
Windows::Windows(short x, short y, short width, short height) :
		x(x), 
		y(y), 
		width(width), 
		height(height),
		visible(false), 
		renderComplete(false) {
	if (Windows::window!=nullptr) return;
	Windows::regFirstWin();
	ProcessingThread::init();
	
	this->mainFileLibrary = new FileLibrary("shader330.tar.lz");
	
	puts("<Windows message='start WIN32'/>");
	Windows::window = this;
	this->renderThreadLock = PTHREAD_MUTEX_INITIALIZER;
	
	sem_t semaphore;
	sem_init(&semaphore, 0, 0);
	pthread_create(&this->winThread, NULL, Windows::threadWindow, &semaphore);
	sem_wait(&semaphore);
	sem_destroy(&semaphore);
	
	Font::init();
	this->eachFrame.addTask(&ShapeRectTask::task);
	puts("<Windows message='end'/>");
	/*int attributes[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB,	3,
		WGL_CONTEXT_MINOR_VERSION_ARB,	3,
		WGL_CONTEXT_FLAGS_ARB,			WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, 0};
	OPENGL_GET_PROC(PFNWGLCREATECONTEXTATTRIBSARBPROC, wglCreateContextAttribsARB);
	if (!wglCreateContextAttribsARB) {
		fprintf(stderr, "wglCreateContextAttribsARB fail (%d)\n", GetLastError());
		return;
	}
	Windows::hRC = wglCreateContextAttribsARB(Windows::hDC, 0, attributes);
	if (!Windows::hRC|| !wglMakeCurrent(Windows::hDC, Windows::hRC)) {// <editor-fold defaultstate="collapsed" desc="***">
		fprintf(stderr, "Creating render context fail (%d)\n", GetLastError());
		return;
	}// </editor-fold>
	wglDeleteContext(hRCTemp);*/
}
void Windows::redraw() {
	OpenGL::clearViewMatrix();
	switch (OpenGL::ver) {
		case OpenGL::VER_CORE_100:
			this->root->renderGL100();
			break;
		case OpenGL::VER_CORE_330:
			this->root->renderGL330();
			break;
	}
	//glFlush( );
	#if defined(WIN32)
		SwapBuffers(this->hDC);
	#elif defined(X11)
		glXSwapBuffers(this->x11display, this->x11context);
	#endif
	this->renderComplete = true;
}
/*int ft_init () {
	FT_Library library;
	FT_Face face;
	//FT_Glyph  glyph;
	FT_GlyphSlot gs;
	FT_Vector  delta;
	FT_Matrix matrix;
	FT_BitmapGlyphRec bmp;
	int error;
	
	matrix.xx = (FT_Fixed)( cos( 0 ) * 0x10000L );
	matrix.xy = (FT_Fixed)(-sin( 0 ) * 0x10000L );
	matrix.yx = (FT_Fixed)( sin( 0 ) * 0x10000L );
	matrix.yy = (FT_Fixed)( cos( 0 ) * 0x10000L );
	delta.x = 0;
	delta.y = 0;
	
	error = FT_Init_FreeType( &library );
	if (error) return error;
	printf("FT_Init %i\n", error);
	error = FT_New_Face( library, "C:/Windows/Fonts/arial.ttf", 0, &face );
	if (error) return error;
	printf("FT_Init %i\n", error);
	printf("FT_Face %i\n", face->num_glyphs);
	//error = FT_Set_Pixel_Sizes(face, 0, 30);
	error = FT_Set_Char_Size(face, 0, 30*64, 72, 72 );
	//FT_Set_Transform(face, &matrix, &delta);
	error = FT_Load_Glyph( face, FT_Get_Char_Index(face, 'W'), FT_LOAD_DEFAULT );
	//error = FT_Get_Glyph( face->glyph, &glyph );
	error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_LIGHT   );
	gs = face->glyph;
	printf(" width %i ", gs->metrics.width>>6);
	printf(" height %i\n", gs->metrics.height>>6);
	//printf("FT_Init %i\n", error);
	//printf("FT_Init %i\n", error);
	printf("FT_Init %i\n", error);
	glRasterPos2s(100, 100);
	glPixelZoom(1,-1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glDrawPixels(gs->bitmap.width, gs->bitmap.rows, GL_ALPHA, GL_UNSIGNED_BYTE, gs->bitmap.buffer);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	//printf("GLERR %i\n");
	return error;
}*/