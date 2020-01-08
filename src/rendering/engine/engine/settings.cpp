// This file is part of Tempest project
// Author: Karol Kontny

#include "settings.h"

namespace tst {
namespace engine {

    settings::settings(api::settings&& impl) : api::settings(std::move(impl)) { 
    }
}
} // namespace tst