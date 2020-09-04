// This file is part of Tempest project
// Author: Karol Kontny
#pragma once

#include "engine-impl/api.h"
#include "engine-impl/resources/settings.h"

namespace tst {
namespace application {
    class data_loader;
}
namespace engine {

    struct settings final : public api::settings {
        using super = api::settings;

    public:
        settings(api::settings&& impl);

    private:
    };

    settings parse_engine_settings(const application::data_loader& dataLoader);

    static_assert(!std::is_polymorphic_v<settings>);
    static_assert(sizeof(settings) == sizeof(api::settings));

} // namespace engine
} // namespace tst
