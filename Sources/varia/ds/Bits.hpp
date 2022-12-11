#pragma once

#include "varia/vcommon.hpp"
#include "varia/validation.hpp"
#include "varia/ds/Result.hpp"

namespace vds
{

struct Bits32
{
	u32 storage = 0;

	void and(Bits32 other) { this->storage &= other.storage; }
	void or(Bits32 other) { this->storage |= other.storage; }
	void not(void) { this->storage = ~this->storage; }
	void unset(u8 bit_index) { this->storage &= ~(1 << bit_index); }
	void unset_all(void) { this->storage = 0; }
	void set(u8 bit_index) { this->storage |= (1 << bit_index); }
	void set_all(void) { this->unset_all(); this->storage = ~this->storage; }
	void toggle(u8 bit_index) { this->storage ^= (1 << bit_index); }

	bool is_set(u8 bit_index) { return this->storage & (1 << bit_index); }
	bool is_unset(u8 bit_index) { return !is_set(bit_index); }

	void rotate_left(u8 shift)
 	{ 
		DEBUG_ENSURE_UINT_LT(shift, 32, "Exceeded bitwidth during shift."); 

		this->storage <<= shift; 
	}

	void rotate_right(u8 shift) 
	{ 
		DEBUG_ENSURE_UINT_LT(shift, 32, "Exceeded bitwidth during shift."); 

		this->storage >>= shift; 
	}

	void generate_bitmask_lo(u8 pivot)
	{
		DEBUG_ENSURE_UINT_EQUALS(this->storage, 0, "A generated bitmask must start with a zero value.");
		DEBUG_ENSURE_UINT_LT(pivot, 32, "Exceeded bitwidth during shift."); 

		//We want to start with a zero value for that a negation results in all bits being set
		this->not();

		u8 to_cut = 31 - pivot;
		this->rotate_left(to_cut);
		this->rotate_right(to_cut);
	}

	void generate_bitmask_hi(u8 pivot)
	{
		DEBUG_ENSURE_UINT_EQUALS(this->storage, 0, "A generated bitmask must start with a zero value.");
		DEBUG_ENSURE_UINT_LT(pivot, 32, "Exceeded bitwidth during shift."); 

		//Note(zshoals): View the high bitmask as being generated from a left shift!
		//Not from shifting to the right!
		this->generate_bitmask_lo(pivot);
		this->not();
	}

	void increment_upper(u8 pivot)
	{
		DEBUG_ENSURE_UINT_LT(pivot, 32, "Exceeded bitwidth during shift."); 

		//Clear the lower bits and work only with the upper bits of the value
		Bits32 shifted_upper = *this;
		{
			shifted_upper.rotate_right(pivot);
			
			DEBUG_ENSURE_UINT_LT(shifted_upper.storage, (1ULL << (31 - pivot + 1)), "Upper range bit overflow");

			++(shifted_upper.storage);
			shifted_upper.rotate_left(pivot);
		}

		//Clear the upper bits and work with the lower bits of the value
		Bits32 shifted_lower = *this;
		{
			Bits32 lower_mask = {};
			
			//Note(zshoals Dec-11-2022): Pivot is not inclusive, lower bits get ripped off here
			lower_mask.generate_bitmask_lo(pivot - 1);
			shifted_lower.mask_allow(lower_mask);
		}

		//Remerge the upper and lower portions of the value after incrementing
		//the upper bit range
		shifted_upper.or(shifted_lower);

		this->storage = shifted_upper.storage;
	}

	void mask_allow(Bits32 mask)
	{
		this->and(mask);
	}

	void mask_deny(Bits32 mask)
	{
		mask.not();
		this->and(mask);
	}

	vds::Result<u8> find_first_set(void)
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

		vds::Result<u8> res;
		res.valid = static_cast<vds::ResultStatus_e>(zero_case_fixup);
		res.value = first_set;

		return res;
	}

