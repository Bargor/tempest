// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "object_controller.h"

#include "scene.h"

#include <application/data_loader.h>
#include <application/event_processor.h>
#include <engine/resource_factory.h>

namespace tst {
namespace scene {

    engine::base::pipeline_settings create_pipeline_settings() {
        engine::base::viewport_settings viewportSettings{0, 0, 640, 525, 0.0f, 1.0f};
        core::rectangle<std::int32_t, std::uint32_t> scissorSettings{{0, 0}, {640, 525}};
        engine::base::rasterizer_settings rasterizerSettings{false,
                                                             false,
                                                             engine::base::rasterizer_settings::polygon_mode::fill,
                                                             engine::base::rasterizer_settings::cull_mode::back,
                                                             engine::base::rasterizer_settings::front_face::clockwise,
                                                             1.0f,
                                                             {false, 0.0f, 0.0f, 0.0f}};
        engine::base::multisampling_settings multisamplingSettings{
            false, engine::base::multisampling_settings::sample_count::samples_1};
        engine::base::color_blending_settings blendingSettings{
            false,
            engine::base::color_blending_settings::blend_operation::add,
            engine::base::color_blending_settings::blend_factor::one,
            engine::base::color_blending_settings::blend_factor::zero,
            engine::base::color_blending_settings::blend_operation::add,
            engine::base::color_blending_settings::blend_factor::one,
            engine::base::color_blending_settings::blend_factor::zero,
            {true, true, true, true}};
        engine::base::global_blending_settings globalBlendingSettings{
            false, engine::base::global_blending_settings::logic_operation::copy, {0.0f, 0.0f, 0.0f, 0.0f}};

        return engine::base::pipeline_settings(viewportSettings,
                                               scissorSettings,
                                               rasterizerSettings,
                                               multisamplingSettings,
                                               {blendingSettings},
                                               globalBlendingSettings);
    }

    object_controller::object_controller(scene& scene,
                                         application::data_loader& dataLoader,
                                         application::event_processor<application::app_event>& eventProcessor,
                                         engine::resource_factory& resourceFactory)
        : m_scene(scene), m_dataLoader(dataLoader), m_eventProcessor(eventProcessor), m_resourceFactory(resourceFactory) {
    }

    scene_object object_controller::load_object(const std::string&) {
        auto vertexFormat = engine::vertex_format{};

        auto vertexBuffer =
            m_resourceFactory.create_vertex_buffer(vertexFormat,
                                                   std::vector<engine::vertex>({{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                                                                                {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
                                                                                {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
                                                                                {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}}));
        auto indexBuffer = m_resourceFactory.create_index_buffer(std::vector<std::uint16_t>({{0, 1, 2, 2, 3, 0}}));

        auto material = m_resourceFactory.create_material();

        auto pipeline = m_resourceFactory.create_pipeline("test", "test", vertexFormat);

        scene_object object(std::move(vertexBuffer), std::move(indexBuffer), std::move(material), pipeline);
        m_scene.add_object(std::move(object));

        return object;
    }

} // namespace scene
} // namespace tst
