#pragma once

#include "external/entt/entt.hpp"
#include <memory>
#include <string>
#include <vector>
#include <3ds.h>
#include "scenes/scene.h"

namespace ql {
	class Script;

	class GameObject {
		friend class Camera;
		friend class ComponentManager;
		friend class AudioSource;
		friend class Script;
		friend class SceneManager;
		friend class SceneLoader;
		friend class AudioFilter;
		Scene &s;
		std::vector<std::shared_ptr<GameObject>> children;
		std::weak_ptr<GameObject> parent;
		entt::entity id;
		std::string name; // saved in scene file

		std::weak_ptr<GameObject> r_search(std::string name);
	  public:
		GameObject(std::string name, Scene &s);
		GameObject(GameObject &other);
		GameObject(GameObject &&other);

		operator entt::entity() { return id; }

		/**
		 * @brief Adds a component to GameObject at runtime. If the GameObject
		 * already has the component, this one will replace it.
		 *
		 * @tparam T Component to add
		 * @tparam Args Argument types
		 *
		 * @param args Arguments to pass to the component constructor
		 */
		template <typename T, typename... Args>
		void addComponent(Args &&...args) {
			s.reg.emplace_or_replace<T>(id, std::forward<Args>(args)...);
		}

		/**
		 * @brief Get the Component object
		 *
		 * @tparam T Component to get
		 * @return T* Pointer to the component instance
		 */
		template <typename T> T *getComponent() {
			return s.reg.try_get<T>(id);
		}

		/**
		 * @brief Adds child to self
		 *
		 * @param object Reference to GameObject to take ownership of as child
		 */
		void assignChild(std::shared_ptr<GameObject> &object);
		
		/**
		 * @brief Adds child to self
		 *
		 * @param object GameObject to take ownership of as child
		 */
		void assignChild(std::shared_ptr<GameObject> &&object);

		/**
         * @brief Enables or disables the GameObject
         * This causes all logic to skip over it
         */
		void setEnabled(bool enabled);

		/**
		 * @brief Finds an object within the scene.
		 *
		 * WARNING very slow, it is only intended to find the object in the
		 * Start() function of a script (do NOT use it every frame). Instead,
		 * store the pointer in a variable and reuse it in the future.
		 *
		 * - ` `  in front of name will search from self
		 * - `/`   in front of name will search from root (then find children based on '/' "subdirectories")
		 * - `./`  in front of name will search from self
		 * - `../` in front of name will search from parent (do ../../ to get level above etc)
		 * - All of this assumes no two sibling objects have the same name
		 *
		 * @param path The path of the GameObject to search for
		 * @return A weak pointer to the found GameObject, or empty if not found
		 */
		std::weak_ptr<GameObject> find(std::string_view path);

		~GameObject();
	};
} // namespace ql
