#pragma once

#include "util/gameobject.h"
#include "components/script.h"
#include "util/console.h"
#include <3ds.h>
#include <external/entt/entt.hpp>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include "components/script.h"

namespace ql {
	template <typename T>
	concept validcomponent = requires(std::weak_ptr<GameObject> obj, const void *data) {
								 T(obj, data);
							 } || std::is_base_of_v<Script, T>;

	class ComponentManager {
		static LightLock _l;
		ComponentManager() {}
		template <typename T> static void attachScript(std::weak_ptr<GameObject> owner) {
		    if (owner.expired()) return;
			owner.lock()->s.scripts.push_back(std::make_shared<T>(owner));
		}
		template <typename T>
		static void attachComponent(std::weak_ptr<GameObject> obj, const void *data) {
		    if (obj.expired()) return;
		    obj.lock()->addComponent<T>(obj, data);
		}

	  public:
        template <typename T>
            requires validcomponent<T>
        static bool registerComponent(const std::string name) {
            Console::log(name.c_str());
        if constexpr (std::is_base_of_v<Script, T>)
            getScriptMap()[name] = ComponentManager::attachScript<T>;
        else
            getComponentMap()[name] = ComponentManager::attachComponent<T>;
       	return true;
        }

		static inline bool init() {
			LightLock_Init(&_l);
			return true;
		}

		static bool addComponent(const char *name, std::weak_ptr<GameObject> obj,
								 const void *data = nullptr);
		static bool addScript(const char *name, std::weak_ptr<GameObject> obj);

	  private:
		// Use Meyer's singleton to prevent SIOF
		static std::unordered_map<std::string, void (*)(std::weak_ptr<GameObject>)> &
		getScriptMap() {
			static std::unordered_map<std::string, void (*)(std::weak_ptr<GameObject>)> map;
			return map;
		}
		static std::unordered_map<std::string, void (*)(std::weak_ptr<GameObject>, const void *)> &
		getComponentMap() {
			static std::unordered_map<std::string, void (*)(std::weak_ptr<GameObject>, const void *)> map;
			return map;
		}
	};
} // namespace ql

#define COMPONENT_REGISTER(component) \
	__attribute__((weak)) bool component##_component = \
		::ql::ComponentManager::registerComponent<component>(#component);
