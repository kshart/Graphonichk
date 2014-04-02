#include "FlatUI.h"
using namespace std;
using namespace FlatUI;
using namespace Graphonichk;

Graphonichk::Texture *MainFlatUI::flRB = nullptr;
Graphonichk::Texture *MainFlatUI::flRBDisable = nullptr;
Graphonichk::Texture *MainFlatUI::flRBChk = nullptr;
Graphonichk::Texture *MainFlatUI::flRBRollOver = nullptr;
Graphonichk::Texture *MainFlatUI::flCB = nullptr;
Graphonichk::Texture *MainFlatUI::flCBDisable = nullptr;
Graphonichk::Texture *MainFlatUI::flCBChk = nullptr;
Graphonichk::Texture *MainFlatUI::flCBRollOver = nullptr;

int MainFlatUI::init() {
	MainFlatUI::flRB = new Graphonichk::Texture("FlatUIRadioButton.png");
	MainFlatUI::flRBDisable = new Graphonichk::Texture("FlatUIRadioButtonDisable.png");
	MainFlatUI::flRBChk = new Graphonichk::Texture("FlatUIRadioButtonCheck.png");
	MainFlatUI::flRBRollOver = new Graphonichk::Texture("FlatUIRadioButtonRollOver.png");
	MainFlatUI::flCB = new Graphonichk::Texture("FlatUICheckBox.png");
	MainFlatUI::flCBDisable = new Graphonichk::Texture("FlatUICheckBoxDisable.png");
	MainFlatUI::flCBChk = new Graphonichk::Texture("FlatUICheckBoxCheck.png");
	MainFlatUI::flCBRollOver = new Graphonichk::Texture("FlatUICheckBoxRollOver.png");
	return true;
}


void FlatUICheckboxMouseUp (const EventMouse *e) {
	FlatUICheckbox *flsb = (FlatUICheckbox*)(e->obj);
	short localx, localy;
	EventCheckButton *event;
	if (flsb->press) {
		flsb->press = false;
		localx = e->x - flsb->globalx;
		localy = e->y - flsb->globaly;
		if (localx>0 && localx<flsb->width && localy>0 && localy<flsb->height) {
			if (flsb->checked) {
				flsb->checked = false;
				flsb->status = UICheckbox::UNCHECKED_NORMAL;
				flsb->tex = MainFlatUI::flCB;
				event = new EventCheckButton();
				event->checked = false;
				event->type = EventCheckButton::CHECK_UPDATE;
				event->obj = flsb;
				flsb->UICheckbox::callEvent(event);
				delete event;
			}else{
				flsb->checked = true;
				flsb->status = UICheckbox::CHECKED_NORMAL;
				flsb->tex = MainFlatUI::flCBChk;
				event = new EventCheckButton();
				event->checked = true;
				event->type = EventCheckButton::CHECK_UPDATE;
				event->obj = flsb;
				flsb->UICheckbox::callEvent(event);
				delete event;
			}
		}else{
			if (flsb->checked) {
				flsb->status = UICheckbox::CHECKED_NORMAL;
				flsb->tex = MainFlatUI::flCBChk;
			}else{
				flsb->status = UICheckbox::UNCHECKED_NORMAL;
				flsb->tex = MainFlatUI::flCB;
			}
		}
		#ifdef REDRAWN_BY_THE_ACTION
		Windows::window->renderComplete = false;
		#endif
	}
}
FlatUICheckbox::FlatUICheckbox() :Bitmap(MainFlatUI::flCB) {
	this->mouseEventActive = true;
	Windows::window->events.mouse.addEventHandler(EventMouse::MOUSE_UP, FlatUICheckboxMouseUp, this);
}
int FlatUICheckbox::callEvent(EventMouseShape* event) {
	if (this->status!=UICheckbox::CHECKED_DISABLE && this->status!=UICheckbox::UNCHECKED_DISABLE) {
		switch (event->type) {
			case EventMouseShape::MOUSE_DOWN:
				this->press = true;
				if (this->checked) {
					this->status = UICheckbox::CHECKED_PRESS;
					this->tex = MainFlatUI::flCBChk;
				}else{
					this->status = UICheckbox::UNCHECKED_PRESS;
					this->tex = MainFlatUI::flCBRollOver;
				}
				#ifdef REDRAWN_BY_THE_ACTION
					Windows::window->renderComplete = false;
					#endif
				return true;
			case EventMouseShape::MOUSE_ROLL_OUT:
				if (this->checked) {
					this->status = UICheckbox::CHECKED_NORMAL;
					this->tex = MainFlatUI::flCBChk;
				}else{
					this->status = UICheckbox::UNCHECKED_NORMAL;
					this->tex = MainFlatUI::flCB;
				}
				#ifdef REDRAWN_BY_THE_ACTION
					Windows::window->renderComplete = false;
					#endif
				return true;
			case EventMouseShape::MOUSE_ROLL_OVER:
				if (this->checked) {
					if (this->press) {
						this->status = UICheckbox::CHECKED_PRESS;
						this->tex = MainFlatUI::flCBChk;
					}else{
						this->status = UICheckbox::CHECKED_ROLLOVER;
						this->tex = MainFlatUI::flCBChk;
					}
				}else{
					if (this->press) {
						this->status = UICheckbox::UNCHECKED_PRESS;
						this->tex = MainFlatUI::flCBChk;
					}else{
						this->status = UICheckbox::UNCHECKED_ROLLOVER;
						this->tex = MainFlatUI::flCBRollOver;
					}
				}
				#ifdef REDRAWN_BY_THE_ACTION
					Windows::window->renderComplete = false;
					#endif
				return true;
		}
	}else{
		this->tex = MainFlatUI::flCBDisable;
	}
}


