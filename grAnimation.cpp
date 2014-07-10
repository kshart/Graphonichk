#include "grAnimation.h"

namespace Animation {
	
	template<> int TimePoint<int>::average (const int &startValue, float scale) const noexcept {
		return (int)roundf((value-startValue)*scale)+startValue;
	}
	template<> short TimePoint<short>::average (const short &startValue, float scale) const noexcept {
		return (short)roundf((value-startValue)*scale)+startValue;
	}
	
	template<class Tvalue> Tvalue TimeLine<Tvalue>::getValue(float time) noexcept {
		if (time > lastTime) {
			while (lastPoint<timePoint.size) {
				float t1 = lastTime+timePoint[lastPoint].timeLength;
				if (t1 > time) {
					return timePoint[lastPoint].average(lastValue, (time-lastTime)/timePoint[lastPoint].timeLength);
				}
				lastValue = timePoint[lastPoint].value;
				lastTime += timePoint[lastPoint].timeLength;
				lastPoint++;
			}
		}else{
			/*while (lastPoint>0) {
				float t1 = lastTime-timePoint[lastPoint].timeLength;
				if (t1 < time) {
					return timePoint[lastPoint].average(lastValue, (time-lastTime)/timePoint[lastPoint].timeLength);
				}
				lastValue = timePoint[lastPoint].value;
				lastTime -= timePoint[lastPoint].timeLength;
				lastPoint--;
			}*/
		}
	}
	
}