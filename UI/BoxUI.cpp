#include "BoxUI.h"
using namespace std;
using namespace BoxUI;
using namespace Graphonichk;


ubvec4 MainBoxUI::color[] = {
	ubvec4(22, 0, 42, 20),
	ubvec4(22, 0, 42, 20),
	ubvec4(44, 0, 64, 20),
	ubvec4(66, 0, 86, 20),
	ubvec4(88, 0, 108, 20),
	ubvec4(0, 22, 20, 42),
	ubvec4(22, 22, 42, 42),
	ubvec4(44, 22, 64, 42),
	ubvec4(66, 22, 86, 42),
	ubvec4(88, 22, 108, 42),
	ubvec4(0xFF, 0xFF, 0xFF, 0xFF),
	ubvec4(0xD6, 0xE2, 0xD5, 0xFF),
	ubvec4(0x3B, 0xC7, 0x2E, 0xFF),
};

void BoxUIButtonMouseUp (const EventMouse *e) {
	BoxUIButton *flsb = (BoxUIButton*)(e->obj);
	short localx, localy;
	EventCheckButton *event;
	if (flsb->press) {
		flsb->press = false;
		localx = e->x - flsb->getGlobalX();
		localy = e->y - flsb->getGlobalY();
		if (localx>0 && localx<flsb->getWidth() && localy>0 && localy<flsb->getHeight()) {
			flsb->status = UIButton::ROLLOVER;
			flsb->backgroundColor = MainBoxUI::color[MainBoxUI::BUTTON_BOX_ROLL_OVER];
			//flsb->setRectID(MainBoxUI::BUTTON_BOX_ROLL_OVER);
			//event = new EventCheckButton( EventCheckButton::CHECK_UPDATE );
			//event->checked = true;
			//event->obj = flsb;
			//flsb->UICheckbox::callEvent(event);
			//delete event;
		}else{
			flsb->status = UIButton::NORMAL;
			flsb->backgroundColor = MainBoxUI::color[MainBoxUI::BUTTON_BOX];
			//flsb->setRectID(MainBoxUI::BUTTON_BOX);
		}
		#ifdef REDRAWN_BY_THE_ACTION
		Windows::window->renderComplete = false;
		#endif
	}
}
BoxUIButton::BoxUIButton(unsigned short w, unsigned short h) :FRect(w, h, MainBoxUI::color[MainBoxUI::BUTTON_BOX]) {
	this->mouseEventActive = true;
	Windows::window->events.mouse.addEventHandler(EventMouse::MOUSE_UP, BoxUIButtonMouseUp, this);
}
int BoxUIButton::callEvent(EventMouseShape* event) {
	if (true) {
		switch (event->type) {
			case EventMouseShape::MOUSE_DOWN:
				this->press = true;
				this->status = BoxUIButton::PRESS;
				this->backgroundColor = MainBoxUI::color[MainBoxUI::BUTTON_BOX_PRESSED];
				//this->setRectID(MainBoxUI::BUTTON_BOX_PRESSED);
				#ifdef REDRAWN_BY_THE_ACTION
				Windows::window->renderComplete = false;
				#endif
				return true;
			case EventMouseShape::MOUSE_ROLL_OUT:
				this->status = BoxUIButton::NORMAL;
				this->backgroundColor = MainBoxUI::color[MainBoxUI::BUTTON_BOX];
				//this->setRectID(MainBoxUI::BUTTON_BOX);
				#ifdef REDRAWN_BY_THE_ACTION
				Windows::window->renderComplete = false;
				#endif
				return true;
			case EventMouseShape::MOUSE_ROLL_OVER:
				if (this->press) {
					this->status = BoxUIButton::PRESS;
					this->backgroundColor = MainBoxUI::color[MainBoxUI::BUTTON_BOX_PRESSED];
					//this->setRectID(MainBoxUI::BUTTON_BOX_PRESSED);
				}else{
					this->status = BoxUIButton::ROLLOVER;
					this->backgroundColor = MainBoxUI::color[MainBoxUI::BUTTON_BOX_ROLL_OVER];
					//this->setRectID(MainBoxUI::BUTTON_BOX_ROLL_OVER);
				}
				#ifdef REDRAWN_BY_THE_ACTION
				Windows::window->renderComplete = false;
				#endif
				return true;
		}
	}else{
		//this->setRectID(MainBoxUI::CHECK_BOX_DISABLE);
	}
	this->ShapeRect::callEvent(event);
}
void BoxUICheckboxMouseUp (const EventMouse *e) {
	BoxUICheckbox *flsb = (BoxUICheckbox*)(e->obj);
	short localx, localy;
	EventCheckButton *event;
	if (flsb->press) {
		flsb->press = false;
		localx = e->x - flsb->getGlobalX();
		localy = e->y - flsb->getGlobalY();
		if (localx>0 && localx<flsb->getWidth() && localy>0 && localy<flsb->getHeight()) {
			if (flsb->checked) {
				flsb->checked = false;
				flsb->status = UICheckbox::UNCHECKED_NORMAL;
				flsb->backgroundColor = MainBoxUI::color[MainBoxUI::CHECK_BOX];
				event = new EventCheckButton( EventCheckButton::CHECK_UPDATE );
				event->checked = false;
				event->obj = flsb;
				flsb->UICheckbox::callEvent(event);
				delete event;
			}else{
				flsb->checked = true;
				flsb->status = UICheckbox::CHECKED_NORMAL;
				flsb->backgroundColor = MainBoxUI::color[MainBoxUI::CHECK_BOX_CHECKED];
				event = new EventCheckButton( EventCheckButton::CHECK_UPDATE );
				event->checked = true;
				event->obj = flsb;
				flsb->UICheckbox::callEvent(event);
				delete event;
			}
		}else{
			if (flsb->checked) {
				flsb->status = UICheckbox::CHECKED_NORMAL;
				flsb->backgroundColor = MainBoxUI::color[MainBoxUI::CHECK_BOX_CHECKED];
			}else{
				flsb->status = UICheckbox::UNCHECKED_NORMAL;
				flsb->backgroundColor = MainBoxUI::color[MainBoxUI::CHECK_BOX];
			}
		}
		#ifdef REDRAWN_BY_THE_ACTION
		Windows::window->renderComplete = false;
		#endif
	}
}
BoxUICheckbox::BoxUICheckbox() :FRect(24, 24, MainBoxUI::color[MainBoxUI::CHECK_BOX]) {
	this->mouseEventActive = true;
	Windows::window->events.mouse.addEventHandler(EventMouse::MOUSE_UP, BoxUICheckboxMouseUp, this);
}
int BoxUICheckbox::callEvent(EventMouseShape* event) {
	if (this->status!=UICheckbox::CHECKED_DISABLE && this->status!=UICheckbox::UNCHECKED_DISABLE) {
		switch (event->type) {
			case EventMouseShape::MOUSE_DOWN:
				this->press = true;
				if (this->checked) {
					this->status = UICheckbox::CHECKED_PRESS;
					this->backgroundColor = MainBoxUI::color[MainBoxUI::CHECK_BOX_CHECKED];
				}else{
					this->status = UICheckbox::UNCHECKED_PRESS;
					this->backgroundColor = MainBoxUI::color[MainBoxUI::CHECK_BOX_ROLL_OVER];
				}
				#ifdef REDRAWN_BY_THE_ACTION
				Windows::window->renderComplete = false;
				#endif
				return true;
			case EventMouseShape::MOUSE_ROLL_OUT:
				if (this->checked) {
					this->status = UICheckbox::CHECKED_NORMAL;
					this->backgroundColor = MainBoxUI::color[MainBoxUI::CHECK_BOX_CHECKED];
				}else{
					this->status = UICheckbox::UNCHECKED_NORMAL;
					this->backgroundColor = MainBoxUI::color[MainBoxUI::CHECK_BOX];
				}
				#ifdef REDRAWN_BY_THE_ACTION
				Windows::window->renderComplete = false;
				#endif
				return true;
			case EventMouseShape::MOUSE_ROLL_OVER:
				if (this->checked) {
					if (this->press) {
						this->status = UICheckbox::CHECKED_PRESS;
						this->backgroundColor = MainBoxUI::color[MainBoxUI::CHECK_BOX_CHECKED];
					}else{
						this->status = UICheckbox::CHECKED_ROLLOVER;
						this->backgroundColor = MainBoxUI::color[MainBoxUI::CHECK_BOX_CHECKED];
					}
				}else{
					if (this->press) {
						this->status = UICheckbox::UNCHECKED_PRESS;
						this->backgroundColor = MainBoxUI::color[MainBoxUI::CHECK_BOX_CHECKED];
					}else{
						this->status = UICheckbox::UNCHECKED_ROLLOVER;
						this->backgroundColor = MainBoxUI::color[MainBoxUI::CHECK_BOX_ROLL_OVER];
					}
				}
				#ifdef REDRAWN_BY_THE_ACTION
				Windows::window->renderComplete = false;
				#endif
				return true;
		}
	}else{
		this->backgroundColor = MainBoxUI::color[MainBoxUI::CHECK_BOX_DISABLE];
	}
}


