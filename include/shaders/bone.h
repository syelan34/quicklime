#pragma once

#include "util/dualquat.h"

namespace ql {
	struct bone {
		uint16_t ID, parentID;
		DualQuat transform;
	};
}