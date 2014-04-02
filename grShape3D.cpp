#include "grShape3D.h"

using namespace std;
using namespace Graphonichk;

//'   -00123.5432' or '  .0123'
float getFloat(const char *str, size_t *point) {
	size_t v1, v2;
	size_t it1, it2;
	size_t i = (*point);
	float num1;
	bool minus = false;
	while (str[i]!=0 && str[i]==' ') ++i;
	if (str[i]==0) {
		return NAN;
	}else if (str[i]=='-') {
		minus = true;
		++i;
		
	}else if (str[i]=='+') {
		minus = false;
		++i;
	}else{
		minus = false;
	}
	v1 = i;
	it1 = 0;
	while (str[i]!=0 && str[i]>=0x30 && str[i]<=0x39 )  {
		it1 = it1*10+ (str[i]-0x30);
		++i;
	}
	num1 = (float)it1;
	if (str[i]=='.') {
		it2 = 0;
		v2 = 1;
		++i;
		while (str[i]!=0 && str[i]>=0x30 && str[i]<=0x39 )  {
			it2 = it2*10+ (str[i]-0x30);
			v2 *= 10;
			++i;
		}
		num1 += (float)it2/v2;
	}
	(*point) = i;
	if (minus) num1*=-1; 
	return num1;
}
int getInteger(const char *str, size_t *point) {
	size_t v1;
	size_t it1;
	size_t i = (*point);
	int num1;
	bool minus = false;
	while (str[i]!=0 && str[i]==' ') ++i;
	if (str[i]==0) {
		return 0;
	}else if (str[i]=='-') {
		minus = true;
		++i;
		
	}else if (str[i]=='+') {
		minus = false;
		++i;
	}else{
		minus = false;
	}
	v1 = i;
	it1 = 0;
	while (str[i]!=0 && str[i]>=0x30 && str[i]<=0x39 )  {
		it1 = it1*10+ (str[i]-0x30);
		++i;
	}
	num1 = (int)it1;
	
	(*point) = i;
	if (minus) num1*=-1; 
	return num1;
}

Model3D::Model3D(const char* data, const char* objName) {
	size_t i = 0;
	size_t v1;
	vec2 vc2;
	vec3 vc3;
	uvec3 uvc3;
	if (objName!=nullptr) {

searchObj:
		if (data[i]!='o') {
			while (data[i]!=0 && data[i]!='\n') ++i;
			if (data[i]==0) goto endStr;
			++i;
			goto searchObj;
		}
		++i;
		while (data[i]!=0 && data[i]==' ') ++i;
		if (data[i]==0) goto endStr;
		v1 = i;
		while (objName[i-v1]!=0 && data[i]!=0 && data[i]==objName[i-v1]) ++i;
		if (data[i]==0) goto endStr;
		if (objName[i-v1]==0 && data[i]<=0x20 ) {
		}else{
			while (data[i]!=0 && data[i]!='\n') ++i;
			if (data[i]==0) goto endStr;
			++i;
			printf("'%c'", data[i]);
			puts("OK");
			goto searchObj;
		}
		puts("OK");
searchChank:
		while (data[i]!=0 && data[i]!='\n') ++i;
		if (data[i]==0) goto endStr;
		++i;
		switch (data[i]) {
			case 'v':
				++i;
				if (data[i]==' ') {
					++i;
					vc3.x = getFloat(data, &i);
					if (data[i]==0) goto endStr;
					vc3.y = getFloat(data, &i);
					if (data[i]==0) goto endStr;
					vc3.z = getFloat(data, &i);
					vertex.push_back(vc3);
					if (data[i]==0) goto endStr;
				}else if (data[i]=='t') {
					++i;
					vc2.x = getFloat(data, &i);
					if (data[i]==0) goto endStr;
					vc2.y = getFloat(data, &i);
					texCoord.push_back(vc2);
					if (data[i]==0) goto endStr;
				}else if (data[i]=='n') {
					++i;
					vc3.x = getFloat(data, &i);
					if (data[i]==0) goto endStr;
					vc3.y = getFloat(data, &i);
					if (data[i]==0) goto endStr;
					vc3.z = getFloat(data, &i);
					normal.push_back(vc3);
					if (data[i]==0) goto endStr;
				}
				break;
			case 'f':
				++i;
				uvc3.x = getInteger(data, &i)-1;
				if (data[i]==0) goto endStr;
				uvc3.y = getInteger(data, &i)-1;
				if (data[i]==0) goto endStr;
				uvc3.z = getInteger(data, &i)-1;
				polygon.push_back(uvc3);
				if (data[i]==0) goto endStr;
				break;
		}
		goto searchChank;
		return;
	}else{
		
		return;
	}
	
	endStr:
	/*for (vector<vec3>::const_iterator it=vertex.begin(), end=vertex.end(); it!=end; ++it) {
		printf("v %f %f %f\n", (*it).x, (*it).y, (*it).y);
	}*/
	printf("%i, %i", vertex.size(), polygon.size());
	return;
}
