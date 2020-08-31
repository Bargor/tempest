// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "draw_info.h"
#include "resources/settings.h"

#include <vulkan/vulkan.hpp>

namespace tst {
namespace application {
    class data_loader;
    class main_window;
    template<typename Event>
    class event_processor;

    struct app_event;
} // namespace application

namespace engine {

    namespace vulkan {
        class device;
        class index_buffer;
        class resource_cache;
        class swap_chain;
        class vertex_buffer;
        class uniform_buffer;

        class engine_frontend {
        public:
            engine_frontend(device& device);
            engine_frontend(const engine_frontend& engine) = delete;
            ~engine_frontend();

            std::vector<vk::CommandBuffer> prepare_draw(const std::vector<draw_info>& drawInfos);

        private:
            vk::CommandBuffer generate_command_buffer(std::vector<draw_info>::const_iterator begin,
                                                      std::vector<draw_info>::const_iterator end);

        private:
            device& m_device;

            std::array<vk::CommandPool, settings::m_inFlightFrames> m_commandPools;
            std::array<std::vector<vk::CommandBuffer>, settings::m_inFlightFrames> m_bufferCache;
        };

    } // namespace vulkan

} // namespace engine
} // namespace tst
