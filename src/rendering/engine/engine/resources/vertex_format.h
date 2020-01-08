// This file is part of Tempest-engine project
// Author: Karol Kontny

#pragma once

#include <engine-impl/api.h>
#include <engine-impl/resources/vertex_format.h>
#include <type_traits>

namespace tst {
namespace engine {

    class vertex_format final : private api::vertex_format {
    public:
        using super = api::vertex_format;
        using primitive_topology = base::vertex_format::primitive_topology;

    public:
        vertex_format(vertex_format::primitive_topology topology);
        ~vertex_format();

    public:
        void add_attribute(
            location location, format format, std::uint32_t offset, std::uint32_t stride, std::uint32_t divisor);
        const std::vector<attribute_descriptor>& get_attribute_descriptors() const noexcept;
        primitive_topology get_topology() const noexcept;

    private:
    };

    static_assert(!std::is_polymorphic_v<vertex_format>);
    static_assert(sizeof(vertex_format) == sizeof(api::vertex_format));

    TST_INLINE void vertex_format::add_attribute(
        location location, format format, std::uint32_t offset, std::uint32_t stride, std::uint32_t divisor) {
        super::add_attribute(location, format, offset, stride, divisor);
    }

    TST_INLINE const std::vector<vertex_format::super::attribute_descriptor>& vertex_format::get_attribute_descriptors() const
        noexcept {
        return super::get_attribute_descriptors();
    }

    TST_INLINE vertex_format::super::primitive_topology vertex_format::get_topology() const noexcept {
        return super::get_topology();
    }

} // namespace engine
} // namespace tst