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
    RigidBody::RigidBody(const void* data) {
        mass = 1;
        drag = 0;
        angulardrag = 0;
        automasscenter = true;
        usegravity = true;
        iskinematic = false;
    }
    
    RigidBody::RigidBody(RigidBody& other) {
        mass = other.mass;
        drag = other.drag;
        angulardrag = other.angulardrag;
        automasscenter = other.automasscenter;
        usegravity = other.usegravity;
        iskinematic = other.iskinematic;
    }
    
    RigidBody::RigidBody(RigidBody&& other) {
        mass = other.mass;
        drag = other.drag;
        angulardrag = other.angulardrag;
        automasscenter = other.automasscenter;
        usegravity = other.usegravity;
        iskinematic = other.iskinematic;
    }
    
    RigidBody& RigidBody::operator=(RigidBody& other) {
        mass = other.mass;
        drag = other.drag;
        angulardrag = other.angulardrag;
        automasscenter = other.automasscenter;
        usegravity = other.usegravity;
        iskinematic = other.iskinematic;
        
        return *this;
    }
    
    RigidBody& RigidBody::operator=(RigidBody&& other) {
        mass = other.mass;
        drag = other.drag;
        angulardrag = other.angulardrag;
        automasscenter = other.automasscenter;
        usegravity = other.usegravity;
        iskinematic = other.iskinematic;
        
        return *this;
    }
    
    COMPONENT_REGISTER(RigidBody);
} // namespace ql
