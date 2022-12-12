#pragma once

#include "varia/vcommon.hpp"
#include "varia/validation.hpp"
#include "varia/ds/Result.hpp"

namespace vds
{

struct Bits32
{
	u32 storage = 0;

	constexpr Bits32(u32 value) : storage{value} {}
	constexpr Bits32(void) {}

	constexpr void and(Bits32 other) { this->storage &= other.storage; }
	constexpr void or(Bits32 other) { this->storage |= other.storage; }
	constexpr void not(void) { this->storage = ~this->storage; }
	constexpr void unset(u8 bit_index) { this->storage &= ~(1 << bit_index); }
	constexpr void unset_all(void) { this->storage = 0; }
	constexpr void set(u8 bit_index) { this->storage |= (1 << bit_index); }
	constexpr void set_all(void) { this->unset_all(); this->storage = ~this->storage; }
	constexpr void toggle(u8 bit_index) { this->storage ^= (1 << bit_index); }

	constexpr bool is_set(u8 bit_index) { return this->storage & (1 << bit_index); }
	constexpr bool is_unset(u8 bit_index) { return !is_set(bit_index); }

	constexpr void rotate_left(u8 shift)
 	{ 
		assert(shift < 32 && "Exceeded bitwidth during shift.");

		this->storage <<= shift; 
	}

	constexpr void rotate_right(u8 shift) 
	{ 
		assert(shift < 32 && "Exceeded bitwidth during shift.");
		this->storage >>= shift; 
	}

	constexpr void generate_bitmask_lo(u8 pivot)
	{
		assert(pivot < 32 && "Exceeded bitwidth during shift.");

		//We want to start with a zero value for that a negation results in all bits being set
		this->storage = 0;
		this->storage = (1 << pivot) - 1;
	}

	constexpr void generate_bitmask_hi(u8 pivot)
	{
		assert(pivot < 32 && "Exceeded bitwidth during shift.");

		//Note(zshoals): View the high bitmask as being generated from a left shift!
		//Not from shifting to the right!
		this->generate_bitmask_lo(pivot);
		this->not();
	}

	constexpr void increment_upper(u8 pivot)
	{
		assert(pivot < 32 && "Exceeded bitwidth during shift.");

		//Clear the lower bits and work only with the upper bits of the value
		Bits32 shifted_upper = *this;
		{
			shifted_upper.rotate_right(pivot + 1);
			
			assert( (1ULL << (31 - pivot + 1)) && "Upper range bit overflow.");

			++(shifted_upper.storage);
			shifted_upper.rotate_left(pivot + 1);
		}

		//Clear the upper bits and work with the lower bits of the value
		Bits32 shifted_lower = *this;
		{
			Bits32 lower_mask = {};
			
			lower_mask.generate_bitmask_lo(pivot);
			shifted_lower.mask_allow(lower_mask);
		}

		//Remerge the upper and lower portions of the value after incrementing
		//the upper bit range
		shifted_upper.or(shifted_lower);

		this->storage = shifted_upper.storage;
	}

	constexpr void mask_allow(Bits32 mask)
	{
		this->and(mask);
	}

	constexpr void mask_deny(Bits32 mask)
	{
		mask.not();
		this->and(mask);
	}

	constexpr vds::Result<u8> find_first_set(void)
	{
		//Note(zshoals): Bit "Find First Set" implementation
		//derived from wikipedia https://en.wikipedia.org/wiki/Find_first_set#FFS
		u32 x = this->storage;

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

	constexpr vds::Result<u8> find_first_unset(void)
	{
		Bits32 inverse = *this;
		inverse.not();
		return inverse.find_first_set();
	}	

	constexpr static u8 pow2_to_bitshift_value(size_t pow2_value)
	{
		u8 shift = 0;
		while (pow2_value > 1)
		{
			pow2_value /= 2;
			++shift;
		}

		return shift;
	}

	constexpr static size_t pow2_next_nearest(size_t value)
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
};

struct Bits64
{
	u64 storage = 0;

	constexpr Bits64(u64 value) : storage{value}{}
	constexpr Bits64(void){}

	constexpr void and(Bits64 other) { this->storage &= other.storage; }
	constexpr void or(Bits64 other) { this->storage |= other.storage; }
	constexpr void not(void) { this->storage = ~this->storage; }
	constexpr void set(u8 bit_index) { this->storage |= (1ULL << bit_index); }
	constexpr void set_all(void) { this->unset_all(); this->storage = ~this->storage; }
	constexpr void unset(u8 bit_index) { this->storage &= ~(1ULL << bit_index); }
	constexpr void toggle(u8 bit_index) { this->storage ^= (1ULL << bit_index); }
	constexpr void unset_all(void) { this->storage = 0; }

	constexpr bool is_set(u8 bit_index) { return this->storage & (1ULL << bit_index); }
	constexpr bool is_unset(u8 bit_index) { return !is_set(bit_index); }

	constexpr void rotate_left(u8 shift)
 	{ 
		assert(shift < 64 && "Exceeded bitwidth during shift.");
		this->storage <<= shift; 
	}

	constexpr void rotate_right(u8 shift) 
	{ 
		assert(shift < 64 && "Exceeded bitwidth during shift.");
		this->storage >>= shift; 
	}

	constexpr void generate_bitmask_lo(u8 pivot)
	{
		assert(pivot < 64 && "Exceeded bitwidth during shift.");

		this->storage = 0;
		this->storage = (1 << pivot) - 1;
	}

	constexpr void generate_bitmask_hi(u8 pivot)
	{
		assert(pivot < 64 && "Exceeded bitwidth during shift.");

		//Note(zshoals): View the high bitmask as being generated from a left shift!
		//Not from shifting to the right!
		this->generate_bitmask_lo(pivot);
		this->not();
	}

	constexpr void increment_upper(u8 pivot)
	{
		assert(pivot < 64 && "Exceeded bitwidth during shift.");

		//Clear the lower bits and work only with the upper bits of the value
		Bits64 shifted_upper = *this;
		{
			shifted_upper.rotate_right(pivot + 1);
			
			assert( (1ULL << (63 - pivot + 1)) && "Upper range bit overflow.");

			shifted_upper.storage += 1;
			shifted_upper.rotate_left(pivot + 1);
		}

		//Clear the upper bits and work with the lower bits of the value
		Bits64 shifted_lower = *this;
		{
			// shifted_lower.rotate_left(31 - pivot);
			// shifted_lower.rotate_right(31 - pivot);
			Bits64 lower_mask = {};
			
			lower_mask.generate_bitmask_lo(pivot);
			shifted_lower.mask_allow(lower_mask);
		}

		//Remerge the upper and lower portions of the value after incrementing
		//the upper bit range
		shifted_upper.or(shifted_lower);

		this->storage = shifted_upper.storage;
	}

	constexpr void mask_allow(Bits64 mask)
	{
		this->and(mask);
	}

	constexpr void mask_deny(Bits64 mask)
	{
		mask.not();
		this->and(mask);
	}

	constexpr vds::Result<u8> find_first_set(void)
	{
		vds::Result<u8> res = {};

		//TODO(zshoals): Update with Count Leading Zeroes version
		for_range_var(i, 64)
		{
			if (this->is_set(static_cast<u8>(i)))
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
};

}