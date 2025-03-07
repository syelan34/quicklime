#include "audiofilter.h"
#include "componentmanager.h"
#include "audiolistener.h"
#include "defines.h"
#include "ql_assert.h"

namespace ql {
	void applyFilter(FilterType t, ndsp_channel c, bool biquad) {
		if (biquad)
			switch (t) {
			case FILTER_HIGHPASS:
			case FILTER_LOWPASS:
			case FILTER_BANDPASS:
			case FILTER_NOTCH:
				break;
			}
		else {
		}
	}
	
	namespace {
        struct filter_args {
            FilterType t;
        };
    }

	AudioFilter::AudioFilter(std::weak_ptr<GameObject> obj, const void *data) {
	    (void)data;
        p = obj;
        t = FILTER_LOWPASS;
        onListener = false;
        ASSERT(!p.expired(), "Parent expired");
        if (p.lock()->getComponent<Listener>() != nullptr)
            onListener = true;
	}

	// apply filter
	void AudioFilter::apply(ndsp_channel channel) {
	    ASSERT(!p.expired(), "Parent expired");
	    if (p.lock()->getComponent<Listener>() != nullptr)
            onListener = true;
		switch (channel) {
		case -1: // all channels
		    for (int i = 0; i < NDSP_NUM_CHANNELS; i++) applyFilter(t, i, onListener);
			break;
		default: // specific channel
		    applyFilter(t, channel, onListener);
			break;
		}
	}

	COMPONENT_REGISTER(AudioFilter)
} // namespace ql
