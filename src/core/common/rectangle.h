// This file is part of Tempest-core project
// Author: Karol Kontny

#pragma once

#include "core.h"
#include "position.h"
#include "util/hash.h"

#include <cstdint>

namespace tst {
namespace core {

    template<typename T>
    struct extent {
        T width;
        T height;

        bool operator==(const extent& other) const noexcept;
    };

    template<typename T, typename S>
    struct rectangle {
        position<T> offset;
        extent<S> dimensions;

        bool operator==(const rectangle& other) const noexcept;
    };

    template<typename T>
    bool extent<T>::operator==(const extent& other) const noexcept {
        if (width == other.width && height && other.height) {
            return true;
        }
        return false;
    }

    template<typename T, typename S>
    TST_INLINE bool rectangle<T, S>::operator==(const rectangle& other) const noexcept {
        if (offset == other.offset && dimensions == other.dimensions) {
            return true;
        }
        return false;
    }

} // namespace core
} // namespace tst

namespace std {

template<typename T>
struct hash<tst::core::extent<T>> {
    std::size_t operator()(const tst::core::extent<T>& extent) {
        size_t seed = 0;
        tst::hash_combine(seed, hash<T>{}(extent.width));
        tst::hash_combine(seed, hash<T>{}(extent.height));

        return seed;
    }
};

template<typename T, typename S>
struct hash<tst::core::rectangle<T, S>> {
    std::size_t operator()(const tst::core::rectangle<T, S>& rectangle) {
        size_t seed = 0;
        tst::hash_combine(seed, hash<tst::core::position<T>>{}(rectangle.offset));
        tst::hash_combine(seed, hash<tst::core::extent<S>>{}(rectangle.dimensions));

        return seed;
    }
};

} // namespace std
