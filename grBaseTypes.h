//add gcc -mno-ms-bitfields
//add make -lxml2 -ljpeg -lpng16 -lfreetype -ldinput -ldxguid -lgdi32 -glew32s -lopengl32 
//define WIN32 GLEW_STATIC
#ifndef GRBASETYPES_H
#define	GRBASETYPES_H

#include <pthread.h>
#if defined(WIN32)
	#include <GL/glew.h>
	#include <dinput.h>
	#include <windows.h>
	#include <windowsx.h>

	//#include <GL/wglext.h>
	//#include <GL/glext.h>
	//#define CRITICAL_SECTION 
	//#define CRITICAL_SECTION_INIT(cs) InitializeCriticalSection(&cs);
	//#define CRITICAL_SECTION_INTER(cs) EnterCriticalSection(&cs);
	//#define CRITICAL_SECTION_LEAVE(cs) LeaveCriticalSection(&cs);
	//#define MUTEX HANDLE
	//#define THREAD_H HANDLE
	//#define THREAD DWORD WINAPI
	//#define THREAD_START_H(handle, thread, data) handle = CreateThread(NULL, 0, thread, data, 0, 0);
	//#define THREAD_START(thread, data) CreateThread(NULL, 0, thread, data, 0, 0);
	///#define THREAD_SUSPEND(thread) SuspendThread(thread);
	//#define THREAD_CLOSE(thread) CloseHandle(thread);

#elif defined(X11)
	#include <GL/glew.h>
	namespace X11lib {
		#include <X11/X.h>
		#include <X11/Xlib.h>
		#include <GL/glx.h>
	}

	/*#define CRITICAL_SECTION pthread_mutex_t
	#define CRITICAL_SECTION_INIT(cs) cs = PTHREAD_MUTEX_INITIALIZER;
	#define CRITICAL_SECTION_INTER(cs) pthread_mutex_lock(&cs);
	#define CRITICAL_SECTION_LEAVE(cs) pthread_mutex_unlock(&cs);
	
	typedef pthread_t THREAD_H;
	typedef void* THREAD;
	
	#define THREAD_START(handle, thread, data) pthread_create(&handle, NULL, thread, data)
	#define THREAD_SUSPEND(thread) SuspendThread(thread);
	#define THREAD_CLOSE(thread) pthread_cancel(thread);
	
	typedef pthread_mutex_t MUTEX;*/

#endif

#define FRAME_PER_SECOND 60
#define TIME_IN_FRAME_MS (float)1/FRAME_PER_SECOND
#define M_PI 3.14159265358979323846

using namespace std;
extern FILE *iovir;
extern FILE *ioshp;

// <editor-fold defaultstate="collapsed" desc="#define IMG_TYPE">
#define GR_IMG_BMP "bmp"//0x4D42
#define GR_IMG_TGA "tga"
enum {
	GR_IMG_I_BMP,
	GR_IMG_I_TGA
};
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="#define TGA">
#define TGA_TYPE_NO 0
#define TGA_TYPE_PALETTE 1
#define TGA_TYPE_TC 2
#define TGA_TYPE_MONOCHROME 3
#define TGA_TYPE_RLE_PALETTE 9
#define TGA_TYPE_RLE_TC 10
#define TGA_TYPE_RLE_MONOCHROME 11
#define TGA_DEP_8 8
#define TGA_DEP_16 16
#define TGA_DEP_24 24
#define TGA_DEP_32 32
#define TGA_TRANSFER_X 0x10
#define TGA_TRANSFER_Y 0x20
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="#define LIB">
#define LIB_SIGN 0x5247
// </editor-fold>

typedef unsigned int uint;
namespace Graphonichk {
	class System;
	class Windows;
	class GLShader;
	class Image;
	class Texture;
	class EventKeyboard;
	class EventMouse;
	class EventWindow;
	class TextField;
	class Font;
	class TextFormat;
	
	class Rect {
	  public:
		short x, y;
		unsigned short width, height;
	};
	
