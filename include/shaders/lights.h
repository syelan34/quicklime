#pragma once

#include <3ds.h>
#include <citro3d.h>

#define HW_NUM_LIGHTS 8

namespace ql::lights {
    using light_id = u32;
    using light_bitmask = u8;
    
	extern C3D_Light lights[HW_NUM_LIGHTS];
	extern const light_bitmask& lights_in_use;
	
	light_id allocateLight();
	void freeLight(light_id lightIdx);
	
	void set_lightenv();
}