// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "api.h"
#include "command_list.h"
#include "resource_factory.h"
#include "resources/index_buffer.h"
#include "resources/settings.h"
#include "resources/uniform_buffer.h"
#include "resources/vertex_buffer.h"

#include <GLFW/glfw3.h>
#include <memory>
#include <vector>

namespace tst {
namespace application {
    class data_loader;
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
                   application::event_processor<application::app_event>& eventProcessor,
                   settings&& engineSettings);
            device(const device& device) = delete;
            ~device();

        public:
            resource_factory create_resource_factory(const application::data_loader& dataLoader) const;

            gpu_info& get_GPU_info() const noexcept;
            void submit_command_list(const command_list& commandList);

            template<typename Iter>
            bool draw_frame(Iter first, Iter last);

            void start();
            void stop();

        private:
            ptr<gpu_info> m_gpuInfo;
            std::vector<command_list> m_drawCommands;
        };

        template<typename Iter>
        bool device::draw_frame(Iter, Iter) {
            return true;
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