void BoxUIRadioButtonMouseUp (const EventMouse *e) {
	BoxUIRadioButton *flsb = (BoxUIRadioButton*)(e->obj);
	short localx, localy;
	EventCheckButton *event;
	if (flsb->press) {
		flsb->press = false;
		localx = e->x - flsb->getGlobalX();
		localy = e->y - flsb->getGlobalY();
		if (localx>0 && localx<flsb->getWidth() && localy>0 && localy<flsb->getHeight()) {
			if (flsb->checked) {
				if (flsb->group==nullptr) {
					flsb->checked = false;
					flsb->status = UIRadioButton::UNCHECKED_NORMAL;
					flsb->backgroundColor = MainBoxUI::color[MainBoxUI::RADIO_BUTTON];
					event = new EventCheckButton( EventCheckButton::CHECK_UPDATE );
					event->checked = false;
					event->obj = flsb;
					flsb->UIRadioButton::callEvent(event);
					delete event;
				}
			}else{
				if (flsb->group==nullptr) {
					flsb->checked = true;
					flsb->status = UIRadioButton::CHECKED_NORMAL;
					flsb->backgroundColor = MainBoxUI::color[MainBoxUI::RADIO_BUTTON_CHECKED];
					event = new EventCheckButton( EventCheckButton::CHECK_UPDATE );
					event->checked = true;
					event->obj = flsb;
					flsb->UIRadioButton::callEvent(event);
					delete event;
				}else{
					flsb->group->setActiveButton(flsb);
				}
			}
		}else{
			if (flsb->checked) {
				flsb->status = UIRadioButton::CHECKED_NORMAL;
				flsb->backgroundColor = MainBoxUI::color[MainBoxUI::RADIO_BUTTON_CHECKED];
			}else{
				flsb->status = UIRadioButton::UNCHECKED_NORMAL;
				flsb->backgroundColor = MainBoxUI::color[MainBoxUI::RADIO_BUTTON];
			}
		}
		#ifdef REDRAWN_BY_THE_ACTION
		Windows::window->renderComplete = false;
		#endif
	}
}
BoxUIRadioButton::BoxUIRadioButton() :FRect(24, 24, MainBoxUI::color[MainBoxUI::RADIO_BUTTON]) {
	this->mouseEventActive = true;
	Windows::window->events.mouse.addEventHandler(EventMouse::MOUSE_UP, BoxUIRadioButtonMouseUp, this);
}
int BoxUIRadioButton::callEvent(EventMouseShape* event) {
	if (this->status!=UIRadioButton::CHECKED_DISABLE && this->status!=UIRadioButton::UNCHECKED_DISABLE) {
		switch (event->type) {
			case EventMouseShape::MOUSE_DOWN:
				this->press = true;
				if (this->checked) {
					this->status = UIRadioButton::CHECKED_PRESS;
					this->backgroundColor = MainBoxUI::color[MainBoxUI::RADIO_BUTTON_CHECKED];
				}else{
					this->status = UIRadioButton::UNCHECKED_PRESS;
					this->backgroundColor = MainBoxUI::color[MainBoxUI::RADIO_BUTTON_ROLL_OVER];
				}
				#ifdef REDRAWN_BY_THE_ACTION
					Windows::window->renderComplete = false;
					#endif
				return true;
			case EventMouseShape::MOUSE_ROLL_OUT:
				if (this->checked) {
					this->status = UIRadioButton::CHECKED_NORMAL;
					this->backgroundColor = MainBoxUI::color[MainBoxUI::RADIO_BUTTON_CHECKED];
				}else{
					this->status = UIRadioButton::UNCHECKED_NORMAL;
					this->backgroundColor = MainBoxUI::color[MainBoxUI::RADIO_BUTTON];
				}
				#ifdef REDRAWN_BY_THE_ACTION
					Windows::window->renderComplete = false;
					#endif
				return true;
			case EventMouseShape::MOUSE_ROLL_OVER:
				if (this->checked) {
					if (this->press) {
						this->status = UIRadioButton::CHECKED_PRESS;
						this->backgroundColor = MainBoxUI::color[MainBoxUI::RADIO_BUTTON_CHECKED];
					}else{
						this->status = UIRadioButton::CHECKED_ROLLOVER;
						this->backgroundColor = MainBoxUI::color[MainBoxUI::RADIO_BUTTON_CHECKED];
					}
				}else{
					if (this->press) {
						this->status = UIRadioButton::UNCHECKED_PRESS;
						this->backgroundColor = MainBoxUI::color[MainBoxUI::RADIO_BUTTON_CHECKED];
					}else{
						this->status = UIRadioButton::UNCHECKED_ROLLOVER;
						this->backgroundColor = MainBoxUI::color[MainBoxUI::RADIO_BUTTON_ROLL_OVER];
					}
				}
				#ifdef REDRAWN_BY_THE_ACTION
					Windows::window->renderComplete = false;
					#endif
				return true;
		}
	}else{
		this->backgroundColor = MainBoxUI::color[MainBoxUI::RADIO_BUTTON_DISABLE];
	}
}
void BoxUIRadioButton::changeCheck() {
	if (this->checked) {
		this->status = UIRadioButton::CHECKED_NORMAL;
		this->backgroundColor = MainBoxUI::color[MainBoxUI::RADIO_BUTTON_CHECKED];
	}else{
		this->status = UIRadioButton::UNCHECKED_NORMAL;
		this->backgroundColor = MainBoxUI::color[MainBoxUI::RADIO_BUTTON];
	}
}