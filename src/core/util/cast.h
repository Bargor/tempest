// This file is part of Tempest-core project
// Author: Karol Kontny

#pragma once

#include <vector>

namespace tst {
namespace core {

    template<typename To, typename From>
    std::vector<To>& vector_cast(const std::vector<From>& vector) {
        return *(std::vector<To>*)&vector;
    }

}
} // namespace tst