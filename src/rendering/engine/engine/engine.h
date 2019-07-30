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

        template<typename Iter>
        bool draw_frame(Iter first, Iter last);

    private:
        device m_device;
    };

    template<typename Iter>
    bool rendering_engine::draw_frame(Iter first, Iter last) {
        super::draw_frame(first, last);

        return true;
    }

    static_assert(!std::is_polymorphic_v<rendering_engine>);

} // namespace engine
} // namespace tst