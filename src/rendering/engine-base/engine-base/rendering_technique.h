// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "settings.h"

#include <array>
#include <core.h>
#include <optional>
#include <string>
#include <vector>

namespace tst {
namespace engine {
    namespace base {

        enum class image_layout {
            undefined,
            general,
            color_attachment_optimal,
            depth_stencil_attachment_optimal,
            present
        };

        struct framebuffer_settings {
            enum class attachment_type { color, depth };
            enum class load_operation { load, clear, dont_care };
            enum class store_operation { store, dont_care };

            std::uint32_t id;
            attachment_type type;
            sample_count samples;
            load_operation attachmentLoadOperation;
            store_operation attachementStoreOperation;
            load_operation stencilLoadOperation;
            store_operation stencilStoreOperation;
            image_layout initialLayout;
            image_layout finalLayout;
            std::array<float, 4> clearValue;
        };

        struct subpass_settings {
            enum class bind_point { graphics, compute };

            bind_point bindPoint;
            std::vector<std::uint32_t> colorAttachments;
            std::optional<std::uint32_t> depthAttachment;
        };

        struct dependency {
            std::uint32_t srcSubpassId;
            std::uint32_t dstSubpassId;
            std::uint32_t srcStageMask;
            std::uint32_t dstStageMask;
            std::uint32_t srcAccessMask;
            std::uint32_t dstAccessMask;
        };

        struct technique_settings {
            std::vector<framebuffer_settings> m_framebuffers;
            std::vector<subpass_settings> m_subpasses;
            std::vector<dependency> m_depencencies;
        };

        class rendering_technique {
        public:
            rendering_technique(std::string&& techniqueName, technique_settings&& settings);

            std::string_view get_name() const;

        protected:
            std::string m_techniqueName;
            technique_settings m_settings;
        };

        TST_INLINE std::string_view rendering_technique::get_name() const {
            return m_techniqueName;
        }
    } // namespace base
} // namespace engine
} // namespace tst

namespace std {} // namespace std
