#include "pointlight.h"

namespace ql {
	namespace {
		struct pointlight_args {};
	} // namespace
	PointLight::PointLight(const void *data) {(void)data;}
	PointLight::PointLight(PointLight &&l) {}
	PointLight &PointLight::operator=(PointLight &&l) {return *this;}
	void PointLight::set(C3D_Mtx &modelView) {(void)modelView;}
	PointLight::~PointLight() {}
} // namespace ql