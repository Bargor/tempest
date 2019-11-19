// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "object_controller.h"

#include "scene.h"

#include <application/app_event.h>
#include <application/data_loader.h>
#include <application/event_processor.h>
#include <engine/resource_factory.h>
#include <util/variant.h>

namespace tst {
namespace scene {

    engine::base::technique_settings create_technique_settings() {
        engine::base::viewport_settings viewportSettings{0, 0, 840, 525, 0.0f, 1.0f};
        core::rectangle<std::int32_t, std::uint32_t> scissorSettings{{0, 0}, {640, 525}};
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

        return engine::base::technique_settings{viewportSettings,
                                                scissorSettings,
                                                std::vector<engine::base::color_blending_settings>{blendingSettings},
                                                globalBlendingSettings};
    }

    object_controller::object_controller(scene& scene,
                                         application::data_loader& dataLoader,
                                         application::event_processor<application::app_event>& eventProcessor,
                                         engine::resource_factory& resourceFactory)
        : m_scene(scene), m_dataLoader(dataLoader), m_eventProcessor(eventProcessor), m_resourceFactory(resourceFactory) {
        auto framebuffer_callback = [](const application::app_event::arguments& args) {
            assert(std::holds_alternative<application::app_event::iconify>(args));
        };

        m_eventProcessor.subscribe(
            core::variant_index<application::app_event::arguments, application::app_event::framebuffer>(),
            this,
            std::move(framebuffer_callback));
    }

    void object_controller::load_object(const std::string&) {
        auto vertexFormat = engine::vertex_format(engine::base::vertex_format::primitive_topology::triangle_list);

        m_resourceFactory.create_technique("test", create_technique_settings());
        const auto& pipeline = m_resourceFactory.create_pipeline("test", "test", vertexFormat);

        auto vertexBuffer =
            m_resourceFactory.create_vertex_buffer(vertexFormat,
                                                   std::vector<engine::vertex>({{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                                                                                {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
                                                                                {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
                                                                                {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}}));
        auto indexBuffer = m_resourceFactory.create_index_buffer(std::vector<std::uint16_t>({{0, 1, 2, 2, 3, 0}}));

        auto material = m_resourceFactory.create_material();

        scene_object object(std::move(vertexBuffer), std::move(indexBuffer), std::move(material), pipeline);
        m_scene.add_object(std::move(object));
    }

} // namespace scene
} // namespace tst
