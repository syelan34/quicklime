#pragma once

#include <memory>
namespace ql {
    class GameObject;
	class RigidBody {
		float mass;
		float drag;
		float angulardrag;
		bool automasscenter;
		bool usegravity;
		bool iskinematic;
		public:
		RigidBody(const void* data);
		RigidBody(RigidBody& other);
		RigidBody(RigidBody&& other);
		RigidBody& operator=(RigidBody& other);
		RigidBody& operator=(RigidBody&& other);
	};
}