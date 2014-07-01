#ifndef GRSHAPE3D_H
#define	GRSHAPE3D_H

#include "grMain.h"



using namespace std;
namespace Graphonichk {
	class Model3D {
	public:
		Model3D(const char *data, const char *objName);
		
		vector<vec3> vertex;
		vector<vec2> texCoord;
		vector<vec3> normal;
		vector<uivec3> polygon;
		
		GLuint vao, vbo, vboIndex;
	};
	
	
	class Shape3DBox :public ShapeBasic {
	private:
	protected:
		Shape3DBox(int crc32) :ShapeBasic(crc32) {};
		float x, y, z,
			xScale, yScale, zScale,
			xRotate, yRotate, zRotate;
	public:
		enum {CRC32=0x6F5DB670};
		Shape3DBox() :ShapeBasic(Shape3DBox::CRC32) {};
	};
	
	
	class Shape3DBoxGroup :public Shape3DBox {
	public:
		enum {CRC32=0xBAB1253A};
		Shape3DBoxGroup() :Shape3DBox(Shape3DBoxGroup::CRC32) {};
		vector<Shape3DBox*> child;
	};
	
	class Scene3D :public ShapeRect {
	public:
		Scene3D();
		int renderGL100() override;
		int renderGL330() override;
		Matrix3D viewMatrix;
		Matrix3D viewPosMatrix;
		Shape3DBoxGroup root;
	};
}




#endif	/* GRSHAPE3D_H */

