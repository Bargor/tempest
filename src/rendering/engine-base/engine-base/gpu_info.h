// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <string>

namespace tst {
namespace engine {

    namespace base {
        class gpu_info {
        public:
            enum class device_type { discrete, integrated, cpu, other };
            enum class device_vendor { nvidia, amd, intel, other };

            std::uint32_t versionMajor;
            std::uint32_t versionMinor;
            std::uint32_t versionPatch;
            device_type deviceType;
            device_vendor deviceVendor;
            std::string deviceName;

            std::uint32_t maxIndexValue;
            std::uint32_t maxFramebufferWidth;
            std::uint32_t maxFramebufferHeight;
            std::uint32_t maxColorAttachments;
            std::uint32_t maxImageDimension2D;
            std::uint32_t maxVertexInputAttributes;
            std::uint32_t maxVertexInputBindings;
        };
    } // namespace base
} // namespace engine
} // namespace tst