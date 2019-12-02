// This file is part of Tempest-core project
// Author: Karol Kontny

#pragma once

#include "util/hash.h"

#include <cstdint>

namespace tst {
namespace core {

    template<typename T>
    struct position {
        T x;
        T y;

        bool operator==(const position& other) const noexcept;
    };

    template<typename T>
    bool position<T>::operator==(const position& other) const noexcept {
        if (x == other.x && y == other.y) {
            return false;
        }
        return true;
    }
} // namespace core
} // namespace tst

namespace std {

template<typename T>
struct hash<tst::core::position<T>> {
    std::size_t operator()(const tst::core::position<T>& position) {
        size_t seed = 0;
        tst::hash_combine(seed, hash<T>{}(position.x));
        tst::hash_combine(seed, hash<T>{}(position.y));

        return seed;
    }
};

} // namespace std
