#pragma once

#include <citro3d.h>

namespace ql {
	class GameObject;

	class BoxCollider {
		GameObject *p;
		C3D_FVec dimensions, center;
		bool isTrigger;

	  public:
	    BoxCollider(GameObject &owner, const void *data);
		
	};
} // namespace ql