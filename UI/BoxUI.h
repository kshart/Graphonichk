#ifndef BOXUI_H
#define	BOXUI_H

#include "../grMain.h"
#include "UIMain.h"


using namespace Graphonichk;

namespace BoxUI {
	class BoxUIButton;
	class BoxUICheckbox;
	class BoxUIRadioButton;
	
	class MainBoxUI {
	public:
		enum COLOR{
			CHECK_BOX_CHECKED			=0,
			CHECK_BOX					=1,
			CHECK_BOX_ROLL_OVER			=2,
			CHECK_BOX_DISABLE			=3,
			CHECK_BOX_DISABLE_CHECKED	=4,
			RADIO_BUTTON_CHECKED		=5,
			RADIO_BUTTON				=6,
			RADIO_BUTTON_ROLL_OVER		=7,
			RADIO_BUTTON_DISABLE		=8,
			RADIO_BUTTON_DISABLE_CHECKED=9,
			BUTTON_BOX					=10,
			BUTTON_BOX_ROLL_OVER		=11,
			BUTTON_BOX_PRESSED			=12
		};
		static ubvec4 color[13];
		static Graphonichk::Texture *flImage;
	};
	class BoxUIButton :public Graphonichk::FRect, public Graphonichk::UIButton {
	public:
		BoxUIButton(unsigned short w=140, unsigned short h=40);
		
		int callEvent(EventMouseShape* event) override;
	};
	class BoxUICheckbox :public Graphonichk::FRect, public Graphonichk::UICheckbox {
	public:
		BoxUICheckbox();
		
		int callEvent(EventMouseShape* event) override;
	};
	class BoxUIRadioButton :public Graphonichk::FRect, public Graphonichk::UIRadioButton  {
	public:
		BoxUIRadioButton();
		
		int callEvent(EventMouseShape* event) override;
		void changeCheck();
	};
}

#endif

