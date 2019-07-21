// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <engine-impl/api.h>
#include <engine-impl/engine.h>

#include <engine/device.h>

namespace tst {
namespace engine {

    class device;

    class rendering_engine {
    public:

        rendering_engine(application::main_window& mainWindow,
                         application::data_loader& dataLoader,
                         application::event_processor<application::app_event>& eventProcessor);
        ~rendering_engine();

        void frame(size_t frameCount);
        void start();
        void stop();
        device& get_GPU() noexcept;

    private:
        api::rendering_engine m_engineImpl;
        device m_device;
    };

} // namespace engine
} // namespace tst