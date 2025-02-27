#include "light.h"
#include "componentmanager.h"
#include <utility>

LightLock Light::lock = LightLock();

Light::Light(C3D_FVec color)
	: color(color) {
	LightLock_Guard l(lock);
	if (lights::lightenvneedsupdating) {
		C3D_LightEnvInit(&lights::lightenv);
		C3D_LightEnvBind(&lights::lightenv);
		lights::lightenvneedsupdating = false;
	}
	Light &Light::operator=(ql::Light &&l) {
		internal_light = std::move(l.internal_light);
		l.internal_light.emplace<DummyLight>();
		t	= l.t;
		p	= l.p;
		l.p = nullptr;
		return *this;
	}

	void Light::setSelf(C3D_Mtx &view) {
		std::visit([&](auto &light) { light.set(view); }, internal_light);
	}
	COMPONENT_REGISTER(Light);
} // namespace ql