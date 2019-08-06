// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "resources/index_buffer.h"
#include "resources/uniform_buffer.h"
#include "resources/vertex_buffer.h"

#include <engine-impl/api.h>
#include <engine-impl/device.h>
#include <type_traits>

namespace tst {
namespace application {
    class main_window;
}
namespace engine {

    class device final : public api::device {
        friend class resource_factory;
    public:
        using super = api::device;

        device(application::main_window& mainWindow);
        ~device();

    private:
    };

    static_assert(!std::is_polymorphic_v<device>);
    static_assert(sizeof(device) == sizeof(api::device));
}
}