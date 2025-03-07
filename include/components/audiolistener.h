#pragma once

#include <memory>
namespace ql {
	class GameObject;
	class Listener {
		friend class AudioManager;
		friend class AudioSource;
		std::weak_ptr<GameObject> parent;
		bool active;

	  public:
		Listener(std::weak_ptr<GameObject> owner, const void *data);
		void update();
	};
} // namespace ql