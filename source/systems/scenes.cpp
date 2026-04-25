#include "systems/scenes.h"
// #include "camera.h"
#include "console.h"
#include "threads.h"
#include <memory>

namespace ql::systems {
	bool Scenes::loadSceneNextFrame = false;
	std::unique_ptr<Scene> Scenes::currentScene,
		Scenes::sceneToBeLoaded;
	LightLock Scenes::lock;

	void Scenes::Init() {
		// disallows setting a new scene while also updating what the new scene
		// will be
		LightLock_Init(&lock);
	}

	void Scenes::setScene(std::unique_ptr<Scene> &s) {
		if (!s)
			return; // make sure it is a real scene (non null)

		LightLock_Guard l(lock);
		sceneToBeLoaded	   = std::move(s);
		loadSceneNextFrame = true;
	}

	void Scenes::setScene(std::unique_ptr<Scene> &&s) {
		if (!s)
			return; // make sure it is a real scene (non null)

		LightLock_Guard l(lock);
		sceneToBeLoaded	   = std::move(s);
		loadSceneNextFrame = true;
	}

	// must only be called from main thread
	void Scenes::Update() {
		LightLock_Guard l(lock);
		if (currentScene)
			currentScene->update();

		if (!loadSceneNextFrame)
			return;

		// changing the scene

		// don't reload it again next frame
		loadSceneNextFrame = false;

		// first delete old scene
		if (currentScene)
		    Console::Log("Scene '%s' try reset", currentScene->name.c_str());
		currentScene.reset();
		Console::Success("Scene reset");

		// then swap in new scene
		currentScene = std::move(sceneToBeLoaded);

		// start the scene objects
		Console::Log("Try wake up");
		currentScene->awake();
		Console::Success("Scene woke up");
		Console::Log("Scene starting");
		currentScene->start();
		Console::Success("Scene started");
	}
} // namespace ql
