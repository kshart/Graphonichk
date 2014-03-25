#ifndef FLATUI_H
#define	FLATUI_H

#include "../grBaseTypes.h"
#include "UIMain.h"

namespace FlatUI {
	class FlatUIButton;
	class FlatUICheckbox;
	class FlatUIRadioButton;
	
	class MainFlatUI {
	public:
		static int init();
		static Graphonichk::Texture *flRB, *flRBChk, *flRBRollOver,
				*flCB, *flCBChk, *flCBRollOver;
	};
	class FlatUICheckbox :public Graphonichk::UICheckbox {
	public:
		FlatUICheckbox();
	};
	class FlatUIRadioButton :public Graphonichk::UIRadioButton {
	public:
		FlatUIRadioButton(Graphonichk::UIRadioButtonGroup *gr);
	};
}

#endif	/* FLATUI_H */

