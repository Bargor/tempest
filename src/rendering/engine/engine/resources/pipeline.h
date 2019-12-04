// This file is part of Tempest project
// Author: Karol Kontny
#pragma once

#include <engine-impl/api.h>
#include <engine-impl/resources/pipeline.h>

namespace tst {
namespace engine {
    namespace resources {

        class pipeline : public api::pipeline {
        public:
            using super = api::pipeline;

            pipeline(api::pipeline&& pipelineImpl);
            ~pipeline();

        private:
        };

        static_assert(sizeof(pipeline) == sizeof(api::pipeline));
        static_assert(!std::is_polymorphic_v<pipeline>);

    } // namespace resources
} // namespace engine
} // namespace tst
