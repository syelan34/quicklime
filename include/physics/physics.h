#pragma once

#include "3ds/types.h"

namespace ql {
	void physicsThread(void *);
	template<typename T> void physicsInit(T tickspeed);
	void physicsInit(u64 tickspeed);
	void physicsInit(float tickspeed);
	void physicsExit();
} // namespace ql