// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "draw_info.h"
#include "engine_init.h"
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
        class device_queues;
        template<typename IndexType>
        class index_buffer;
        class resource_cache;
        class swap_chain;
        class vertex_buffer;
        class uniform_buffer;

        class engine_frontend {

        public:
            engine_frontend(application::event_processor<application::app_event>& eventProcessor,
                            device& device,
                            resource_cache& resourceCache);
            engine_frontend(const engine_frontend& engine) = delete;
            ~engine_frontend();

            template<typename Iter>
            std::vector<vk::CommandBuffer> prepare_draw(Iter first, Iter last);

        private:
            vk::CommandBuffer generate_command_buffer(const draw_info& drawInfo);

        private:
            application::event_processor<application::app_event>& m_eventProcessor;

            device& m_device;
            resource_cache& m_resourceCache;

            std::array<vk::CommandPool, settings::m_inFlightFrames> m_commandPools;
            std::array<std::vector<vk::CommandBuffer>, 3> m_bufferCache;
            vk::DescriptorPool m_descriptorPool;
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
