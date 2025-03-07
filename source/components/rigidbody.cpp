#include "rigidbody.h"
#include "componentmanager.h"

namespace ql {
    namespace {
        struct rigidbody_args {
            float mass;
            float drag;
            float angulardrag;
            bool automasscenter;
            bool usegravity;
            bool iskinematic;
        };
    }
    RigidBody::RigidBody(std::weak_ptr<GameObject> parent, const void* data): parent(parent) {
        mass = 1;
        drag = 0;
        angulardrag = 0;
        automasscenter = true;
        usegravity = true;
        iskinematic = false;
    }
    void RigidBody::update() {
        
    }
    RigidBody::RigidBody(RigidBody& other) {
        mass = other.mass;
        drag = other.drag;
        angulardrag = other.angulardrag;
        automasscenter = other.automasscenter;
        usegravity = other.usegravity;
        iskinematic = other.iskinematic;
        parent = other.parent;
        other.parent = {};
    }
    
    RigidBody::RigidBody(RigidBody&& other) {
        mass = other.mass;
        drag = other.drag;
        angulardrag = other.angulardrag;
        automasscenter = other.automasscenter;
        usegravity = other.usegravity;
        iskinematic = other.iskinematic;
        parent = other.parent;
        other.parent = {};
    }
    
    RigidBody& RigidBody::operator=(RigidBody& other) {
        mass = other.mass;
        drag = other.drag;
        angulardrag = other.angulardrag;
        automasscenter = other.automasscenter;
        usegravity = other.usegravity;
        iskinematic = other.iskinematic;
        parent = other.parent;
        
        return *this;
    }
    
    RigidBody& RigidBody::operator=(RigidBody&& other) {
        mass = other.mass;
        drag = other.drag;
        angulardrag = other.angulardrag;
        automasscenter = other.automasscenter;
        usegravity = other.usegravity;
        iskinematic = other.iskinematic;
        parent = other.parent;
        other.parent = {};
        
        return *this;
    }
    
    COMPONENT_REGISTER(RigidBody);
} // namespace ql
