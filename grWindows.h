#ifndef GRWINDOWS_H
#define	GRWINDOWS_H

//#include "grProcessingQueue.h"
#include "grMain.h"

#define WIN_CLASS_NAME "grEWin"

//Graphonichk
using namespace std;
namespace Graphonichk {
	class EventDeviceMouse :public Event {
	public:
		enum :int{
			DEVICE_MOUSE_KEYDOWN,
			DEVICE_MOUSE_KEYUP,
			DEVICE_MOUSE_MOVE
		};
		EventDeviceMouse( int type );
		int x, y, z;
		char button[8];
	};
	class EventDeviceKeyboard :public Event {
	public:
		enum :int{
			DEVICE_KEYBOARD_UP,
			DEVICE_KEYBOARD_DOWN
		};
		EventDeviceKeyboard(int type);
		char key[256];
	};
	class Device {
	private:
		pthread_t updateDevicesThread;
		static void* threadUpdateDevices(void* data);
		#ifdef WIN32
			static BOOL CALLBACK DIEnumDevicesProc(LPCDIDEVICEINSTANCE inst, LPVOID data);
			IDirectInput8 *_dinput;
			
			IDirectInputDevice8 *_mouseDI;
			IDirectInputDevice8 *_keyboardDI;
		#else
		#endif
	public:
		Device();
		~Device();
		
		struct {
			EventDispatcher<EventDeviceKeyboard> keyboard;
			EventDispatcher<EventDeviceMouse> mouse;
		} events;
		
		static Device* device;
	};
	
	class Windows :public EventDispatcher<EventWindow> {
	private:
		pthread_t winThread, renderThread;
		pthread_mutex_t renderThreadLock;
		static void* threadRender (void* data_args);
		static void* threadWindow (void* data_args);
		#ifdef WIN32
			HWND hWnd;
			HGLRC hRC;
			HDC hDC;
			static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
			friend Device;
		#else
			X11lib::Display *x11display;
			X11lib::Window x11window;
			X11lib::GLXContext x11context;
			X11lib::XEvent x11event;
			static bool x11EventProc();
		#endif
	public:
		static Windows* window;
		static void regFirstWin();
		static void deleteLastWin();
		
		Windows(short x, short y, short w, short h);
		void close();
		void hide();
		void show();
		void setFocus();
		void killFocus();
		void resize(short width, short height);
		void fullScreen();
		
		//void saveAsXML();
		
		void redraw();
		
		struct {
			EventDispatcher<EventKeyboard> keyboard;
			EventDispatcher<EventMouse> mouse;
		} events;
		
		FileLibrary *mainFileLibrary;
		
		ProcessingEachFrame eachFrame;
		ProcessingSupSuspend suspendProcess;

		ShapeMain *root;
		bool visible, renderComplete;
		short x, y, width, height;
	};
	class Screen {
	public:
		static float dpi;
		static unsigned short width, height;
	};
}

#endif	/* GRWINDOWS_H */

