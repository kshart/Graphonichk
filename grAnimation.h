#ifndef GRANIMATION_H
#define	GRANIMATION_H

#include "grMain.h"


namespace Animation {
	template<class Tvalue> class TimePoint;
	template<class Tvalue> class TimeLine;
	//(24+12*100)*12
	//(20+8*100)*12
	template<class Tvalue> class TimePoint {
	public:
		enum TYPE:short {
			LINEAR
		};
		TimePoint(float sec, Tvalue v, TYPE type=TYPE::LINEAR) :timeLength(sec), type(type), value(v) {};
		Tvalue average (const Tvalue &startValue, float scale) const noexcept;
	private:
		float timeLength;
		Tvalue value;
		TYPE type;
		friend TimeLine<Tvalue>;
	};
	template<class Tvalue> class TimeLine {
	public:
		float lastTime = 0;
		size_t lastPoint = 0;
		Tvalue startValue;
		Tvalue lastValue;
		Graphonichk::Array< TimePoint<Tvalue> > timePoint;
		
		TimeLine(size_t length, Tvalue startValue) :timePoint(length), startValue(startValue), lastValue(startValue) {};
		
		Tvalue getValue (float time) noexcept;
	};
}

namespace Graphonichk {
}





#endif