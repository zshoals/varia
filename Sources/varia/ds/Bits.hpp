#pragma once

#include "varia/vcommon.hpp"
#include "varia/validation.hpp"
#include "varia/ds/Result.hpp"



//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//||                          Bits32                                     ||
//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||

constexpr inline u32 vds_bits32_and(u32 a, u32 b) { return a &= b; }
constexpr inline u32 vds_bits32_or(u32 a, u32 b) { return a |= b; }
constexpr inline u32 vds_bits32_not(u32 bits) { return ~bits; }
constexpr inline u32 vds_bits32_unset(u32 bits, u8 bit_index) { return bits &= ~(1 << bit_index); }
constexpr inline u32 vds_bits32_unset_all(u32 bits) { return 0; }
constexpr inline u32 vds_bits32_set(u32 bits, u8 bit_index) { return bits |= (1 << bit_index); }
constexpr inline u32 vds_bits32_set_all(u32 bits) { return ~0; }
constexpr inline u32 vds_bits32_toggle(u32 bits, u8 bit_index) { return bits ^= (1 << bit_index); }

constexpr inline bool vds_bits32_is_set(u32 bits, u8 bit_index) { return bits & (1 << bit_index); }
constexpr inline bool vds_bits32_is_unset(u32 bits, u8 bit_index) { return !vds_bits32_is_set(bits, bit_index); }

constexpr u32 vds_bits32_rotate_left(u32 bits, u8 shift)
{ 
	assert(shift < 32 && "Exceeded bitwidth during shift.");

	return bits <<= shift; 
}

constexpr u32 vds_bits32_rotate_right(u32 bits, u8 shift) 
{ 
	assert(shift < 32 && "Exceeded bitwidth during shift.");
	return bits >>= shift; 
}

constexpr u32 vds_bits32_generate_bitmask_lo(u8 pivot)
{
	assert(pivot < 32 && "Exceeded bitwidth during shift.");

	return ( (1 << pivot) - 1 );
}

constexpr u32 vds_bits32_generate_bitmask_hi(u8 pivot)
{
	assert(pivot < 32 && "Exceeded bitwidth during shift.");

	//Note(zshoals): View the high bitmask as being generated from a left shift!
	//Not from shifting to the right!
	u32 bitmask = vds_bits32_generate_bitmask_lo(pivot);
	return vds_bits32_not(bitmask);
}

constexpr u32 vds_bits32_increment_upper(u32 bits, u8 pivot)
{
	assert(pivot < 32 && "Exceeded bitwidth during shift.");

	//Clear the lower bits and work only with the upper bits of the value
	u32 shifted_upper = bits;
	{
		shifted_upper = vds_bits32_rotate_right(shifted_upper, pivot);
		
		assert( (1ULL << (31 - pivot + 1)) && "Upper range bit overflow.");

		++shifted_upper;

		shifted_upper = vds_bits32_rotate_left(shifted_upper, pivot);
	}

	//Clear the upper bits and work with the lower bits of the value
	u32 shifted_lower = bits;
	{
		u32 lower_mask = vds_bits32_generate_bitmask_lo(pivot);
		
		shifted_lower = vds_bits32_mask_allow(shifted_lower, lower_mask);
	}

	//Remerge the upper and lower portions of the value after incrementing
	//the upper bit range
	return vds_bits32_or(shifted_upper, shifted_lower);
}

constexpr u32 vds_bits32_mask_allow(u32 bits, u32 mask)
{
	return vds_bits32_and(bits, mask);
}

constexpr u32 vds_bits32_mask_deny(u32 bits, u32 mask)
{
	mask = vds_bits32_not(mask);
	return vds_bits32_and(bits, mask);
}

constexpr vds::Result<u8> vds_bits32_find_first_set(u32 bits)
{
	//Note(zshoals): Bit "Find First Set" implementation
	//derived from wikipedia https://en.wikipedia.org/wiki/Find_first_set#FFS
	u32 x = bits;

	//Note(zshoals): FFS implementations do whatever they want when the value is 0
	//We do a branchless check to determine if this value is 0,
	//and then multiply "first_set" by the comparison's result in order to
	//set a default value (0) for res.value
	//This value determines whether or not the result is valid
	bool zero_case_fixup = (x != 0);

	//Note(zshoals): Negating x is intentional
	//Disables:
	//"Warning: Unary minus operator applied to unsigned type, result still unsigned"
	#pragma warning(push)
	#pragma warning(disable: 4146)

	x &= -x;

	#pragma warning(pop)

	u32 r = (x > 0xFFFF) << 4; x >>= r;
	u32 q = (x > 0xFF) << 3; x >>= q; r |= q;
	q = (x > 0xF) << 2; x >>= q; r |= q;
	q = (x > 0x3) << 1; x >>= q; r |= q;
	r |= (x >> 1);

	u32 first_set = r * zero_case_fixup;

	vds::Result<u8> res = {};
	res.valid = static_cast<vds::ResultStatus_e>(zero_case_fixup);
	res.value = first_set;

	return res;
}

constexpr vds::Result<u8> vds_bits32_find_first_unset(u32 bits)
{
	u32 inverse = bits;
	inverse = vds_bits32_not(inverse);
	return vds_bits32_find_first_set(inverse);
}	

constexpr static u8 vds_bits32_pow2_to_bitshift_value(size_t pow2_value)
{
	u8 shift = 0;
	while (pow2_value > 1)
	{
		pow2_value /= 2;
		++shift;
	}

	return shift;
}

