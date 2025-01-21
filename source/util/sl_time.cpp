#include "ql_time.h"

#include <3ds.h>

namespace ql {
	void Time::Update() {
		curtime = osGetTime();
		_delta	= (curtime - oldtime) * 0.001f;
		oldtime = curtime;
		_cur = curTime * 0.001f;
	}

	float Time::_delta				 = 0.0167f;
	const float &Time::deltaTime	 = _delta;
	const float &Time::curTime		 = _cur;
	unsigned long long Time::curtime = 0, Time::oldtime = 0;

} // namespace ql