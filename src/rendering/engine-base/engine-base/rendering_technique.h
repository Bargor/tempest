// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "settings.h"

#include <core.h>
#include <string>
#include <optional>

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
            enum class type { color, depth };
            enum class load_operation { load, clear, dont_care };
            enum class store_operation { store, dont_care };

            sample_count samples;
            load_operation attachmentLoadOperation;
            store_operation attachementStoreOperation;
            load_operation stencilLoadOperation;
            store_operation stencilStoreOperation;
            image_layout initialLayout;
            image_layout finalLayout;
        };

        struct subpass_settings {
            enum class bind_point {graphics, compute};

            bind_point bindPoint;
            std::vector<std::uint32_t> colorAttachments;
            std::optional<std::uint32_t> depthAttachment;
        };

        struct dependency {
            std::uint32_t srcSubpassId;
            std::uint32_t dstSubpassId;
            std::size_t srcStageMask;
            std::size_t dstStageMask;
            std::size_t srcAccessMask;
            std::size_t dstAccessMask;
        };

        struct technique_settings {
            std::vector<framebuffer_settings> m_framebuffers;
            std::vector<subpass_settings> m_subpasses;
            std::vector<dependency> m_depencencies;
        };

        class rendering_technique {
        public:
            rendering_technique(std::string&& techniqueName);

            const std::string& get_name() const;

        protected:
            std::string m_techniqueName;
        };

        TST_INLINE const std::string& rendering_technique::get_name() const {
            return m_techniqueName;
        }
    } // namespace base
} // namespace engine
} // namespace tst

namespace std {} // namespace std