constexpr static size_t vds_bits32_pow2_next_nearest(size_t value)
{
	--value;
	value |= (value >> 1);
	value |= (value >> 2);
	value |= (value >> 4);
	value |= (value >> 8);
	value |= (value >> 16);
	++value;

	return value;
}












//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//||                        Bits 64                                      ||
//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||

constexpr inline u64 vds_bits64_and(u64 a, u64 b) { return a &= b; }
constexpr inline u64 vds_bits64_or(u64 a, u64 b) { return a |= b; }
constexpr inline u64 vds_bits64_not(u64 bits) { return ~bits; }
constexpr inline u64 vds_bits64_unset(u64 bits, u8 bit_index) { return bits &= ~(1 << bit_index); }
constexpr inline u64 vds_bits64_unset_all(u64 bits) { return 0; }
constexpr inline u64 vds_bits64_set(u64 bits, u8 bit_index) { return bits |= (1 << bit_index); }
constexpr inline u64 vds_bits64_set_all(u64 bits) { return ~0; }
constexpr inline u64 vds_bits64_toggle(u64 bits, u8 bit_index) { return bits ^= (1 << bit_index); }

constexpr inline bool vds_bits64_is_set(u64 bits, u8 bit_index) { return bits & (1 << bit_index); }
constexpr inline bool vds_bits64_is_unset(u64 bits, u8 bit_index) { return !vds_bits64_is_set(bits, bit_index); }

constexpr u64 vds_bits64_rotate_left(u64 bits, u8 shift)
{ 
	assert(shift < 64 && "Exceeded bitwidth during shift.");
	return bits <<= shift;
}

constexpr u64 vds_bits64_rotate_right(u64 bits, u8 shift) 
{ 
	assert(shift < 64 && "Exceeded bitwidth during shift.");
	return bits >>= shift;
}

constexpr u64 vds_bits64_generate_bitmask_lo(u8 pivot)
{
	assert(pivot < 64 && "Exceeded bitwidth during shift.");
	return ( (1 << pivot) - 1 );
}

constexpr u64 vds_bits64_generate_bitmask_hi(u8 pivot)
{
	assert(pivot < 64 && "Exceeded bitwidth during shift.");

	//Note(zshoals): View the high bitmask as being generated from a left shift!
	//Not from shifting to the right!
	u64 bitmask = vds_bits64_generate_bitmask_lo(pivot);
	return vds_bits64_not(bitmask);
}

//TODO(zshoals 01-31-2023):> This is currently 64bits only and not in the 32bit version, why? Lazy?
constexpr u64 vds_bits64_set_upper(u64 bits, u64 new_upper_value, u8 pivot)
{
	assert(pivot < 64 && "Exceeded bitwidth during shift.");

	u64 himask = vds_bits64_generate_bitmask_hi(pivot);
	u64 cleared_upper_bits = vds_bits64_mask_deny(bits, himask);
	u64 new_upper_value_rotated_left = vds_bits64_rotate_left(new_upper_value, pivot);
	return vds_bits64_or(cleared_upper_bits, new_upper_value_rotated_left);
}

constexpr u64 vds_bits64_set_lower(u64 bits, u64 new_lower_value, u8 pivot)
{
	assert(pivot < 64 && "Exceeded bitwidth during shift.");
	assert(new_lower_value < (1ULL << pivot) && "Setting lower value would overrun bitrange.");  

	u64 lomask = vds_bits64_generate_bitmask_lo(pivot);
	u64 cleared_lower_bits = vds_bits64_mask_deny(bits, lomask);
	return vds_bits64_or(cleared_lower_bits, new_lower_value);
}

constexpr u64 vds_bits64_increment_upper(u64 bits, u8 pivot)
{
	assert(pivot < 64 && "Exceeded bitwidth during shift.");

	//Clear the lower bits and work only with the upper bits of the value
	u64 shifted_upper = bits;
	{
		shifted_upper = vds_bits64_rotate_right(shifted_upper, pivot);
		
		assert( (1ULL << (63 - pivot + 1)) && "Upper range bit overflow.");

		++shifted_upper;

		shifted_upper = vds_bits64_rotate_left(shifted_upper, pivot);
	}

	//Clear the upper bits and work with the lower bits of the value
	u64 shifted_lower = bits;
	{
		u64 lower_mask = vds_bits64_generate_bitmask_lo(pivot);
		shifted_lower = vds_bits64_mask_allow(shifted_lower, lower_mask);
	}

	//Remerge the upper and lower portions of the value after incrementing
	//the upper bit range
	return vds_bits64_or(shifted_upper, shifted_lower);
}

constexpr u64 vds_bits64_mask_allow(u64 bits, u64 mask)
{
	return vds_bits64_and(bits, mask);
}

constexpr u64 vds_bits64_mask_deny(u64 bits, u64 mask)
{
	u64 inverse_mask = vds_bits64_not(mask);
	return vds_bits64_and(bits, mask);
}

constexpr vds::Result<u8> find_first_set(u64 bits)
{
	vds::Result<u8> res = {};

	//TODO(zshoals): Update with Count Leading Zeroes version
	for_range_var(i, 64)
	{
		if (vds_bits64_is_set(bits, static_cast<u8>(i)))
		{
			res.valid = vds::ResultStatus_e::Success;
			res.value = static_cast<u8>(i);

			return res;
		}
	}

	res.valid = vds::ResultStatus_e::Failure;
	res.value = 0;

	return res;
}







//Bit Tricks (zshoals:> Started Dec-14-2022)

		//Count set bits
		// x -= ((x >> 1) & 0x55555555);
		// x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
		// x = (x + (x >> 4)) & 0x0f0f0f0f;
		// x = (x * 0x01010101) >> 24;