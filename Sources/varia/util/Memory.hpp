#pragma once

#include "varia/vcommon.hpp"

constexpr size_t varia_memory_kilobytes_to_bytes(size_t KB) { return KB * 1000; }
constexpr size_t varia_memory_megabytes_to_bytes(size_t MB) { return MB * 1000 * 1000; }
constexpr size_t varia_memory_gigabytes_to_bytes(size_t GB) { return GB * 1000 * 1000 * 1000; }

constexpr size_t VARIA_MEMORY_CONSTANTS_DEFAULT_ALIGNMENT = 16;

