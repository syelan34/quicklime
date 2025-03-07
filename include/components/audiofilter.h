#pragma once

#include "audio/audiomanager.h"
#include <memory>

namespace ql {
	enum FilterType {
		FILTER_HIGHPASS,
		FILTER_LOWPASS,
		FILTER_BANDPASS,
		FILTER_NOTCH
	};

	class GameObject;

	class AudioFilter {
		std::weak_ptr<GameObject> p;
		FilterType t;
		bool onListener;

	  public:
		AudioFilter(std::weak_ptr<GameObject> owner, const void *data);
		void apply(ndsp_channel channel = -1);
		void disable(ndsp_channel channel = -1);
	};
} // namespace ql