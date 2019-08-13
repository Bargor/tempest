// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "command_list.h"
#include "resources/index_buffer.h"
#include "resources/uniform_buffer.h"
#include "resources/vertex_buffer.h"

#include <GLFW/glfw3.h>
#include <memory>
#include <vector>

namespace tst {
namespace application {
    template<typename Event>
    class event_processor;
    class main_window;

    struct app_event;
} // namespace application

namespace engine {
    namespace opengl {

        class gpu_info;

        class device {
            template<typename T>
            using ptr = std::unique_ptr<T>;

        public:
            device(application::main_window& mainWindow,
                   application::event_processor<application::app_event>& eventProcessor);
            device(const device& device) = delete;
            ~device();

            template<typename IndexType>
            index_buffer<IndexType> create_index_buffer(std::vector<IndexType>&& indices) const;
            vertex_buffer create_vertex_buffer(const vertex_format& format, std::vector<vertex>&& vertices) const;
            uniform_buffer create_uniform_buffer() const;

            gpu_info& get_GPU_info() const noexcept;
            void submit_command_list(const command_list& commandList);

            bool draw();

        private:
            ptr<gpu_info> m_gpuInfo;
            std::vector<command_list> m_drawCommands;
        };

        template<typename IndexType>
        index_buffer<IndexType> device::create_index_buffer(std::vector<IndexType>&& indices) const {
            return index_buffer<std::uint16_t>(std::move(indices));
        }

        TST_INLINE gpu_info& device::get_GPU_info() const noexcept {
            return *m_gpuInfo;
        }

        TST_INLINE void device::submit_command_list(const command_list& commandList) {
            m_drawCommands.emplace_back(commandList);
        }
    } // namespace opengl
} // namespace engine
} // namespace tst
