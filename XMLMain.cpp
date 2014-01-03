#include <vector>
#include <string>

#include "XMLMain.h"
#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace grEngine;
#define SERACH_WHITE_SPACE(str, point, end) while (point<end&&(str[point]==0x20||str[point]==0x9||str[point]==0xD||str[point]==0xA)) point++;
#define SERACH_NAME_CHAR(str, point, end) while (point<end&&\
				str[point]!=' '&&\
				str[point]==':'||\
				str[point]=='_'||\
				str[point]=='-'||\
				str[point]=='.'||\
				str[point]==0xB7||\
				(str[point]>=0x30&&str[point]<=0x39)||\
				(str[point]>=0x61&&str[point]<=0x7A)||\
				(str[point]>=0x41&&str[point]<=0x5A)||\
				(str[point]>=0xC0&&str[point]<=0xD6)||\
				(str[point]>=0xD8&&str[point]<=0xF6)) point++;\
//NameChar
#define CHECK_SPACE(point, size, xml, err_i) if(point>=size) {xml->error=err_i;goto err;}
grEngine::XML::XML(string& str) { 
	printf("XML size=%i :\n%s\n", str.size(), str.c_str());
	
	unsigned int point=0, oot1, oor1, oor2, strSize=str.size();
	Object *obj = NULL;
	Object *parent = NULL;
	Prototupe *proto = NULL;
	goto objOut;
	
	objOver : // <editor-fold defaultstate="collapsed" desc="objOver">
	parent = obj;
	if (str[point]=='?') {
	}else if (str[point]=='!') {
	}else if (	str[point]==':' ||
				str[point]=='_' ||
				(str[point]>=0x61&&str[point]<=0x7A) ||
				(str[point]>=0x41&&str[point]<=0x5A) ||
				(str[point]>=0xC0&&str[point]<=0xD6) ||
				(str[point]>=0xD8&&str[point]<=0xF6) ) {
		oor1 = point;
		point++;
		//"-" | "." | [0-9] | #xB7 | [#x0300-#x036F] | [#x203F-#x2040]
		SERACH_NAME_CHAR(str, point, strSize);
		CHECK_SPACE(point, strSize, this, XML_NO_CLOSE_SYM);
		printf("_%s_", (str.substr(oor1, point-oor1)).c_str() );
		oor2 = this->getProto( str.substr(oor1, point-oor1) );//oor1 free
		proto = this->prototups[oor2];
		obj = new Object(proto, obj, oor2);
		//attribute
		printf("_%c_", str[point] );
		SERACH_WHITE_SPACE(str, point, strSize);
		CHECK_SPACE(point, strSize, this, XML_NO_CLOSE_SYM);
		
		printf("_%c_", str[point] );
		goto err;		
	}
	
	
	// </editor-fold>
	objOut :// <editor-fold defaultstate="collapsed" desc="objOut">
	oot1 = str.find('<', point);
	if (obj != NULL) {
		if (oot1 < 0) {
			this->error = XML_NO_CLOSE_TAG;
			goto err;
		}else{
			obj->str += "_ADD_";
			point = oot1+1;
			goto objOver;
		}
	}else{
		point = oot1+1;
		goto objOver;
	}
	// </editor-fold>
	err :;
}
short grEngine::XML::getProto(string name) {
	Prototupe* proto;
	int size=this->prototups.size(), i=0;
	for (; i<size; i++) {
		if (this->prototups[i]->name == name ) return i;
	}
	i = size;
	proto = new Prototupe(name);
	this->prototups.push_back(proto);
	return i;
}


grEngine::Prototupe::Prototupe(const string& name) {
	this->name = name;
	this->data.clear();
}
grEngine::Prototupe::~Prototupe() {
}


grEngine::Object::Object(Prototupe* proto, Object* parent, short nameID) {
	this->nameID = nameID;
	this->parent = parent;
	this->child.clear();
	prop.resize(proto->data.size());
	
}
grEngine::Object::~Object() {
	this->parent = NULL;
}