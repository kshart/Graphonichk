/* 
 * File:   UIWorkspace.h
 * Author: РђСЂС‚С‘Рј РљР°С€РёСЂРёРЅ
 *
 * Created on 8 РђРІРіСѓСЃС‚ 2013 Рі., 1:45
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
	class UIUnitDirectory;
	
	class UIUnit {
	public:
		UIUnit(unsigned short w, unsigned short h, Shape *sh=NULL);
		virtual void trace(unsigned int tab=0);
		virtual void updateGlobalPosition();
		virtual void resize(unsigned short w, unsigned short h);
		virtual void drag(unsigned short x, unsigned short y);
		virtual Directory* getRect();
		virtual void getRect(Directory *dir);
		
		Shape *shape;
		UIUnitDirectory *parent;
		unsigned short width, height, scaledWidth, scaledHeight, x, y, globalx, globaly;
	};
	class UIUnitDirectory :public UIUnit {
	public:
		enum SEPARATION {
			SEPARATION_WIDTH=true,
			SEPARATION_HEIGHT=false
		};
		UIUnitDirectory(unsigned short w, unsigned short h, Shape *sh=NULL);
		
		//void resize(unsigned short w, unsigned short h);
		void addChild(UIUnit* unit, unsigned short pos=SHRT_MAX);
		void drag(unsigned short x, unsigned short y);
		void resize(unsigned short w, unsigned short h);
		Directory* getRect();
		void getRect(Directory *dir);
		void trace(unsigned int tab=0);
		
		vector<UIUnit*> child;
		
		bool separationStyle;
	};
	class UIWorkspace :public Directory {
	  public:
		UIWorkspace(unsigned short width, unsigned short height);
		UIUnitDirectory *root;
		unsigned short width, height;
		void getUIUnits();
	};
	
	
	class UIScrollBarH;
	class UIScrollBarW;
	class UIButton;
	class UICheckbox;
	class UIRadioButton;
	class UIRadioButtonGroup;
	
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
		int callEvent(EventMouseShape *e);
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

	
	class UITable;
	class UITable :public Directory{
	public:
		UITable(unsigned short w, unsigned short h);
	};
}


#ifdef	__cplusplus
}
#endif

#endif	/* UIWORKSPACE_H */

