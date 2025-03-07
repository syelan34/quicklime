#pragma once

#include <memory>
namespace ql {
    class GameObject;
	class RigidBody {
	    std::weak_ptr<GameObject> parent;
		float mass;
		float drag;
		float angulardrag;
		bool automasscenter;
		bool usegravity;
		bool iskinematic;
		public:
		RigidBody(std::weak_ptr<GameObject> parent, const void* data);
		RigidBody(RigidBody& other);
		RigidBody(RigidBody&& other);
		RigidBody& operator=(RigidBody& other);
		RigidBody& operator=(RigidBody&& other);
		void update();
	};
}