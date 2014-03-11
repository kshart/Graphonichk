/* 
 * File:   grWindows.h
 * Author: РђСЂС‚С‘Рј РљР°С€РёСЂРёРЅ
 *
 * Created on 25 РђРІРіСѓСЃС‚ 2013 Рі., 19:32
 */

#ifndef GRWINDOWS_H
#define	GRWINDOWS_H


#include "grBaseTypes.h"
#include "grEvent.h"

#define WIN_CLASS_NAME "grEWin"

//Graphonichk
using namespace std;
namespace Graphonichk {
	template class EventDispatcher<EventWindow>;
	template class EventDispatcher<EventKeyboard>;
	template class EventDispatcher<EventMouse>;
	class Windows :public EventDispatcher<EventWindow> {
	private:
		#ifdef WIN32
			HWND hWnd;
			HGLRC hRC;
			HDC hDC;
			HANDLE winThread, renderThread;
			DWORD winThreadID, renderThreadID;
			static DWORD WINAPI threadRender (void* sys);
			static DWORD WINAPI threadWindow (void* sys);
			static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
		#else
			
			Display *x11display;
			Window x11window;
			GLXContext x11context;
			XEvent x11event;
			pthread_t winThread, renderThread;
			static void* threadRender (void* vptr_args);
			static void* threadWindow (void* vptr_args);
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
		
		void saveAsXML();
		
		void redraw();
		void redrawFBO();
		void resize(short, short);
		void loop();

		
		struct {
			EventDispatcher<EventKeyboard> keyboard;
			EventDispatcher<EventMouse> mouse;
		} events;
		
		vector<Buffer*> FBOBuffer;
		ShapeGroupRect *root;
		bool visible, renderComplete;
		short x, y, width, height;
		int dpi;
	};
	
}

#endif	/* GRWINDOWS_H */

