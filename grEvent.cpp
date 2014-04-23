#include <vector>
#include "grBaseTypes.h"
#include "grEvent.h"
using namespace std;
using namespace Graphonichk;


Event::Event(int type) :type(type) {
}
Event::Event(int type, void *obj) :type(type), obj(obj) {
}

/*
template<class TEvent> int EventDispatcher<TEvent>::addEventHandler(int type, void(*fun)(const TEvent*), void* obj) {
	EventLinc el;
	el.obj = obj;
	el.type = type;
	el.fun = (void(*)(const Event*))fun;
	this->eventList.push_back( el );
}
template<class TEvent> int EventDispatcher<TEvent>::callEvent(TEvent *event) {
	for(int i=0, s=this->eventList.size(); i<s; i++) {
		if (this->eventList[i].type == event->type) {
			event->obj = this->eventList[i].obj;
			this->eventList[i].fun(event);
		}
	}
}
template<class TEvent> int EventDispatcher<TEvent>::removeEventHandler( void(*fun)(const TEvent*) ) {
	
}
*/
EventKeyboard::EventKeyboard(int type) :Event(type) {
}
EventMouse::EventMouse(int type) :Event(type) {
}
EventMouseShape::EventMouseShape(int type) :Event(type) {
}
EventWindow::EventWindow(int type) :Event(type), window(nullptr) {
}
EventWindow::EventWindow(int type, Windows *window) :Event(type), window(window) {
}
EventFileLoad::EventFileLoad(int type, FileLoad *file) :Event(type), file(file) {
}

//:Event(Event::EVENT_KEYBOARD)