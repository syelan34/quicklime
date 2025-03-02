#include "ql_time.h"

#include <3ds.h>

namespace ql {
    namespace {
        static unsigned long long curtime = 0, oldtime = 0;
		static float _delta = 0.0167f, _cur;
    }
    namespace Time {
        void Update() {
            curtime = osGetTime();
            _delta = (curtime - oldtime) * 0.001f;
            oldtime = curtime;
            _cur = curtime * 0.001f;
        }
        const float &deltaTime	= _delta;
        const float &curTime    = _cur;
    }

} // namespace ql