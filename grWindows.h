#ifndef GRWINDOWS_H
#define	GRWINDOWS_H


#include "grBaseTypes.h"

#define WIN_CLASS_NAME "grEWin"

//Graphonichk
using namespace std;
namespace Graphonichk {
	class Windows :public EventDispatcher<EventWindow> {
	private:
		THREAD_H winThread, renderThread;
		static THREAD threadRender (void* data_args);
		static THREAD threadWindow (void* data_args);
		#ifdef WIN32
			HWND hWnd;
			HGLRC hRC;
			HDC hDC;
			DWORD winThreadID, renderThreadID;
			static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
		#else
			Display *x11display;
			Window x11window;
			GLXContext x11context;
			XEvent x11event;
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
		
		void saveAsXML();
		
		void redraw();
		
		struct {
			EventDispatcher<EventKeyboard> keyboard;
			EventDispatcher<EventMouse> mouse;
		} events;
		
		ProcessingQueue<EachFrameTask> eachFrame;
		
		ShapeGroupRect *root;
		bool visible, renderComplete;
		short x, y, width, height;
		int dpi;
	};
}

#endif	/* GRWINDOWS_H */

