/* 
 * File:   UIWorkspace.h
 * Author: Артём Каширин
 *
 * Created on 8 Август 2013 г., 1:45
 */

#ifndef UIWORKSPACE_H
#define	UIWORKSPACE_H

#include <vector>
#include "../grBaseTypes.h"
#ifdef	__cplusplus
extern "C" {
#endif

using namespace std;

namespace grEngine {
	class UIWorkspace;
	class UIUnit;
	class UIObject;
	
	class UIObject {
	  private:
	  public:
		enum SEPARATION {
			SEPARATION_WIDTH=true,
			SEPARATION_HEIGHT=false
		};
		//UIObject(bool separation = false);
		UIObject(unsigned short w, unsigned short h, UIObject *parent=NULL, UIUnit *unit=NULL);
		
		void resize(unsigned short w, unsigned short h);
		void addChild(UIUnit* unit, unsigned short size, unsigned short pos=SHRT_MAX);
		void getChild(vector<UIUnit*> *arr);
		
		UIObject *parent;
		UIUnit *unit;
		vector<UIObject*> child;
		bool separationStyle;
		short x, y;
		unsigned short width, height, rectWidth, rectHeight, position;
	};
	class UIUnit :public Shape {
	  public:
		UIUnit();
		void trace();
	};
	class UIWorkspace :public Directory {
	  public:
		UIWorkspace(unsigned short width, unsigned short height);
		UIObject *root;
		unsigned short width, height;
		void getUIUnits();
	};
	
	
	
	
	
	class UIButton;
	class UICheckbox;
		//Unchecked
		//Checked
		//Disabled unchecked
		//Disabled checked
	class UIRadioButton;
		//Radio is off
		//Radio is on
		//Disabled radio is off
		//Disabled radio is on
	//class UIWorkspace;
	
	/*enum :char {
		UI_SEPARATION_OFF,
		UI_SEPARATION_LEFT_WIDTH,
		UI_SEPARATION_RIGHT_WIDTH,
		UI_SEPARATION_TOP_HEIGHT,
		UI_SEPARATION_BOTTOM_HEIGHT
	};
	
	class UIMain : public Directory {
	public:
		UIWorkspace *workspace;
		//win
		int type;
		//0x80000000 ins theme;
		
		
		unsigned short barHeight, buttonsHeight, border;
		///win
		short x, y;
		unsigned short width, height;
		
		UIMain(short, short, unsigned short, unsigned short);
	};
	class UIObject : public Directory {
		unsigned short width, height;
		int resize(unsigned short, unsigned short);
		int open();
		int close();
	};
	class UIWorkspace {
	  public:
		UIWorkspace *parent;
		vector<UIWorkspace> child;
		vector<unsigned short> childSepY;
		UIObject *obj;
		char sepStyle;
		short x, y;
		unsigned short width, height;
		
		UIWorkspace(UIObject* obj, char sepStyle=UI_SEPARATION_OFF, unsigned short childSize=0, unsigned short width=0, unsigned short height=0);
		void addChild(UIWorkspace*, unsigned short);
		int getChild(vector<UIObject>*);
		void trace();
	};
	
	
	class UIButton : public Bitmap {
	  public:
		enum STATE:char {
			asd
		};
		UIButton(short x, short y, unsigned short w, unsigned short h, void *click() );
		STATE state;
		void *click();
	};*/
	class ScrollBarH;
	class ScrollBarW;
	
	class ScrollBarH :public Directory {
	  public:
		ScrollBarH(unsigned short w, unsigned short h);
		void mouseDown (const EventMouse *e);
		void mouseUp (const EventMouse *e);
		void mouseMove (const EventMouse *e);
		
		FRect *pad, *bar;
		float position;
		bool scrollStarted;
	};
	class ScrollBarW :public Directory {
	  public:
		ScrollBarW(unsigned short w, unsigned short h);
		void mouseDown (const EventMouse *e);
		void mouseUp (const EventMouse *e);
		void mouseMove (const EventMouse *e);
		
		FRect *pad, *bar;
		float position;
		bool scrollStarted;
	};
	
	class Button :public Shape {
	public:
		enum STATUS {
			PRESS, NORMAL, ROLLOVER
		};
		Button(unsigned short w, unsigned short h);
		int renderGLComptAll();
		int renderGL400();
		int renderGL330();
		int renderGL210();
		Shape *shapePressed, *shapeNormal, *shapeRollOver;
		STATUS status;
		bool press;
	};
}


#ifdef	__cplusplus
}
#endif

#endif	/* UIWORKSPACE_H */

