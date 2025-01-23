#pragma once

namespace ql {
	class rigidbody {
		float mass;
		float drag;
		float angulardrag;
		bool automasscenter;
		bool usegravity;
		bool iskinematic;
	};
}