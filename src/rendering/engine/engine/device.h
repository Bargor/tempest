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

    class device {
        friend class resource_factory;
    public:
        device(api::device& deviceImpl);
        ~device();

    private:
        api::device& m_deviceImpl;
    };

}
}