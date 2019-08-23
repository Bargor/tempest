// This file is part of Tempest-core project
// Author: Karol Kontny

#pragma once

#include "position.h"

#include <cstdint>

namespace tst {
namespace core {

    template<typename T>
    struct extent {
        T width;
        T height;
    };

    template<typename T, typename S>
    struct rectangle {
        position<T> offset;
        extent<S> dimensions;
    };

} // namespace core
} // namespace tst