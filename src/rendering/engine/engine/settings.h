// This file is part of Tempest project
// Author: Karol Kontny
#pragma once

#include "engine-impl/api.h"
#include "engine-impl/resources/settings.h"
#include "engine-impl/settings_parser.h"

namespace tst {
namespace engine {

    class settings final : public api::settings {
        using super = api::settings;

    public:
        settings(api::settings&& impl);

    private:
    };

    static_assert(!std::is_polymorphic_v<settings>);
    static_assert(sizeof(settings) == sizeof(api::settings));

} // namespace engine
} // namespace tst
