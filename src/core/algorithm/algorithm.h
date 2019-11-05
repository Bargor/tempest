// This file is part of Tempest-core project
// Author: Karol Kontny

#pragma once

#include <algorithm>
#include <platform.h>

namespace tst {

template<typename InputIt1, typename InputIt2, typename Pred>
bool includes(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, Pred pred) {
    for (; first2 != last2; ++first2) {
        auto it = first1;
        for (; it != last1; ++it) {
            if (pred(*it, *first2)) {
                break;
            }
        }
        if (it == last1) {
            return false;
        }
    }
    return true;
}
}
