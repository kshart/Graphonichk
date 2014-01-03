/* 
 * File:   XMLMain.h
 * Author: Артём Каширин
 *
 * Created on 10 Август 2013 г., 2:03
 */

#ifndef XMLMAIN_H
#define	XMLMAIN_H

#include <vector>
#include <string>

#ifdef	__cplusplus
extern "C" {
#endif

using namespace std;
namespace grEngine {
	class XML;
	class Prototupe;
	class Object;
	
	enum : char {
		XML_STR_LOKED,
		XML_STR_UNLOKED,
	};
	enum {
		XML_NO_CLOSE_TAG,
		XML_NO_CLOSE_SYM
	};
	class XML {
	  public:
		int error;
		vector<Prototupe*> prototups;
		vector<Object*> child;
		
		XML(string&);
		XML(string, char);
		short getProto(string);
		int setProp(string& obj, string& , string&);
		static char *firstSymName, symName;
	};
	class Object {
	  public:
		short nameID;
		Object* parent;
		vector<string> prop;
		vector<Object*> child;
		string str;
		Object(Prototupe*, Object*, short);
		~Object();
	};
	class Prototupe {
	  public:
		vector<string> data;
		string name;
		
		Prototupe(const string&);
		~Prototupe();
	};
	
}

#ifdef	__cplusplus
}
#endif

#endif	/* XMLMAIN_H */

/*
 <xml>
	<obj/>
 </xml>
 
 
 */