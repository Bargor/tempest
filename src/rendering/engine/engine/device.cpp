// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "device.h"

namespace tst {
namespace engine {
    device::device(api::device& deviceImpl) : m_deviceImpl(deviceImpl) {
    }

    device::~device() {
    }

} // namespace engine
} // namespace tst