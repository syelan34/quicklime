#pragma once

#include <bullet/btBulletDynamicsCommon.h>

namespace ql::systems {
    class Physics {
        Physics() = delete;
        public:
        static void Init(float interval);
        static void Exit();
    };
}