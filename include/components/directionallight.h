#pragma once

#include <citro3d.h>

namespace ql {
	class DirectionalLight {
		friend class Light;
		C3D_FVec position, direction;
		bool distanceattenuation;
		int id;
		C3D_FVec color;

	  public:
		DirectionalLight(const void *data);
		DirectionalLight(DirectionalLight &&l);
		DirectionalLight &operator=(DirectionalLight &&l);
		void set(C3D_Mtx &modelView);
	};
} // namespace ql