	template<typename Type> class _vec2 {
	public:
		union {
			Type x, r, s;
		};
		union {
			Type y, g, t;
		};
		_vec2() noexcept {};
		_vec2(Type x, Type y) noexcept :x(x), y(y) {};
		const bool operator==(const _vec2 &arg) noexcept {
			return (this->x==arg.x) && (this->y==arg.y);
		}
		void operator+=(const _vec2& right) noexcept {
			this->x += right.x;
			this->y += right.y;
		}
		void operator+=(const Type right) noexcept {
			this->x += right;
			this->y += right;
		}
		void operator*=(const _vec2& right) noexcept {
			this->x *= right.x;
			this->y *= right.y;
		}
		void operator*=(const Type right) noexcept {
			this->x *= right;
			this->y *= right;
		}
		void operator-=(const _vec2& right) noexcept {
			this->x -= right.x;
			this->y -= right.y;
		}
		void operator-=(const Type right) noexcept {
			this->x -= right;
			this->y -= right;
		}
		void operator/=(const _vec2& right) noexcept {
			this->x /= right.x;
			this->y /= right.y;
		}
		void operator/=(const Type right) noexcept {
			this->x /= right;
			this->y /= right;
		}
		const friend _vec2 operator+(const _vec2 &a, const _vec2 &b) noexcept {
			return _vec2(a.x+b.x, a.y+b.y);
		}
		const friend _vec2 operator+(const _vec2 &a, const Type b) noexcept {
			return _vec2(a.x+b, a.y+b);
		}
		const friend _vec2 operator-(const _vec2 &a, const _vec2 &b) noexcept {
			return _vec2(a.x-b.x, a.y-b.y);
		}
		const friend _vec2 operator-(const _vec2 &a, const Type b) noexcept {
			return _vec2(a.x-b, a.y-b);
		}
		const friend _vec2 operator*(const _vec2 &a, const _vec2 &b) noexcept {
			return _vec2(a.x*b.x, a.y*b.y);
		}
		const friend _vec2 operator*(const _vec2 &a, const Type b) noexcept {
			return _vec2(a.x*b, a.y*b);
		}
		const friend _vec2 operator/(const _vec2 &a, const _vec2 &b) noexcept {
			return _vec2(a.x/b.x, a.y/b.y);
		}
		const friend _vec2 operator/(const _vec2 &a, const Type b) noexcept {
			return _vec2(a.x/b, a.y/b);
		}
	};
	template<typename Type> class _vec3 {
	public:
		union {
			Type x, r, s;
		};
		union {
			Type y, g, t;
		};
		union {
			Type z, b, p;
		};
		_vec3() noexcept {};
		_vec3(Type x, Type y, Type z) noexcept :x(x), y(y), z(z) {};
		const bool operator==(const _vec3 &arg) noexcept {
			return (this->x==arg.x) && (this->y==arg.y) && (this->z==arg.z);
		}
		void operator+=(const _vec3& right) noexcept {
			this->x += right.x;
			this->y += right.y;
			this->z += right.z;
		}
		void operator+=(const Type right) noexcept {
			this->x += right;
			this->y += right;
			this->z += right;
		}
		void operator*=(const _vec3& right) noexcept {
			this->x *= right.x;
			this->y *= right.y;
			this->z *= right.z;
		}
		void operator*=(const Type right) noexcept {
			this->x *= right;
			this->y *= right;
			this->z *= right;
		}
		void operator-=(const _vec3& right) noexcept {
			this->x -= right.x;
			this->y -= right.y;
			this->z -= right.z;
		}
		void operator-=(const Type right) noexcept {
			this->x -= right;
			this->y -= right;
			this->z -= right;
		}
		void operator/=(const _vec3& right) noexcept {
			this->x /= right.x;
			this->y /= right.y;
			this->z /= right.z;
		}
		void operator/=(const Type right) noexcept {
			this->x /= right;
			this->y /= right;
			this->z /= right;
		}
		const friend _vec3 operator+(const _vec3 &a, const _vec3 &b) noexcept {
			return _vec3(a.x+b.x, a.y+b.y, a.z+b.z);
		}
		const friend _vec3 operator+(const _vec3 &a, const Type b) noexcept {
			return _vec3(a.x+b, a.y+b, a.z+b);
		}
		const friend _vec3 operator-(const _vec3 &a, const _vec3 &b) noexcept {
			return _vec3(a.x-b.x, a.y-b.y, a.z-b.z);
		}
		const friend _vec3 operator-(const _vec3 &a, const Type b) noexcept {
			return _vec3(a.x-b, a.y-b, a.z-b);
		}
		const friend _vec3 operator*(const _vec3 &a, const _vec3 &b) noexcept {
			return _vec3(a.x*b.x, a.y*b.y, a.z*b.z);
		}
		const friend _vec3 operator*(const _vec3 &a, const Type b) noexcept {
			return _vec3(a.x*b, a.y*b, a.z*b);
		}
		const friend _vec3 operator/(const _vec3 &a, const _vec3 &b) noexcept {
			return _vec3(a.x/b.x, a.y/b.y, a.z/b.z);
		}
		const friend _vec3 operator/(const _vec3 &a, const Type b) noexcept {
			return _vec3(a.x/b, a.y/b, a.z/b);
		}
	};
	template<typename Type> class _vec4 {
	public:
		union {
			Type x, r, s;
		};
		union {
			Type y, g, t;
		};
		union {
			Type z, b, p;
		};
		union {
			Type w, a, q;
		};
		_vec4() noexcept {};
		_vec4(Type x, Type y, Type z, Type w) noexcept :x(x), y(y), z(z), w(w) {};
		const bool operator==(const _vec4 &arg) noexcept {
			return (this->x==arg.x) && (this->y==arg.y) && (this->z==arg.z) && (this->w==arg.w);
		}
		void operator+=(const _vec4& right) noexcept {
			this->x += right.x;
			this->y += right.y;
			this->z += right.z;
			this->w += right.w;
		}
		void operator+=(const Type right) noexcept {
			this->x += right;
			this->y += right;
			this->z += right;
			this->w += right;
		}
		void operator*=(const _vec4& right) noexcept {
			this->x *= right.x;
			this->y *= right.y;
			this->z *= right.z;
			this->w *= right.w;
		}
		void operator*=(const Type right) noexcept {
			this->x *= right;
			this->y *= right;
			this->z *= right;
			this->w *= right;
		}
		void operator-=(const _vec4& right) noexcept {
			this->x -= right.x;
			this->y -= right.y;
			this->z -= right.z;
			this->w -= right.w;
		}
		void operator-=(const Type right) noexcept {
			this->x -= right;
			this->y -= right;
			this->z -= right;
			this->w -= right;
		}
		void operator/=(const _vec4& right) noexcept {
			this->x /= right.x;
			this->y /= right.y;
			this->z /= right.z;
			this->w /= right.w;
		}
		void operator/=(const Type right) noexcept {
			this->x /= right;
			this->y /= right;
			this->z /= right;
			this->w /= right;
		}
		const friend _vec4 operator+(const _vec4 &a, const _vec4 &b) noexcept {
			return _vec4(a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w);
		}
		const friend _vec4 operator+(const _vec4 &a, const Type b) noexcept {
			return _vec4(a.x+b, a.y+b, a.z+b, a.w+b);
		}
		const friend _vec4 operator-(const _vec4 &a, const _vec4 &b) noexcept {
			return _vec4(a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w);
		}
		const friend _vec4 operator-(const _vec4 &a, const Type b) noexcept {
			return _vec4(a.x-b, a.y-b, a.z-b, a.w-b);
		}
		const friend _vec4 operator*(const _vec4 &a, const _vec4 &b) noexcept {
			return _vec4(a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w);
		}
		const friend _vec4 operator*(const _vec4 &a, const Type b) noexcept {
			return _vec4(a.x*b, a.y*b, a.z*b, a.w*b);
		}
		const friend _vec4 operator/(const _vec4 &a, const _vec4 &b) noexcept {
			return _vec4(a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w);
		}
		const friend _vec4 operator/(const _vec4 &a, const Type b) noexcept {
			return _vec4(a.x/b, a.y/b, a.z/b, a.w/b);
		}
	};
	
