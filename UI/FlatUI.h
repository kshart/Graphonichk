#ifndef FLATUI_H
#define	FLATUI_H

#include "../grMain.h"
#include "UIMain.h"


using namespace Graphonichk;

namespace FlatUI {
	class FlatUIButton;
	class FlatUICheckbox;
	class FlatUIRadioButton;
	
	class MainFlatUI {
	public:
		enum {
			CHECK_BOX_CHECKED=0,
			CHECK_BOX=1,
			CHECK_BOX_ROLL_OVER=2,
			CHECK_BOX_DISABLE=3,
			RADIO_BUTTON_CHECKED=4,
			RADIO_BUTTON=5,
			RADIO_BUTTON_ROLL_OVER=6,
			RADIO_BUTTON_DISABLE=7
		};
		static int init();
		static Graphonichk::Texture *flImage;
	};
	class FlatUICheckbox :public Graphonichk::BitmapAtlas, public Graphonichk::UICheckbox {
	public:
		FlatUICheckbox();
		
		int callEvent(EventMouseShape* event);
	};
	class FlatUIRadioButton :public Graphonichk::UIRadioButton, public Graphonichk::BitmapAtlas  {
	public:
		FlatUIRadioButton();
		
		int callEvent(EventMouseShape* event);
		void changeCheck();
	};
}

#endif	/* FLATUI_H */

