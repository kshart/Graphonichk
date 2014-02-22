/* 
 * File:   grEvent.h
 * Author: Артём Каширин
 *
 * Created on 27 Август 2013 г., 19:53
 */

#ifndef GREVENT_H
#define	GREVENT_H

#include <vector>
#include "grBaseTypes.h"
using namespace std;

namespace Graphonichk {
	class Event;
	template<class TEvent> class EventDispatcher;//handler
	template<class TEvent, class TObject> class EventDispatcherObject;//handler
	class EventHandler;
	struct EventLinc;
	
	class EventKeyboard;
	class EventMouse;
	class EventWindow;
	class Windows;
	
	struct EventLinc {
		int type;
		void *obj;
		void (*fun)(const Event*);
	};
	class Event {
	  public:
		enum EVENT:char{
			KEYBOARD,
			MOUSE
		};
		Event();
		int type;
		void *obj;
	};
	
	class EventKeyboard :public Event {
	  public:
		enum :int{
			KEY_DOWN,
			KEY_UP
		};
		EventKeyboard();
		bool ctrlKey, shiftKey, altKey;
		char keyCode;
	};
	class EventMouse :public Event {
	  public:
		enum :int{
			MOUSE_DOWN,
			MOUSE_UP,
			MOUSE_MOVE,
			MOUSE_WHEEL
		};
		EventMouse();
		bool ctrlKey, shiftKey, leftKey, middleKey, rightKey, firstX, secondX;
		char keyCode;
		short x, y, wheelDelta;
	};
	class EventWindow :public Event {
	  public:
		enum :int{
			WIN_CREATE,
			WIN_DESTROY,
			WIN_MOVE,
			WIN_SIZE,
			WIN_SHOW,
			WIN_HIDE,
			WIN_ACTIVATE
		};
		EventWindow();
		bool visible;
		Windows *window;
	};
	
	class ShapeRect;
	class EventMouseShape :public Event {
	  public:
		enum :int{
			MOUSE_DOWN,
			MOUSE_UP,
			MOUSE_ROLL_OUT,
			MOUSE_ROLL_OVER,
			MOUSE_MOVE
		};
		EventMouseShape();
		bool ctrlKey, shiftKey, leftKey, middleKey, rightKey, firstX, secondX;
		char keyCode;
		short localx, localy, globalx, globaly;
		ShapeRect *shape;
	};
	
	template<class TEvent> class EventDispatcher {
	  public:
		vector<EventLinc> eventList;
		virtual int callEvent(TEvent*);
		virtual int addEventHandler( int type, void(*)(const TEvent*), void* obj=NULL);
		virtual int removeEventHandler( void(*)(const TEvent*) );
	};
	template<class TEvent, class TObject> class EventDispatcherObject {
	  public:
		vector<EventLinc> eventList;
		virtual int callEvent(TEvent*);
		virtual int addEventHandler( int type, void(TObject::*)(const TEvent*), TObject *obj=NULL);
		virtual int removeEventHandler( void(TObject::*)(const TEvent*) );
	};
}
#endif	/* GREVENT_H */

