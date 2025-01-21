#pragma once

namespace ql {
	class Time {
		Time() = delete;
		static unsigned long long curtime, oldtime;
		static float _delta, _cur;

	  public:
		static const float &deltaTime;
		static const float &curTime;
		static void Update();
	};
} // namespace ql
