#ifndef UIWORKSPACE_H
#define	UIWORKSPACE_H

#include <vector>
#include "../grBaseTypes.h"

using namespace std;

namespace Graphonichk {
	class UIWorkspace;
	class UIUnit;
	class UIUnitDirectory;
	
	class UIUnit {
	public:
		UIUnit(unsigned short w, unsigned short h, ShapeRect *sh=NULL);
		virtual void trace(unsigned int tab=0);
		virtual void updateGlobalPosition();
		virtual void resize(unsigned short w, unsigned short h);
		virtual void drag(unsigned short x, unsigned short y);
		virtual ShapeGroupRect* getRect();
		virtual void getRect(ShapeGroupRect *dir);
		
		ShapeRect *shape;
		UIUnitDirectory *parent;
		unsigned short width, height, scaledWidth, scaledHeight, x, y, globalx, globaly;
	};
	class UIUnitDirectory :public UIUnit {
	public:
		enum SEPARATION {
			SEPARATION_WIDTH=true,
			SEPARATION_HEIGHT=false
		};
		UIUnitDirectory(unsigned short w, unsigned short h, ShapeRect *sh=NULL);
		
		//void resize(unsigned short w, unsigned short h);
		bool addChild(UIUnit* unit, unsigned short pos=SHRT_MAX);
		void drag(unsigned short x, unsigned short y);
		void resize(unsigned short w, unsigned short h);
		ShapeGroupRect* getRect();
		void getRect(ShapeGroupRect *dir);
		void trace(unsigned int tab=0);
		
		vector<UIUnit*> child;
		
		bool separationStyle;
	};
	class UIWorkspace :public ShapeGroupRect {
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
	
	class UIScrollBarH :public ShapeGroupRect {
	public:
		enum{CRC32=0xE8BC50C8};
		UIScrollBarH(unsigned short w, unsigned short h);
		void mouseDown (const EventMouse *e);
		void mouseUp (const EventMouse *e);
		void mouseMove (const EventMouse *e);
		
		void (*chengePosition)(float n, void* obj);
		void* obj;
		
		
		FRect *pad, *bar;
		float position;
		bool scrollStarted;
	};
	class UIScrollBarW :public ShapeGroupRect {
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
	
	class EventCheckButton :public Event {
	  public:
		enum :int{
			CHECK_UPDATE
		};
		EventCheckButton(int type);
		bool checked;
	};
	class UIButton {
	protected:
		UIButton();
	public:
		enum STATUS {
			PRESS, NORMAL, ROLLOVER
		};
		int callEvent(EventMouseShape *e);
		STATUS status;
		bool press;
	};
	class UICheckbox :public EventDispatcher<EventCheckButton> {
	protected:
		UICheckbox();
	public:
		enum STATUS {
			UNCHECKED_PRESS, UNCHECKED_NORMAL, UNCHECKED_ROLLOVER, UNCHECKED_DISABLE,
			CHECKED_PRESS, CHECKED_NORMAL, CHECKED_ROLLOVER, CHECKED_DISABLE
		};
		STATUS status;
		bool press;
		bool checked;
	};
	
	class UIRadioButtonGroup {
	private:
		vector<UIRadioButton*> _buttons;
	public:
		enum {CRC32=0xE7E01EB1};
		UIRadioButtonGroup();
		bool addRadioButton(UIRadioButton *rb);
		bool setActiveButton(UIRadioButton *rb);
		bool active;
		UIRadioButton *checked;
	};
	class UIRadioButton :public EventDispatcher<EventCheckButton> {
	protected:
		UIRadioButton();
	public:
		enum STATUS {
			UNCHECKED_PRESS, UNCHECKED_NORMAL, UNCHECKED_ROLLOVER, UNCHECKED_DISABLE,
			CHECKED_PRESS, CHECKED_NORMAL, CHECKED_ROLLOVER, CHECKED_DISABLE
		};
		UIRadioButtonGroup *group;
		STATUS status;
		bool press;
		bool checked;
		virtual void changeCheck();
	};
	
	
	
	class UITable;
	class UITableRow;
	class UITable :public ShapeGroupRect {
	private:
		unsigned short startRowPos, stopRowPos;
	public:
		UITable(unsigned short w, unsigned short h, vector<unsigned short> *columns=NULL);
		void addRow(UITableRow* row);
		void updateRows();
		void setPos(float n);
		
		vector<unsigned short> columnsWidth;
		
		ShapeRect *background;
		unsigned short allRowsVisibleSize;
		vector<UITableRow*> rows;
		float viewPosition;
		unsigned short rowsHeight;
	};
	class UITableRow {
	public:
		UITableRow(UITable *table);
		void addCol(ShapeRect* sh);
		unsigned short columnsUpdate(unsigned short x, unsigned short y);//return rows count
		
		UITable *table;
		vector<ShapeRect*> columns;
	};
	
	
	
	
	class UITableDirectory;
	class UITableDirectoryRow;
	class UITableDirectory :public ShapeGroupRect {
	private:
		unsigned short startRowPos, stopRowPos;
	public:
		UITableDirectory(unsigned short w, unsigned short h, vector<unsigned short> *columnsWidth=NULL);
		void addRow(UITableDirectoryRow* row);
		void updateRows();
		void setPos(float n);
		
		vector<unsigned short> columnsWidth;
		
		ShapeRect *background;
		unsigned short allRowsVisibleSize;
		vector<UITableDirectoryRow*> rows;
		float viewPosition;
		unsigned short rowsHeight;
	};
	class UITableDirectoryRow {
	private:
		void updateParentDepth(unsigned short i);
	public:
		UITableDirectoryRow(UITableDirectory *table);
		void addCol(ShapeRect *sh);
		void addDirectoryRow(UITableDirectoryRow* dir);
		int render(int y);
		
		void hide();
		void show();
		
		UITableDirectoryRow *parent;
		UITableDirectory *table;
		unsigned short allChildVisibleCount, allChildCount, parentDepth;
		
		vector<UITableDirectoryRow*> child;
		vector<ShapeRect*> columns;
	};
}


#endif	/* UIWORKSPACE_H */

