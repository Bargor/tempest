// This file is part of Tempest-application project
// Author: Karol Kontny
#pragma once

#include <common/rectangle.h>
#include <cstdint>
#include <memory>

template<typename T, typename Deleter = std::default_delete<T>>
using ptr = std::unique_ptr<T, Deleter>;

namespace tst {
namespace application {

    struct image_data {
        core::extent<std::uint32_t> imageSize;
        std::int32_t channels;
        std::size_t memorySize;
        ptr<unsigned char, void (*)(void*)> data;
    };

} // namespace application
} // namespace tst