	typedef _vec2<float> vec2;
	typedef _vec2<char> bvec2;
	typedef _vec2<unsigned char> ubvec2;
	typedef _vec2<short> svec2;
	typedef _vec2<unsigned short> usvec2;
	typedef _vec2<int> ivec2;
	typedef _vec2<unsigned int> uivec2;
	
	typedef _vec3<float> vec3;
	typedef _vec3<char> bvec3;
	typedef _vec3<unsigned char> ubvec3;
	typedef _vec3<short> svec3;
	typedef _vec3<unsigned short> usvec3;
	typedef _vec3<int> ivec3;
	typedef _vec3<unsigned int> uivec3;
	
	typedef _vec4<float> vec4;
	typedef _vec4<char> bvec4;
	typedef _vec4<unsigned char> ubvec4;
	typedef _vec4<short> svec4;
	typedef _vec4<unsigned short> usvec4;
	typedef _vec4<int> ivec4;
	typedef _vec4<unsigned int> uivec4;
	
	template<typename Type> class Array {
	public:
		Type *data;
		unsigned int size;
		
		Array(unsigned int size) {
			this->data = (Type*)malloc( size*sizeof(Type) );
			if (this->data==NULL) {
				this->size = 0;
			}else{
				this->size = size;
			}
		}
		~Array() {
			this->size = 0;
			free(this->data);
		}
		Type operator[](size_t i) const {
			return this->data[i];
		}
		Type& operator[](size_t i) {
			return this->data[i];
		}
	};
	typedef struct __attribute__((packed)) {
		uint16_t	signature;
		uint32_t	sizeFile;
		uint16_t	null1;
		uint16_t	null2;
		uint32_t	offsetPixels;
		uint32_t	sizeHead;
		uint32_t	width;
		uint32_t	height;
		uint16_t	layers;
		uint16_t	bpp;
		uint32_t	compression;
		uint32_t	compressionSize;
		uint32_t	ppmWidth;
		uint32_t	ppmHeight;
		uint32_t	colorsUse;
		uint32_t	colorsSpecial;
	} bmpHead;
	typedef struct __attribute__((packed)) {
		uint8_t	IDLength;
		uint8_t	colorMapType;
		uint8_t	imgType;
		//3
		uint16_t	colorMapIndex;
		uint16_t	colorMapLength;
		uint8_t		colorMapItemSize;
		//5
		uint16_t	x;
		uint16_t	y;
		uint16_t	width;
		uint16_t	height;
		//8
		uint8_t	colorDepth;
		uint8_t	imgDescriptor;
		//7+8+2
	} tgaHead;
	typedef struct __attribute__((packed)) {
		uint16_t signature;
		uint16_t ver;
		uint32_t dirLength;
		uint32_t imgLength;
		uint32_t imgOffset;
		uint32_t size;
		uint8_t	nameLength;
		uint8_t	descriptLength;//CDCDE
		//8
		//id
		//item
	} sdHead;
	typedef struct __attribute__((packed)) {
		uint16_t x;
		uint16_t y;
		uint32_t size;
		//8
		//id
		//item
	} sdTypeDir;
	typedef struct __attribute__((packed)) {
		uint16_t x;
		uint16_t y;
		uint32_t ID;
		uint32_t offset;
		//8
		//id
		//item
	} sdTypeImg;
	
	

