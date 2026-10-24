#include "script.h"
#include "gameobject.h"
#include <string>
namespace ql {
	std::weak_ptr<GameObject> Script::find(std::string object) {
    auto obj = owner.lock();
    if (!obj)
        return {};

    return obj->find(object);
}

	Script::Script(std::weak_ptr<GameObject> _owner) : owner(_owner) {}

	void Script::SetEnabled(bool enabled) {
		bool wasenabled = this->enabled;
		this->enabled	= enabled;
		// only call onEnable() if the enabled state actually changed
		if (enabled && !wasenabled)
			OnEnable();
		// same here
		else if (wasenabled && !enabled)
			OnDisable();
	}
} // namespace ql