#pragma once

namespace Varia
{

constexpr size_t kilobytes_to_bytes(size_t KB) { return KB * 1024; }
constexpr size_t megabytes_to_bytes(size_t MB) { return MB * 1024 * 1024; }
constexpr size_t gigabytes_to_bytes(size_t GB) { return GB * 1024 * 1024 * 1024; }

}