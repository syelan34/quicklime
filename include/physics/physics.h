#pragma once

#include <3ds.h>

namespace ql {
	void physicsThread(void *);
	void physicsInit(int tickspeed);
	void physicsInit(float tickspeed);
	void physicsExit();
} // namespace ql