void FlatUIRadioButtonMouseUp (const EventMouse *e) {
	FlatUIRadioButton *flsb = (FlatUIRadioButton*)(e->obj);
	short localx, localy;
	EventCheckButton *event;
	if (flsb->press) {
		flsb->press = false;
		localx = e->x - flsb->globalx;
		localy = e->y - flsb->globaly;
		if (localx>0 && localx<flsb->width && localy>0 && localy<flsb->height) {
			if (flsb->checked) {
				if (flsb->group==nullptr) {
					flsb->checked = false;
					flsb->status = UIRadioButton::UNCHECKED_NORMAL;
					flsb->tex = MainFlatUI::flRB;
					event = new EventCheckButton();
					event->checked = false;
					event->type = EventCheckButton::CHECK_UPDATE;
					event->obj = flsb;
					flsb->UIRadioButton::callEvent(event);
					delete event;
				}
			}else{
				if (flsb->group==nullptr) {
					flsb->checked = true;
					flsb->status = UIRadioButton::CHECKED_NORMAL;
					flsb->tex = MainFlatUI::flRBChk;
					event = new EventCheckButton();
					event->checked = true;
					event->type = EventCheckButton::CHECK_UPDATE;
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
				flsb->tex = MainFlatUI::flRBChk;
			}else{
				flsb->status = UIRadioButton::UNCHECKED_NORMAL;
				flsb->tex = MainFlatUI::flRB;
			}
		}
		#ifdef REDRAWN_BY_THE_ACTION
		Windows::window->renderComplete = false;
		#endif
	}
}
FlatUIRadioButton::FlatUIRadioButton() :Bitmap(MainFlatUI::flRB) {
	this->mouseEventActive = true;
	Windows::window->events.mouse.addEventHandler(EventMouse::MOUSE_UP, FlatUIRadioButtonMouseUp, this);
}
int FlatUIRadioButton::callEvent(EventMouseShape* event) {
	if (this->status!=UIRadioButton::CHECKED_DISABLE && this->status!=UIRadioButton::UNCHECKED_DISABLE) {
		switch (event->type) {
			case EventMouseShape::MOUSE_DOWN:
				this->press = true;
				if (this->checked) {
					this->status = UIRadioButton::CHECKED_PRESS;
					this->tex = MainFlatUI::flRBChk;
				}else{
					this->status = UIRadioButton::UNCHECKED_PRESS;
					this->tex = MainFlatUI::flRBRollOver;
				}
				#ifdef REDRAWN_BY_THE_ACTION
					Windows::window->renderComplete = false;
					#endif
				return true;
			case EventMouseShape::MOUSE_ROLL_OUT:
				if (this->checked) {
					this->status = UIRadioButton::CHECKED_NORMAL;
					this->tex = MainFlatUI::flRBChk;
				}else{
					this->status = UIRadioButton::UNCHECKED_NORMAL;
					this->tex = MainFlatUI::flRB;
				}
				#ifdef REDRAWN_BY_THE_ACTION
					Windows::window->renderComplete = false;
					#endif
				return true;
			case EventMouseShape::MOUSE_ROLL_OVER:
				if (this->checked) {
					if (this->press) {
						this->status = UIRadioButton::CHECKED_PRESS;
						this->tex = MainFlatUI::flRBChk;
					}else{
						this->status = UIRadioButton::CHECKED_ROLLOVER;
						this->tex = MainFlatUI::flRBChk;
					}
				}else{
					if (this->press) {
						this->status = UIRadioButton::UNCHECKED_PRESS;
						this->tex = MainFlatUI::flRBChk;
					}else{
						this->status = UIRadioButton::UNCHECKED_ROLLOVER;
						this->tex = MainFlatUI::flRBRollOver;
					}
				}
				#ifdef REDRAWN_BY_THE_ACTION
					Windows::window->renderComplete = false;
					#endif
				return true;
		}
	}else{
		this->tex = MainFlatUI::flRBDisable;
	}
}
void FlatUIRadioButton::changeCheck() {
	if (this->checked) {
		this->status = UIRadioButton::CHECKED_NORMAL;
		this->tex = MainFlatUI::flRBChk;
	}else{
		this->status = UIRadioButton::UNCHECKED_NORMAL;
		this->tex = MainFlatUI::flRB;
	}
}