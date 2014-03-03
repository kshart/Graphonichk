/* 
 * File:   grWindows.h
 * Author: Артём Каширин
 *
 * Created on 25 Август 2013 г., 19:32
 */

#ifndef GRWINDOWS_H
#define	GRWINDOWS_H

#include <stack>
#include <vector>
#include <string>
#include <stdio.h>
#include <windows.h>
#include <windowsx.h>

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
	public:
		static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
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
		#ifdef WIN32
			HWND hWnd;
			HGLRC hRC;
			HDC hDC;
			HANDLE winThread, renderThread;
			DWORD winThreadID, renderThreadID;
		#endif
		//vector<Bitmap*> bitmapUpdateBuffer;
		vector<Buffer*> FBOBuffer;
		ShapeGroupRect *root;
		bool visible, renderComplete;
		short x, y, width, height;
		int dpi;
	};
	
}

#endif	/* GRWINDOWS_H */

