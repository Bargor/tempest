// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "device.h"
#include "resources/index_buffer.h"
#include "resources/pipeline.h"
#include "resources/uniform_buffer.h"
#include "resources/vertex_buffer.h"
#include "resources/vertex_format.h"

namespace tst {

namespace application {
    class data_loader;
}

namespace engine {

    namespace opengl {
        class device;

        class resource_factory {
        public:
            resource_factory(device& device, application::data_loader& dataLoader);
            ~resource_factory();

        public:
            template<typename IndexType>
            index_buffer<IndexType> create_index_buffer(std::vector<std::uint16_t>&& indices);
            const pipeline& create_pipeline(const std::string& techniqueName,
                                            const std::string& shadersName,
                                            const vertex_format& format);
            void create_technique(std::string&& name, base::technique_settings&& settings);
            vertex_buffer create_vertex_buffer(const vertex_format& format, std::vector<vertex>&& vertices);
            uniform_buffer create_uniform_buffer();

        private:
            device& m_device;
            application::data_loader& m_dataLoader;
        };

        template<typename IndexType>
        index_buffer<IndexType> resource_factory::create_index_buffer(std::vector<std::uint16_t>&& indices) {
            return m_device.create_index_buffer(std::move(indices));
        }
    }
}
}