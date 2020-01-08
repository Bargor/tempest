// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "settings.h"

#include <engine-impl/api.h>
#include <engine-impl/device.h>
#include <type_traits>

namespace tst {
namespace application {
    class data_loader;
    template<typename Event>
    class event_processor;
    class main_window;

    struct app_event;
} // namespace application
namespace engine {

    class device final : public api::device {
        friend class resource_factory;
        using super = api::device;
    public:

        device(application::main_window& mainWindow,
               application::event_processor<application::app_event>& eventProcessor,
               settings&& settings);
        ~device();

    private:
    };

    static_assert(!std::is_polymorphic_v<device>);
    static_assert(sizeof(device) == sizeof(api::device));
} // namespace engine
} // namespace tst
