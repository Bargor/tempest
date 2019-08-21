// This file is part of Tempest-engine-GL project
// Author: Karol Kontny
#pragma once

#include "command_list.h"
#include "device.h"

#include <memory>
#include <vector>

namespace tst {
namespace application {
    struct app_event;
    class data_loader;
    template<typename Event>
    class event_processor;
    class main_window;
} // namespace application

namespace engine {

    namespace opengl {

        class shader_compiler;

        class rendering_engine {
            template<typename T>
            using ptr = std::unique_ptr<T>;

        public:
            rendering_engine(application::data_loader& dataLoader,
                             application::event_processor<application::app_event>&,
                             device& device);
            ~rendering_engine();

            void frame(size_t frameCount);
            void start();
            void stop();

            template<typename Iter>
            std::vector<command_list> prepare_draw(Iter first, Iter last);
            template<typename Iter>
            bool draw_frame(Iter first, Iter last);

        private:
            application::data_loader& m_dataLoader;
            device& m_device;
            ptr<shader_compiler> m_shaderCompiler;
        };

        template<typename Iter>
        std::vector<command_list> rendering_engine::prepare_draw(Iter, Iter) {
            std::vector<command_list> commands;

            /*for (; first != last; ++first) {
                auto& drawItem = *first;
                buffers.emplace_back(generate_command_buffer(drawItem));
            }*/

            return commands;
        }

        template<typename Iter>
        bool rendering_engine::draw_frame(Iter first, Iter last) {
            auto commandLists = prepare_draw(first, last);

            for (auto& commandList : commandLists) {
                m_device.submit_command_list(commandList);
            }

            m_device.draw();

            return true;
        }

    } // namespace opengl
} // namespace engine
} // namespace tst
