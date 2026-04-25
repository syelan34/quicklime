#pragma once

#include <3ds.h>
#include <citro3d.h>
#include <external/entt/entt.hpp>
#include <string>

namespace ql::systems {class Scenes;};

namespace ql {
    class Script;
    class GameObject;
	class Scene {
		LightLock lock;
		std::string _name;
		std::vector<std::shared_ptr<Script>> scripts;
		std::shared_ptr<GameObject> root;
		entt::registry reg;
		
		// physics
		// btBroadphaseInterface*                  _broadphase;
	 //    btDefaultCollisionConfiguration*        _collisionConfiguration;
	 //    btCollisionDispatcher*                  _dispatcher;
	 //    btSequentialImpulseConstraintSolver*    _solver;
	 //    btDiscreteDynamicsWorld*                _world;

		friend class SceneLoader;
		friend class Camera;
		friend class AudioSource;
		friend class AudioManager;
		friend class GameObject;
		friend class Script;
		friend class ComponentManager;
		friend class systems::Scenes;
		friend void physicsThread(void *);
		friend void sceneLoadThread(void *params);

		void act_on_scripts(void (Script::*action)());
		
		void awake();
		void start();
		void update();
		void fixedUpdate();

	  public:
		const std::string &name = _name;
		~Scene();

		Scene(std::string name);
	};
} // namespace ql