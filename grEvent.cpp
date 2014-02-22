#include <vector>
#include "grBaseTypes.h"
#include "grEvent.h"
using namespace std;
using namespace Graphonichk;


Event::Event() {
	this->type = 1;
	this->obj = NULL;
}

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

EventKeyboard::EventKeyboard() {
	
}

EventMouse::EventMouse() {
	
}
EventMouseShape::EventMouseShape() {
	
}

EventWindow::EventWindow() {
	this->visible = false;
	this->window = NULL;
}
EventFileLoad::EventFileLoad() {
	this->file = NULL;
}

//:Event(Event::EVENT_KEYBOARD)