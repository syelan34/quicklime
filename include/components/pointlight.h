#pragma once

#include <citro3d.h>

namespace ql {
	class PointLight {
		friend class Light;

	  public:
		PointLight(const void *data);
		PointLight(PointLight &&l);
		PointLight &operator=(PointLight &&l);
		void set(C3D_Mtx &modelView);
		~PointLight();
	};
} // namespace ql