	typedef struct{
		enum TYPE {
			PIXELS,
			INCHES,
			CENTIMETERS,
			MILIMETERS,
			POINTS,
			PICAS,
			PERCENTAGES
		};
		TYPE type;
		union {
			int pixels;
			float inches;
			float centimeters;
			float milimeters;
			float points;
			float picas;
			float percentages;
		};
	} Metric;
	void metricToPixels(Metric *m, int v1=0);
	//template class EventDispatcher<EventFileLoad>;
	
	class System {
	public:
		#ifdef WIN32
			static HINSTANCE hInstance;
		#endif
		static uint16_t countProcessors;
		static void init();
	};
	
	enum ASCII_SPEC:char {
		NUL=0x00,//ignore
		SOH=0x01,//ignore
		STX=0x02,//ignore
		ETX=0x03,//ignore
		EOT=0x04,//ignore
		ENQ=0x05,//ignore
		ACK=0x06,//ignore
		BEL=0x07,//ignore
		BS=0x08,//РІРѕР·РІСЂР°С‚ РЅР° РѕРґРёРЅ СЃРёРјРІРѕР»
		TAB=0x09,//РіРѕСЂРёР·РѕРЅС‚Р°Р»СЊРЅР°СЏ С‚Р°Р±СѓР»СЏС†РёСЏ
		LF=0x0A,//РїРµСЂРµРІРѕРґ СЃС‚СЂРѕРєРё
		VT=0x0B,//РІРµСЂС‚РёРєР°Р»СЊРЅР°СЏ С‚Р°Р±СѓР»СЏС†РёСЏ
		FF=0x0C,//ignore
		CR=0x0D,//ignore РІРѕР·РІСЂР°С‚ РєР°СЂРµС‚РєРё
		SO=0x0E,//
		SI=0x0F,//
		DLE=0x10,//ignore
		DC1=0x11,//ignore
		DC2=0x12,//ignore
		DC3=0x13,//ignore
		DC4=0x14,//ignore
		NAK=0x15,//ignore
		SYN=0x16,//ignore
		ETB=0x17,//ignore
		CAN=0x18,//ignore
		EM=0x19,//ignore
		SUB=0x1A,//ignore
		ESC=0x1B,//ignore
		FS=0x1C,//ignore
		GS=0x1D,//ignore
		RS=0x1E,//ignore
		US=0x1F,//ignore
		DEL=0x7F//ignore
	};
}

#endif	/* GRBASETYPES_H */