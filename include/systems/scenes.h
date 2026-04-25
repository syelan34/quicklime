#pragma once

#include "scenes/scene.h"
#include "scenes/sceneloader.h"
#include <citro3d.h>
#include <memory>

namespace ql {
    class SceneLoader;
    class Console;
    class AudioManager;
    void sceneLoadThread(void* params);
};

namespace ql::systems {
	// TODO needs to be basically completely rewritten/remade
	class Scenes {
		Scenes() = delete;
		static bool loadSceneNextFrame;
		static void setScene(std::unique_ptr<Scene> &s);
		static void setScene(std::unique_ptr<Scene> &&s);
		
		friend class ::ql::SceneLoader;
		friend class ::ql::Console; // given access to read scene name
		friend class ::ql::AudioManager;
		friend void ::ql::sceneLoadThread(void *params);
		static LightLock lock;
		static std::unique_ptr<Scene> currentScene, sceneToBeLoaded;
		
	  public:
		static void Init();
		static void Update();
		static void Exit();
	};
} // namespace ql
