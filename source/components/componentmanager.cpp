#include "componentmanager.h"
#include "console.h"
#include "threads.h"

LightLock ql::ComponentManager::_l = {};

bool ql::ComponentManager::addComponent(const char *name, std::weak_ptr<GameObject> obj,
									const void *data) {
	LightLock_Guard l(_l);
	if (getComponentMap().find(name) == getComponentMap().end()) {
		Console::Error("Unknown component %s", name);
		Console::Error("Known components:");
		for(const auto& elem : getComponentMap())
			Console::Log("%s: %p", elem.first.c_str(), elem.second);
		return false;
	}
	getComponentMap()[name](obj, data);
	return true;
}

bool ql::ComponentManager::addScript(const char *name, std::weak_ptr<GameObject> obj) {
	LightLock_Guard l(_l);
	if (getScriptMap().find(name) == getScriptMap().end()) {
		Console::Error("Unknown script %s", name);
		return false;
	}
	getScriptMap()[name](obj);
	return true;
}