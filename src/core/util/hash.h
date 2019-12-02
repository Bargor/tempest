// This file is part of Tempest-core project
// Author: Karol Kontny

#pragma once

#include "core.h"

#include <functional>

namespace tst {

TST_INLINE void hash_combine(size_t& seed, size_t hash) {
    hash += 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= hash;
}
} // namespace tst
