// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "resources/index_buffer.h"
#include "resources/uniform_buffer.h"
#include "resources/vertex_buffer.h"

#include <GLFW/glfw3.h>
#include <memory>
#include <vector>

namespace tst {
namespace engine {
    namespace opengl {

        class gpu_info;

		class device {
            template<typename T>
            using ptr = std::unique_ptr<T>;
        public:
            device(GLFWwindow* window);
            device(const device& device) = delete;
            ~device();

        template<typename IndexType>
        index_buffer<IndexType> create_index_buffer(std::vector<IndexType>&& indices) const;
        vertex_buffer create_vertex_buffer(const vertex_format& format,
                                               std::vector<vertex>&& vertices) const;
        uniform_buffer create_uniform_buffer() const;

        gpu_info& get_GPU_info() const noexcept;

        private:
            ptr<gpu_info> m_gpuInfo;
		};

        template<typename IndexType>
        index_buffer<IndexType> device::create_index_buffer(std::vector<IndexType>&& indices) const {
            return index_buffer<std::uint16_t>(std::move(indices));
        }

        TST_INLINE gpu_info& device::get_GPU_info() const noexcept {
            return *m_gpuInfo;
        }
	}
}
}