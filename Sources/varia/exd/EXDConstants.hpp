#pragma once

#include "varia/vcommon.hpp"
#include "EXDUtil.hpp"

namespace exd
{
namespace Constants
{

	constexpr size_t exd_max_entities = 8192;
	constexpr size_t exd_max_components = 512;
	constexpr u8 exd_id_shift = EXDUtil::id_shift(exd_max_entities);

}
}