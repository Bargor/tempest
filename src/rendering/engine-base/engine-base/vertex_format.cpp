// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "vertex_format.h"

namespace tst {
namespace engine {

    namespace base {

        vertex_format::vertex_format() {
        }

        void vertex_format::add_attribute(
            location location, format format, std::uint32_t offset, std::uint32_t stride, std::uint32_t divisor) {
            m_attributes.emplace_back(attribute_descriptor{location, format, offset, stride, divisor});
        }

        const std::vector<vertex_format::attribute_descriptor>& vertex_format::get_atttibutes() const noexcept {
            return m_attributes;
        }
    } // namespace base

} // namespace engine
} // namespace tst
