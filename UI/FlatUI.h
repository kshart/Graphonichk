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
		static int init();
		static Graphonichk::Texture *flRB, *flRBDisable, *flRBChk, *flRBRollOver,
				*flCB, *flCBDisable, *flCBChk, *flCBRollOver;
	};
	class FlatUICheckbox :public Graphonichk::Bitmap, public Graphonichk::UICheckbox {
	public:
		FlatUICheckbox();
		
		int callEvent(EventMouseShape* event);
	};
	class FlatUIRadioButton :public Graphonichk::UIRadioButton, public Graphonichk::Bitmap  {
	public:
		FlatUIRadioButton();
		
		int callEvent(EventMouseShape* event);
		void changeCheck();
	};
}

#endif	/* FLATUI_H */

