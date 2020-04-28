// This file is part of Tempest-core project
// Author: Karol Kontny

#pragma once

#include <assert.h>
#include <macros.h>
#include <platform.h>

#include <memory>

template<typename T, typename Deleter = std::default_delete<T>>
using ptr = std::unique_ptr<T, Deleter>;