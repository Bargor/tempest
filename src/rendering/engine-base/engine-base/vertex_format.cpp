// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "vertex_format.h"

namespace tst {
namespace engine {

    namespace base {

        vertex_format::vertex_format(primitive_topology topology) : m_topology(topology) {
        }

        void vertex_format::add_attribute(
            location location, format format, std::uint32_t offset, std::uint32_t stride, std::uint32_t divisor) {
            m_attributes.emplace_back(attribute_descriptor{location, format, offset, stride, divisor});
        }

        const std::vector<vertex_format::attribute_descriptor>& vertex_format::get_attibute_descriptors() const noexcept {
            return m_attributes;
        }

        vertex_format::primitive_topology vertex_format::get_topology() const noexcept {
            return m_topology;
        }

    } // namespace base

} // namespace engine
} // namespace tst
