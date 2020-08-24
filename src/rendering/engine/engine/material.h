// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "resources/texture.h"
#include "resources/uniform_buffer.h"

#include <concepts>
#include <engine-impl/api.h>
#include <engine-impl/resources/material.h>
#include <type_traits>

namespace tst {
namespace engine {

    class material : private api::material {
    public:
        material(api::material&& pipelineImpl);
        ~material() = default;

        material(material&& other) noexcept;

    public:
        const api::material& to_super() const noexcept {
            return *this;
        }
    };

    static_assert(!std::is_polymorphic_v<material>);
    static_assert(sizeof(material) == sizeof(api::material));

    template<typename T>
    concept MaterialType = std::derived_from<T, material>&& requires {
        typename T::StaticStorageType;
        typename T::DynamicStorageType;
    };
} // namespace engine
} // namespace tst
