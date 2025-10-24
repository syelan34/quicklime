#pragma once

#include <string>

namespace ql {
    // Configurations for Quicklime projects
    struct Project {
        static const char* entrypointScene;
        static bool programShouldExitGraceful;
    };
}