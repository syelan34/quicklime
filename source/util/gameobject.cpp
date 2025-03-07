#include "gameobject.h"
#include <3ds.h>
#include "console.h"
#include "scene.h"
#include "camera.h"

ql::GameObject::GameObject(std::string name, Scene &s)
	: s(s), id(s.reg.create()), name(name) {
	Camera::cameraObjectListDirty = true;
}

ql::GameObject::GameObject(GameObject &other): s(other.s), children(other.children), id(other.id), name(other.name) {}

ql::GameObject::GameObject(GameObject &&other)
	: s(other.s), children(std::move(other.children)), 
	  parent(other.parent),
	  id(other.id), name(other.name) {
	other.parent = {};
	other.id	 = entt::entity{entt::null};
	other.children.clear();
}



std::weak_ptr<ql::GameObject> ql::GameObject::r_search(std::string name) {
	std::weak_ptr<ql::GameObject> out;
	for (auto child : children) {
		if (child->name == name)
			return child;
		out = child->r_search(name);
		if (!out.expired())
			return out;
	}
	return out;
}

std::weak_ptr<ql::GameObject> ql::GameObject::find(std::string_view name) {
	switch (name[0]) {
	    case '.':
			switch (name[1]) {
			    case '.': // case ../obj -> get parent and search its children
					if (!parent.expired()) return parent.lock()->find(name.substr(1));
					else Console::warn("Object %s not found", name.data());
					return {}; 
			    case '/':
					return find(name.substr(2));
			}
			break;
		case '/': {
            // get root and start from there
            auto root = std::weak_ptr(s.root);
            return root.lock()->find(name.substr(1));
		}
		default: // obj/obj2 is the same as ./obj/obj2
		    size_t slashpos = name.find_first_of('/');
		    auto subname = name.substr(0, slashpos);
            for (auto& obj : this->children) 
                if (obj->name == subname) return obj->find(name.substr(slashpos+1));
	}
	return {};
};

void ql::GameObject::assignChild(std::shared_ptr<GameObject> &object) {
    children.push_back(std::move(object));
}

void ql::GameObject::assignChild(std::shared_ptr<GameObject> &&object) {
    children.push_back(std::move(object));
}

ql::GameObject::~GameObject() {
	if (id != entt::null)
		s.reg.destroy(id);
	Camera::cameraObjectListDirty = true;
}
