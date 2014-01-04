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
	
	
	class UIScrollBarH;
	class UIScrollBarW;
	class UIButton;
	class UICheckbox;
		//Unchecked
		//Checked
		//Disabled unchecked
		//Disabled checked
	class UIRadioButton;
	class UIRadioButtonGroup;
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
	
	class UIScrollBarH :public Directory {
	public:
		enum{CRC32=0xE8BC50C8};
		UIScrollBarH(unsigned short w, unsigned short h);
		void mouseDown (const EventMouse *e);
		void mouseUp (const EventMouse *e);
		void mouseMove (const EventMouse *e);
		
		FRect *pad, *bar;
		float position;
		bool scrollStarted;
	};
	class UIScrollBarW :public Directory {
	public:
		enum{CRC32=0x65B45D3D};
		UIScrollBarW(unsigned short w, unsigned short h);
		void mouseDown (const EventMouse *e);
		void mouseUp (const EventMouse *e);
		void mouseMove (const EventMouse *e);
		
		FRect *pad, *bar;
		float position;
		bool scrollStarted;
	};
	
	class UIButton :public Shape {
	private:
		UIButton(int crc32, unsigned short w, unsigned short h, Shape *shPressed, Shape *shNormal, Shape *shRollOver);
	public:
		enum {CRC32=0x251F1AC9};
		enum STATUS {
			PRESS, NORMAL, ROLLOVER
		};
		UIButton(unsigned short w, unsigned short h);
		int renderGLComptAll();
		int renderGL400();
		int renderGL330();
		int renderGL210();
		void updateGlobalPosition();
		Shape *shapePressed, *shapeNormal, *shapeRollOver;
		STATUS status;
		bool press;
	};
	class UICheckbox :public Shape {
	private:
		UICheckbox(int crc32, unsigned short w, unsigned short h, Shape *shUnchkPressed, Shape *shUnchkNormal, Shape *shUnchkRollOver, Shape *shUnchkDisable, Shape *shChkPressed, Shape *shChkNormal, Shape *shChkRollOver, Shape *shChkDisable);
	public:
		enum {CRC32=0x4CA925F4};
		enum STATUS {
			UNCHECKED_PRESS, UNCHECKED_NORMAL, UNCHECKED_ROLLOVER, UNCHECKED_DISABLE,
			CHECKED_PRESS, CHECKED_NORMAL, CHECKED_ROLLOVER, CHECKED_DISABLE
		};
		UICheckbox(unsigned short w, unsigned short h);
		void updateGlobalPosition();
		int renderGLComptAll();
		int renderGL400();
		int renderGL330();
		int renderGL210();
		Shape *shUnchkPressed, *shUnchkNormal, *shUnchkRollOver, *shUnchkDisable, *shChkPressed, *shChkNormal, *shChkRollOver, *shChkDisable;
		STATUS status;
		bool press;
		bool checked;
	};
	
	class UIRadioButtonGroup :public Directory {
	private:
		//void addChild(Shape*);
	public:
		enum {CRC32=0xE7E01EB1};
		UIRadioButtonGroup();
		UIRadioButton *addRadioButton();
		bool setActiveButton(UIRadioButton *);
		bool active;
		UIRadioButton *checked;
	};
	class UIRadioButton :public Shape {
	private:
	public:
		enum {CRC32=0x97511BB6};
		enum STATUS {
			UNCHECKED_PRESS, UNCHECKED_NORMAL, UNCHECKED_ROLLOVER, UNCHECKED_DISABLE,
			CHECKED_PRESS, CHECKED_NORMAL, CHECKED_ROLLOVER, CHECKED_DISABLE
		};
		UIRadioButton(unsigned short w, unsigned short h, UIRadioButtonGroup *gr);
		void updateGlobalPosition();
		int renderGLComptAll();
		int renderGL400();
		int renderGL330();
		int renderGL210();
		UIRadioButtonGroup *group;
		Shape *shUnchkPressed, *shUnchkNormal, *shUnchkRollOver, *shUnchkDisable, *shChkPressed, *shChkNormal, *shChkRollOver, *shChkDisable;
		STATUS status;
		bool press;
		bool checked;
	};
}


#ifdef	__cplusplus
}
#endif

#endif	/* UIWORKSPACE_H */

