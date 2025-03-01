#include "lights.h"
#include "ql_assert.h"
#include <3ds.h>
#include <citro3d.h>

namespace ql::lights {
    namespace {
        static C3D_LightEnv shared_lightenv;
	static int _lights_in_use = 0;
	static u8 lights_in_use_mask = 0;
	bool light_in_use(int i) { return (lights_in_use_mask & (1 << i)); }
	void enable_light(int i) { 
		lights_in_use_mask |= 1 << i;
		++_lights_in_use;
		C3D_LightInit(&lights[i], &shared_lightenv);
		C3D_LightEnable(&lights[i], true); 
	}
	void disable_light(int i) { 
		lights_in_use_mask &= ~(1 << i);
		--_lights_in_use;
		C3D_LightEnable(&lights[i], false); 
	}
    }
    
    const light_bitmask& lights_in_use = _lights_in_use;
    
    C3D_Light lights[HW_NUM_LIGHTS] = {};
	
    light_id allocateLight() {
	for (int i = 0; i < HW_NUM_LIGHTS; i++) {
		if (!light_in_use(i)) {
			enable_light(i);
			return i;
		}
	}
	return -1;
    }
    
    void freeLight(light_id lightIdx) {
	ASSERT(lightIdx < HW_NUM_LIGHTS, "Invalid light index");
	if (lightIdx >= HW_NUM_LIGHTS) return;
	disable_light(lightIdx);
    }
	
    void set_lightenv() {
        C3D_LightEnvBind(&shared_lightenv);
    }
}
