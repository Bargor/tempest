// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "draw_info.h"
#include "engine_init.h"
#include "queue_indices.h"

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
        class pipeline_cache;
        class swap_chain;
        class technique_cache;
        class vertex_buffer;
        class uniform_buffer;

        class engine_frontend {
            template<typename T>
            using ptr = std::unique_ptr<T>;

        public:
            engine_frontend(application::event_processor<application::app_event>& eventProcessor,
                            device& device,
                            technique_cache& techniqueCache,
                            pipeline_cache& pipelineCache);
            engine_frontend(const engine_frontend& engine) = delete;
            ~engine_frontend();

            template<typename Iter>
            std::vector<vk::CommandBuffer> prepare_draw(Iter first, Iter last);

        private:
            vk::CommandBuffer generate_command_buffer(const draw_info& drawInfo);

        private:
            static constexpr std::uint32_t m_maxConcurrentFrames = 2;

            application::event_processor<application::app_event>& m_eventProcessor;

            device& m_device;
            technique_cache& m_techniqueCache;
            pipeline_cache& m_pipelineCache;

            std::vector<vk::CommandPool> m_commandPools;
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
