/* 
 * File:   grEvent.h
 * Author: Артём Каширин
 *
 * Created on 27 Август 2013 г., 19:53
 */

#ifndef GREVENT_H
#define	GREVENT_H
#include "grMain.h"
using namespace std;

namespace Graphonichk {
	class Event;
	template<class TEvent> class EventDispatcher;
	template<class TEvent, class TObject> class EventDispatcherObject;
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
		Event(int type);
		Event(int type, void *obj);
		int type;
		void *obj;
	};
	
	class EventKeyboard :public Event {
	  public:
		enum :int{
			KEY_DOWN,
			KEY_UP
		};
		EventKeyboard(int type);
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
		EventMouse(int type);
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
		EventWindow(int type);
		EventWindow(int type, Windows *window);
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
		EventMouseShape(int type);
		bool ctrlKey, shiftKey, leftKey, middleKey, rightKey, firstX, secondX;
		char keyCode;
		short localx, localy, globalx, globaly;
		ShapeRect *shape;
	};
	
	template<class TEvent> class EventDispatcher {
	  public:
		vector<EventLinc> eventList;
		virtual int callEvent(TEvent *event) {
			for(int i=0, s=this->eventList.size(); i<s; i++) {
				if (this->eventList[i].type == event->type) {
					event->obj = this->eventList[i].obj;
					this->eventList[i].fun(event);
				}
			}
			return true;
		}
		virtual int addEventHandler(int type, void(*fun)(const TEvent*), void* obj=NULL) {
			EventLinc el;
			el.obj = obj;
			el.type = type;
			el.fun = (void(*)(const Event*))fun;
			this->eventList.push_back( el );
			return true;
		}
		virtual int removeEventHandler( void(*fun)(const TEvent*) ) {
			fputs("Event::removeEventHandler\n", iovir);
			return false;
		}
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

