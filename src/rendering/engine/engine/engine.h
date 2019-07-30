// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <engine-impl/api.h>
#include <engine-impl/engine.h>

#include <engine/device.h>
#include <engine/draw_info.h>

namespace tst {
namespace engine {

    class device;

    class rendering_engine final : api::rendering_engine {
    public:
        using super = api::rendering_engine;

        rendering_engine(application::main_window& mainWindow,
                         application::data_loader& dataLoader,
                         application::event_processor<application::app_event>& eventProcessor);
        ~rendering_engine();

        void frame(size_t frameCount);
        void start();
        void stop();
        device& get_GPU() noexcept;
        bool draw_frame(std::vector<draw_info>&& infos);

    private:
        device m_device;
    };

} // namespace engine
} // namespace tst