	vds::Result<u8> find_first_unset(void)
	{
		Bits32 inverse = *this;
		inverse.not();
		return inverse.find_first_set();
	}	

	constexpr static size_t pow2_to_bitshift_value(size_t pow2_value)
	{
		size_t shift = 0;
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

	void and(Bits64 other) { this->storage &= other.storage; }
	void or(Bits64 other) { this->storage |= other.storage; }
	void not(void) { this->storage = ~this->storage; }
	void set(u8 bit_index) { this->storage |= (1ULL << bit_index); }
	void set_all(void) { this->unset_all(); this->storage = ~this->storage; }
	void unset(u8 bit_index) { this->storage &= ~(1ULL << bit_index); }
	void toggle(u8 bit_index) { this->storage ^= (1ULL << bit_index); }
	void unset_all(void) { this->storage = 0; }

	bool is_set(u8 bit_index) { return this->storage & (1ULL << bit_index); }
	bool is_unset(u8 bit_index) { return !is_set(bit_index); }

	void rotate_left(u8 shift)
 	{ 
		DEBUG_ENSURE_UINT_LT(shift, 64, "Exceeded bitwidth during shift."); 

		this->storage <<= shift; 
	}

	void rotate_right(u8 shift) 
	{ 
		DEBUG_ENSURE_UINT_LT(shift, 64, "Exceeded bitwidth during shift."); 

		this->storage >>= shift; 
	}

	void generate_bitmask_lo(u8 pivot)
	{
		DEBUG_ENSURE_UINT_EQUALS(this->storage, 0, "A generated bitmask must start with a zero value.");
		DEBUG_ENSURE_UINT_LT(pivot, 64, "Exceeded bitwidth during shift."); 

		//We want to start with a zero value for that a negation results in all bits being set
		this->not();

		u8 to_cut = 63 - pivot;
		this->rotate_left(to_cut);
		this->rotate_right(to_cut);
	}

	void generate_bitmask_hi(u8 pivot)
	{
		DEBUG_ENSURE_UINT_EQUALS(this->storage, 0, "A generated bitmask must start with a zero value.");
		DEBUG_ENSURE_UINT_LT(pivot, 64, "Exceeded bitwidth during shift."); 

		//Note(zshoals): View the high bitmask as being generated from a left shift!
		//Not from shifting to the right!
		this->generate_bitmask_lo(pivot);
		this->not();
	}

	void increment_upper(u8 pivot)
	{
		DEBUG_ENSURE_UINT_LT(pivot, 64, "Exceeded bitwidth during shift."); 

		//Clear the lower bits and work only with the upper bits of the value
		Bits64 shifted_upper = *this;
		{
			shifted_upper.rotate_right(pivot);
			
			DEBUG_ENSURE_UINT_LT(shifted_upper.storage, (1ULL << (63 - pivot + 1)), "Upper range bit overflow");

			shifted_upper.storage += 1;
			shifted_upper.rotate_left(pivot);
		}

		//Clear the upper bits and work with the lower bits of the value
		Bits64 shifted_lower = *this;
		{
			// shifted_lower.rotate_left(31 - pivot);
			// shifted_lower.rotate_right(31 - pivot);
			Bits64 lower_mask = {};
			
			//Note(zshoals Dec-11-2022): Pivot is not inclusive, lower bits get ripped off here
			lower_mask.generate_bitmask_lo(pivot - 1);
			shifted_lower.mask_allow(lower_mask);
		}

		//Remerge the upper and lower portions of the value after incrementing
		//the upper bit range
		shifted_upper.or(shifted_lower);

		this->storage = shifted_upper.storage;
	}

	void mask_allow(Bits64 mask)
	{
		this->and(mask);
	}

	void mask_deny(Bits64 mask)
	{
		mask.not();
		this->and(mask);
	}

	vds::Result<u8> find_first_set(void)
	{
		vds::Result<u8> res;

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