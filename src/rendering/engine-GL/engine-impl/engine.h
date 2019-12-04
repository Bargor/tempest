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

        class engine_frontend {
            template<typename T>
            using ptr = std::unique_ptr<T>;

        public:
            engine_frontend(application::data_loader& dataLoader,
                            application::event_processor<application::app_event>&,
                            device& device);
            ~engine_frontend();

            void frame(size_t frameCount);
            void start();
            void stop();

            template<typename Iter>
            bool draw_frame(Iter first, Iter last);

        private:
            application::data_loader& m_dataLoader;
            device& m_device;
        };

        template<typename Iter>
        bool engine_frontend::draw_frame(Iter first, Iter last) {
            auto commandLists = prepare_draw(first, last);

            for (auto& commandList : commandLists) {
                m_device.submit_command_list(commandList);
            }

            return true;
        }

    } // namespace opengl
} // namespace engine
} // namespace tst
