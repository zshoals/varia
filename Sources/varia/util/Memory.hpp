#pragma once

namespace Varia
{

namespace Memory
{

constexpr size_t kilobytes_to_bytes(size_t KB) { return KB * 1000; }
constexpr size_t megabytes_to_bytes(size_t MB) { return MB * 1000 * 1000; }
constexpr size_t gigabytes_to_bytes(size_t GB) { return GB * 1000 * 1000 * 1000; }

constexpr size_t default_alignment = 16;

}
}