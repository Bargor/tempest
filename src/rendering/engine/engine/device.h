// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "resources/index_buffer.h"
#include "resources/uniform_buffer.h"
#include "resources/vertex_buffer.h"

#include <engine-impl/api.h>
#include <engine-impl/device.h>

namespace tst {
namespace engine {

    class device final : public api::device {
        friend class resource_factory;
    public:
        using super = api::device;

        device(api::device& deviceImpl);
        ~device();

    private:
    };


    static_assert(sizeof(device) == sizeof(api::device));
}
}