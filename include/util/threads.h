#pragma once

#include <3ds.h>
#include <type_traits>
#include <utility>

namespace ql {
	struct LightLock_Guard {
		LightLock &_lock;
		LightLock_Guard(LightLock &lock);
		~LightLock_Guard();
	};
	
	template <typename T> requires(!std::is_pointer_v<T>)
	class LightLock_Mutex;

	template <typename T> class LightLock_Mutex_Guard {
		LightLock_Mutex<T> &mut;

	  public:
		LightLock_Mutex_Guard(LightLock_Mutex<T> &mut) : mut(mut) {
			LightLock_Lock(&mut._lock);
		}
		~LightLock_Mutex_Guard() { LightLock_Unlock(&mut._lock); }
	};
	
	template <typename T>
		requires(!std::is_pointer_v<T>)
	class LightLock_Mutex {
	    friend class LightLock_Mutex_Guard<T>;
		LightLock _lock;
		T _val;

	  public:
		LightLock_Mutex() : _val() { LightLock_Init(&_lock); }
		LightLock_Mutex(T &&val) : _val(std::move(val)) {
			LightLock_Init(&_lock);
		}
		T *operator->() { return &_val; }
		T &operator*() { return _val; }
		LightLock_Mutex_Guard<T> lock() { return LightLock_Mutex_Guard(*this); }
	};
} // namespace ql