// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <engine-base/settings.h>

namespace tst {
namespace engine {
    namespace vulkan {

        struct settings : public base::settings {
            using super = base::settings;
        public:
            enum class buffering { double_buf = 2, triple_buf = 3 };

        public:
            constexpr settings(base::settings&& settings, buffering buffers);

        public:
            static constexpr std::uint32_t m_inFlightFrames = 3;
            buffering m_buffering;

        private:
        };

        constexpr settings::settings(base::settings&& settings, buffering buffers)
            : base::settings(settings), m_buffering(buffers) {
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
