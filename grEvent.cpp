#include <vector>
#include "grBaseTypes.h"
#include "grEvent.h"
using namespace std;
using namespace grEngine;


grEngine::Event::Event() {
	this->type = 1;
	this->obj = NULL;
}

template<class TEvent> int grEngine::EventDispatcher<TEvent>::addEventHandler(int type, void(*fun)(const TEvent*), void* obj) {
	EventLinc el;
	el.obj = obj;
	el.type = type;
	el.fun = (void(*)(const Event*))fun;
	this->eventList.push_back( el );
}
template<class TEvent> int grEngine::EventDispatcher<TEvent>::callEvent(TEvent *event) {
	for(int i=0, s=this->eventList.size(); i<s; i++) {
		if (this->eventList[i].type == event->type) {
			event->obj = this->eventList[i].obj;
			this->eventList[i].fun(event);
		}
	}
}
template<class TEvent> int grEngine::EventDispatcher<TEvent>::removeEventHandler( void(*fun)(const TEvent*) ) {
	
}

grEngine::EventKeyboard::EventKeyboard() {
	
}

grEngine::EventMouse::EventMouse() {
	
}
grEngine::EventMouseShape::EventMouseShape() {
	
}

grEngine::EventWindow::EventWindow() {
	this->visible = false;
	this->window = NULL;
}
grEngine::EventFileLoad::EventFileLoad() {
	this->file = NULL;
}

//:Event(Event::EVENT_KEYBOARD)