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
			CHECK_BOX_DISABLE_CHECKED=4,
			RADIO_BUTTON_CHECKED=5,
			RADIO_BUTTON=6,
			RADIO_BUTTON_ROLL_OVER=7,
			RADIO_BUTTON_DISABLE=8,
			RADIO_BUTTON_DISABLE_CHECKED=9,
			BUTTON_BOX=10,
			BUTTON_BOX_ROLL_OVER=11,
			BUTTON_BOX_PRESSED=12
		};
		static int init();
		static Graphonichk::Texture *flImage;
	};
	class FlatUIButton :public Graphonichk::BitmapAtlas, public Graphonichk::UIButton {
	public:
		FlatUIButton(unsigned short w=140, unsigned short h=40);
		
		int callEvent(EventMouseShape* event) override;
		int renderGL330() override;
		int renderGL100() override;
	};
	class FlatUICheckbox :public Graphonichk::BitmapAtlas, public Graphonichk::UICheckbox {
	public:
		FlatUICheckbox();
		
		int callEvent(EventMouseShape* event) override;
	};
	class FlatUIRadioButton :public Graphonichk::UIRadioButton, public Graphonichk::BitmapAtlas  {
	public:
		FlatUIRadioButton();
		
		int callEvent(EventMouseShape* event) override;
		void changeCheck();
	};
}

#endif	/* FLATUI_H */

