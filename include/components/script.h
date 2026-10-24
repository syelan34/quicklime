#pragma once

#include <external/entt/entt.hpp>
#include <string>
#include "util/gameobject.h"

namespace ql {
	class Script {
	  protected:
		std::weak_ptr<GameObject> owner;

		template <class T>
		inline T *GetComponent() {
			auto obj = owner.lock();
			if (!obj)
				return nullptr;

			return obj->s.reg.try_get<T>(obj->id);
		}

				std::weak_ptr<GameObject> find(std::string object);
	  public:
		bool enabled = true;
		Script(std::weak_ptr<GameObject> owner);
		void SetEnabled(bool enabled);
		virtual ~Script(){};
		virtual void Awake(void){};
		virtual void Start(void){};
		virtual void Update(void){};
		virtual void FixedUpdate(void){};
		virtual void LateUpdate(void){};
		virtual void OnCollisionEnter(void){};
		virtual void OnCollisionStay(void){};
		virtual void OnCollisionExit(void){};
		virtual void OnTriggerEnter(void){};
		virtual void OnTriggerStay(void){};
		virtual void OnTriggerExit(void){};
		virtual void OnDisable(void){};
		virtual void OnEnable(void){};
	};
} // namespace ql