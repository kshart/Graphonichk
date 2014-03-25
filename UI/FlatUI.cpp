#include "FlatUI.h"
using namespace std;
using namespace FlatUI;

Graphonichk::Texture *MainFlatUI::flRB = NULL;
Graphonichk::Texture *MainFlatUI::flRBChk = NULL;
Graphonichk::Texture *MainFlatUI::flRBRollOver = NULL;
Graphonichk::Texture *MainFlatUI::flCB = NULL;
Graphonichk::Texture *MainFlatUI::flCBChk = NULL;
Graphonichk::Texture *MainFlatUI::flCBRollOver = NULL;

int MainFlatUI::init() {
	MainFlatUI::flRB = new Graphonichk::Texture("FlatUIRadioButton.png");
	MainFlatUI::flRBChk = new Graphonichk::Texture("FlatUIRadioButtonCheck.png");
	MainFlatUI::flRBRollOver = new Graphonichk::Texture("FlatUIRadioButtonRollOver.png");
	MainFlatUI::flCB = new Graphonichk::Texture("FlatUICheckBox.png");
	MainFlatUI::flCBChk = new Graphonichk::Texture("FlatUICheckBoxCheck.png");
	MainFlatUI::flCBRollOver = new Graphonichk::Texture("FlatUICheckBoxRollOver.png");
}
FlatUICheckbox::FlatUICheckbox() : UICheckbox(123321, 20, 20, 
		new Graphonichk::Bitmap(MainFlatUI::flCBRollOver),
		new Graphonichk::Bitmap(MainFlatUI::flCB),
		new Graphonichk::Bitmap(MainFlatUI::flCBRollOver),
		new Graphonichk::Bitmap(MainFlatUI::flCB),
		new Graphonichk::Bitmap(MainFlatUI::flCBChk),
		new Graphonichk::Bitmap(MainFlatUI::flCBChk),
		new Graphonichk::Bitmap(MainFlatUI::flCBChk),
		new Graphonichk::Bitmap(MainFlatUI::flCBChk)
) {
	
}
FlatUIRadioButton::FlatUIRadioButton(Graphonichk::UIRadioButtonGroup *gr) : UIRadioButton(30, 30, gr) {
	
}
