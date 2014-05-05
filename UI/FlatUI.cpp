#include "FlatUI.h"
using namespace std;
using namespace FlatUI;
using namespace Graphonichk;

Graphonichk::Texture *MainFlatUI::flImage = nullptr;

int MainFlatUI::init() {
	usvec4 rect[13] = {
		usvec4(0, 0, 20, 20),
		usvec4(22, 0, 42, 20),
		usvec4(44, 0, 64, 20),
		usvec4(66, 0, 86, 20),
		usvec4(88, 0, 108, 20),
		usvec4(0, 22, 20, 42),
		usvec4(22, 22, 42, 42),
		usvec4(44, 22, 64, 42),
		usvec4(66, 22, 86, 42),
		usvec4(88, 22, 108, 42),
		usvec4(110, 0, 120, 10),
		usvec4(110, 12, 120, 22),
		usvec4(110, 24, 120, 34),
	};
	MainFlatUI::flImage = new Graphonichk::Texture("FlatUI.png", 13, rect);
	return true;
}

void FlatUIButtonMouseUp (const EventMouse *e) {
	FlatUIButton *flsb = (FlatUIButton*)(e->obj);
	short localx, localy;
	EventCheckButton *event;
	if (flsb->press) {
		flsb->press = false;
		localx = e->x - flsb->getGlobalX();
		localy = e->y - flsb->getGlobalY();
		if (localx>0 && localx<flsb->getWidth() && localy>0 && localy<flsb->getHeight()) {
			flsb->status = UIButton::ROLLOVER;
			flsb->texRectID = MainFlatUI::BUTTON_BOX_ROLL_OVER;
			//flsb->setRectID(MainFlatUI::BUTTON_BOX_ROLL_OVER);
			//event = new EventCheckButton( EventCheckButton::CHECK_UPDATE );
			//event->checked = true;
			//event->obj = flsb;
			//flsb->UICheckbox::callEvent(event);
			//delete event;
		}else{
			flsb->status = UIButton::NORMAL;
			flsb->texRectID = MainFlatUI::BUTTON_BOX;
			//flsb->setRectID(MainFlatUI::BUTTON_BOX);
		}
		#ifdef REDRAWN_BY_THE_ACTION
		Windows::window->renderComplete = false;
		#endif
	}
}
FlatUIButton::FlatUIButton(unsigned short w, unsigned short h) :BitmapAtlas(MainFlatUI::flImage, MainFlatUI::BUTTON_BOX) {
	this->setRect(w, h);
	this->mouseEventActive = true;
	Windows::window->events.mouse.addEventHandler(EventMouse::MOUSE_UP, FlatUIButtonMouseUp, this);
}
int FlatUIButton::callEvent(EventMouseShape* event) {
	if (true) {
		switch (event->type) {
			case EventMouseShape::MOUSE_DOWN:
				this->press = true;
				this->status = FlatUIButton::PRESS;
				this->texRectID = MainFlatUI::BUTTON_BOX_PRESSED;
				//this->setRectID(MainFlatUI::BUTTON_BOX_PRESSED);
				#ifdef REDRAWN_BY_THE_ACTION
				Windows::window->renderComplete = false;
				#endif
				return true;
			case EventMouseShape::MOUSE_ROLL_OUT:
				this->status = FlatUIButton::NORMAL;
				this->texRectID = MainFlatUI::BUTTON_BOX;
				//this->setRectID(MainFlatUI::BUTTON_BOX);
				#ifdef REDRAWN_BY_THE_ACTION
				Windows::window->renderComplete = false;
				#endif
				return true;
			case EventMouseShape::MOUSE_ROLL_OVER:
				if (this->press) {
					this->status = FlatUIButton::PRESS;
					this->texRectID = MainFlatUI::BUTTON_BOX_PRESSED;
					//this->setRectID(MainFlatUI::BUTTON_BOX_PRESSED);
				}else{
					this->status = FlatUIButton::ROLLOVER;
					this->texRectID = MainFlatUI::BUTTON_BOX_ROLL_OVER;
					//this->setRectID(MainFlatUI::BUTTON_BOX_ROLL_OVER);
				}
				#ifdef REDRAWN_BY_THE_ACTION
				Windows::window->renderComplete = false;
				#endif
				return true;
		}
	}else{
		//this->setRectID(MainFlatUI::CHECK_BOX_DISABLE);
	}
}
int FlatUIButton::renderGL330() {
	if (this->tex == nullptr) return false;
	SET_SHADER(ShaderPartRect);
	glUniform1i(ShaderPartRect::prog->rectID, this->texRectID);
	glUniform2i(ShaderPartRect::prog->offset, 4, 4);
	glBindVertexArray(this->vao);
	glBindTexture(GL_TEXTURE_2D, this->tex->GLID);
	glBindTexture(GL_TEXTURE_1D, this->tex->rectGLID);
	glDrawArrays(GL_POINTS, 0, 1);
	return true;
}
void FlatUICheckboxMouseUp (const EventMouse *e) {
	FlatUICheckbox *flsb = (FlatUICheckbox*)(e->obj);
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
				flsb->setRectID(MainFlatUI::CHECK_BOX);
				event = new EventCheckButton( EventCheckButton::CHECK_UPDATE );
				event->checked = false;
				event->obj = flsb;
				flsb->UICheckbox::callEvent(event);
				delete event;
			}else{
				flsb->checked = true;
				flsb->status = UICheckbox::CHECKED_NORMAL;
				flsb->setRectID(MainFlatUI::CHECK_BOX_CHECKED);
				event = new EventCheckButton( EventCheckButton::CHECK_UPDATE );
				event->checked = true;
				event->obj = flsb;
				flsb->UICheckbox::callEvent(event);
				delete event;
			}
		}else{
			if (flsb->checked) {
				flsb->status = UICheckbox::CHECKED_NORMAL;
				flsb->setRectID(MainFlatUI::CHECK_BOX_CHECKED);
			}else{
				flsb->status = UICheckbox::UNCHECKED_NORMAL;
				flsb->setRectID(MainFlatUI::CHECK_BOX);
			}
		}
		#ifdef REDRAWN_BY_THE_ACTION
		Windows::window->renderComplete = false;
		#endif
	}
}
FlatUICheckbox::FlatUICheckbox() :BitmapAtlas(MainFlatUI::flImage, MainFlatUI::CHECK_BOX) {
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
					this->setRectID(MainFlatUI::CHECK_BOX_CHECKED);
				}else{
					this->status = UICheckbox::UNCHECKED_PRESS;
					this->setRectID(MainFlatUI::CHECK_BOX_ROLL_OVER);
				}
				#ifdef REDRAWN_BY_THE_ACTION
					Windows::window->renderComplete = false;
					#endif
				return true;
			case EventMouseShape::MOUSE_ROLL_OUT:
				if (this->checked) {
					this->status = UICheckbox::CHECKED_NORMAL;
					this->setRectID(MainFlatUI::CHECK_BOX_CHECKED);
				}else{
					this->status = UICheckbox::UNCHECKED_NORMAL;
					this->setRectID(MainFlatUI::CHECK_BOX);
				}
				#ifdef REDRAWN_BY_THE_ACTION
					Windows::window->renderComplete = false;
					#endif
				return true;
			case EventMouseShape::MOUSE_ROLL_OVER:
				if (this->checked) {
					if (this->press) {
						this->status = UICheckbox::CHECKED_PRESS;
						this->setRectID(MainFlatUI::CHECK_BOX_CHECKED);
					}else{
						this->status = UICheckbox::CHECKED_ROLLOVER;
						this->setRectID(MainFlatUI::CHECK_BOX_CHECKED);
					}
				}else{
					if (this->press) {
						this->status = UICheckbox::UNCHECKED_PRESS;
						this->setRectID(MainFlatUI::CHECK_BOX_CHECKED);
					}else{
						this->status = UICheckbox::UNCHECKED_ROLLOVER;
						this->setRectID(MainFlatUI::CHECK_BOX_ROLL_OVER);
					}
				}
				#ifdef REDRAWN_BY_THE_ACTION
					Windows::window->renderComplete = false;
					#endif
				return true;
		}
	}else{
		this->setRectID(MainFlatUI::CHECK_BOX_DISABLE);
	}
}


