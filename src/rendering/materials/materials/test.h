// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <engine/material.h>

namespace tst {
namespace materials {
    class test_material : public engine::material {
    public:
        using StaticStorageType = std::nullptr_t;
        using DynamicStorageType = std::nullptr_t;
    };
}
} // namespace tst