#include "audiolistener.h"
#include "componentmanager.h"
#include "gameobject.h"

namespace ql {
	Listener::Listener(GameObject &owner, const void *data)
		: parent(&owner), active(true) {
		(void)data;
	}
	void Listener::update() {
		static int n = 0;
		n = n + n;
	}

	COMPONENT_REGISTER(Listener);
} // namespace ql