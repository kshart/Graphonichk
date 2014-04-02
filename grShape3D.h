#ifndef GRSHAPE3D_H
#define	GRSHAPE3D_H

#include "grBaseTypes.h"



using namespace std;
namespace Graphonichk {
	class Model3D {
	public:
		Model3D(const char *data, const char *objName);
		
		vector<vec3> vertex;
		vector<vec2> texCoord;
		vector<vec3> normal;
		vector<uvec3> polygon;
		
		GLuint vao, vbo, vboIndex;
	};
	
}




#endif	/* GRSHAPE3D_H */