void FlatUIRadioButtonMouseUp (const EventMouse *e) {
	FlatUIRadioButton *flsb = (FlatUIRadioButton*)(e->obj);
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
					flsb->setRectID(MainFlatUI::RADIO_BUTTON);
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
					flsb->setRectID(MainFlatUI::RADIO_BUTTON_CHECKED);
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
				flsb->setRectID(MainFlatUI::RADIO_BUTTON_CHECKED);
			}else{
				flsb->status = UIRadioButton::UNCHECKED_NORMAL;
				flsb->setRectID(MainFlatUI::RADIO_BUTTON);
			}
		}
		#ifdef REDRAWN_BY_THE_ACTION
		Windows::window->renderComplete = false;
		#endif
	}
}
FlatUIRadioButton::FlatUIRadioButton() :BitmapAtlas(MainFlatUI::flImage, MainFlatUI::RADIO_BUTTON) {
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
					this->setRectID(MainFlatUI::RADIO_BUTTON_CHECKED);
				}else{
					this->status = UIRadioButton::UNCHECKED_PRESS;
					this->setRectID(MainFlatUI::RADIO_BUTTON_ROLL_OVER);
				}
				#ifdef REDRAWN_BY_THE_ACTION
					Windows::window->renderComplete = false;
					#endif
				return true;
			case EventMouseShape::MOUSE_ROLL_OUT:
				if (this->checked) {
					this->status = UIRadioButton::CHECKED_NORMAL;
					this->setRectID(MainFlatUI::RADIO_BUTTON_CHECKED);
				}else{
					this->status = UIRadioButton::UNCHECKED_NORMAL;
					this->setRectID(MainFlatUI::RADIO_BUTTON);
				}
				#ifdef REDRAWN_BY_THE_ACTION
					Windows::window->renderComplete = false;
					#endif
				return true;
			case EventMouseShape::MOUSE_ROLL_OVER:
				if (this->checked) {
					if (this->press) {
						this->status = UIRadioButton::CHECKED_PRESS;
						this->setRectID(MainFlatUI::RADIO_BUTTON_CHECKED);
					}else{
						this->status = UIRadioButton::CHECKED_ROLLOVER;
						this->setRectID(MainFlatUI::RADIO_BUTTON_CHECKED);
					}
				}else{
					if (this->press) {
						this->status = UIRadioButton::UNCHECKED_PRESS;
						this->setRectID(MainFlatUI::RADIO_BUTTON_CHECKED);
					}else{
						this->status = UIRadioButton::UNCHECKED_ROLLOVER;
						this->setRectID(MainFlatUI::RADIO_BUTTON_ROLL_OVER);
					}
				}
				#ifdef REDRAWN_BY_THE_ACTION
					Windows::window->renderComplete = false;
					#endif
				return true;
		}
	}else{
		this->texRectID = MainFlatUI::RADIO_BUTTON_DISABLE;
	}
}
void FlatUIRadioButton::changeCheck() {
	if (this->checked) {
		this->status = UIRadioButton::CHECKED_NORMAL;
		this->setRectID(MainFlatUI::RADIO_BUTTON_CHECKED);
	}else{
		this->status = UIRadioButton::UNCHECKED_NORMAL;
		this->setRectID(MainFlatUI::RADIO_BUTTON);
	}
}