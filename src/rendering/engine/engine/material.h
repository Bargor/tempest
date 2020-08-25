// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "resources/texture.h"
#include "resources/uniform_buffer.h"

#include <engine-impl/api.h>
#include <engine-impl/resources/material.h>
#include <type_traits>

#ifndef __clang__
#include <concepts>
#else
namespace std {
template<class Derived, class Base>
concept derived_from =
    std::is_base_of_v<Base, Derived> && std::is_convertible_v<const volatile Derived*, const volatile Base*>;
}
#endif

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
