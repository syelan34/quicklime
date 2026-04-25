#pragma once

#include <memory>
namespace ql {
	class GameObject;
	class Listener {
		friend class AudioManager;
		friend class AudioSource;
		bool active;

	  public:
		Listener(const void *data);
		void update();
	};
} // namespace ql