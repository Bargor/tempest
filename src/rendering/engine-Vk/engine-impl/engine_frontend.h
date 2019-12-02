// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "draw_info.h"
#include "queue_indices.h"
#include "resources/settings.h"

#include <memory>
#include <vector>
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
        template<typename IndexType>
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

            template<typename Iter>
            std::vector<vk::CommandBuffer> prepare_draw(Iter first, Iter last);

        private:
            vk::CommandBuffer generate_command_buffer(const draw_info& drawInfo);

        private:
            device& m_device;

            std::array<vk::CommandPool, settings::m_inFlightFrames> m_commandPools;
            std::array<std::vector<vk::CommandBuffer>, settings::m_inFlightFrames> m_bufferCache;
        };

        template<typename Iter>
        std::vector<vk::CommandBuffer> engine_frontend::prepare_draw(Iter first, Iter last) {
            std::vector<vk::CommandBuffer> buffers;

            for (; first != last; ++first) {
                auto& drawItem = *first;
                buffers.emplace_back(generate_command_buffer(drawItem));
            }

            return buffers;
        }

    } // namespace vulkan

} // namespace engine
} // namespace tst
