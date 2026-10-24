#include "gameobject.h"
#include <3ds.h>
#include "console.h"
#include "scene.h"
#include "camera.h"

ql::GameObject::GameObject(std::string name, Scene &s)
	: s(s), id(s.reg.create()), name(name) {
	Camera::cameraObjectListDirty = true;
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

std::weak_ptr<ql::GameObject> ql::GameObject::find(std::string_view path) {
    if (path.empty())
        return {};

    if (path.starts_with("./"))
        return find(path.substr(2));

    if (path.starts_with("../")) {
        auto p = parent.lock();
        if (!p) {
            Console::Warn("Object %s not found", path.data());
            return {};
        }
        return p->find(path.substr(3));
    }

    if (path.front() == '/') {
        auto root = s.root;
        if (!root)
            return {};
        if (path.size() == 1)
            return root;
        return root->find(path.substr(1));
    }

    const auto slashpos = path.find('/');
    const auto segment = path.substr(0, slashpos);

    for (const auto &child : children) {
        if (!child || child->name != segment)
            continue;

        if (slashpos == std::string_view::npos)
            return child;

        return child->find(path.substr(slashpos + 1));
    }

    return {};
}

void ql::GameObject::assignChild(std::shared_ptr<GameObject> &object) {
    if (!object || object.get() == this || !object->parent.expired())
        return;

    for (auto ancestor = parent.lock(); ancestor;
         ancestor = ancestor->parent.lock()) {
        if (ancestor.get() == object.get())
            return;
    }

    object->parent = weak_from_this();
    children.push_back(std::move(object));
}

void ql::GameObject::assignChild(std::shared_ptr<GameObject> &&object) {
    if (!object || object.get() == this || !object->parent.expired())
        return;

    for (auto ancestor = parent.lock(); ancestor;
         ancestor = ancestor->parent.lock()) {
        if (ancestor.get() == object.get())
            return;
    }

    object->parent = weak_from_this();
    children.push_back(std::move(object));
}

ql::GameObject::~GameObject() {
	if (id != entt::null)
		s.reg.destroy(id);
	Camera::cameraObjectListDirty = true;
	Console::Log("Children Size: ", children.size());
}
