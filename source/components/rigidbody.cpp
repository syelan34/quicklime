#include "rigidbody.h"
#include "componentmanager.h"

namespace ql {
    RigidBody::RigidBody(GameObject& parent, const void* data): parent(&parent) {
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
        other.parent = nullptr;
    }
    
    RigidBody::RigidBody(RigidBody&& other) {
        mass = other.mass;
        drag = other.drag;
        angulardrag = other.angulardrag;
        automasscenter = other.automasscenter;
        usegravity = other.usegravity;
        iskinematic = other.iskinematic;
        parent = other.parent;
        other.parent = nullptr;
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
        other.parent = nullptr;
        
        return *this;
    }
    
    COMPONENT_REGISTER(RigidBody);
} // namespace ql
