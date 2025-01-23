#pragma once

namespace ql {
	class GameObject;
	class Listener {
		friend class AudioManager;
		friend class AudioSource;
		GameObject *parent;
		bool active;

	  public:
		Listener(GameObject &owner, const void *data);
		void update();
	};
} // namespace ql