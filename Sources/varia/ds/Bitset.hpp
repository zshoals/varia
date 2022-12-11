#pragma once

#include "Bits.hpp"

#include "varia/ds/Result.hpp"

#include "varia/vcommon.hpp"
#include "varia/validation.hpp"

namespace vds
{

template<int Size>
struct Bitset32
{
	//VVVVVVVV Utility VVVVVVVV 
	constexpr static size_t ceil_next_nearest(size_t value, size_t target_multiple)
	{
		bool round_up = (value % target_multiple == 0) ? false : true;
		return (((target_multiple - (value % target_multiple)) * round_up) + value);
	}

	constexpr static size_t bit_width(void)
	{
		return 32;
	}

	constexpr static size_t true_size(void)
	{
		return ceil_next_nearest(Size, Bitset32::bit_width()) / Bitset32::bit_width();
	}
	//^^^^^^^^ End Utility ^^^^^^^^  




	static_assert(Size > 0, "Bitset element count must be greater than 0");
	//Note(zshoals): We ceil our total size up to the next full u32 block
	vds::Bits32 data[true_size()];

	void and(Bitset32<Size> const & other)
	{
		for_range_var(i, true_size())
		{
			this->data[i].and(other.data[i]);
		}
	}

	void or(Bitset32<Size> const & other)
	{
		for_range_var(i, true_size())
		{
			this->data[i].or(other.data[i]);
		}
	}

	void not(void)
	{
		for_range_var(i, true_size())
		{
			this->data[i].not();
		}
	}

	void set(size_t target_bit)
	{
		DEBUG_ENSURE_UINT_LT(target_bit, Size, "Out of range bit in Bitset.");

		size_t block = compute_block_index(target_bit);
		u8 bit_offset = compute_bit_index(target_bit);

		this->data[block].set(bit_offset);
	}

	void unset(size_t target_bit)
	{
		DEBUG_ENSURE_UINT_LT(target_bit, Size, "Out of range bit in Bitset.");

		size_t block = compute_block_index(target_bit);
		u8 bit_offset = compute_bit_index(target_bit);

		this->data[block].unset(bit_offset);
	}

	void toggle(size_t target_bit)
	{
		DEBUG_ENSURE_UINT_LT(target_bit, Size, "Out of range bit in Bitset.");

		size_t block = compute_block_index(target_bit);
		u8 bit_offset = compute_bit_index(target_bit);

		this->data[block].toggle(bit_offset);
	}

	void set_all(void)
	{
		for_range_var(i, true_size())
		{
			this->data[i].set_all();
		}
	}

	void unset_all(void)
	{
		for_range_var(i, true_size())
		{
			this->data[i].unset_all();
		}
	}

	bool is_set(size_t target_bit)
	{
		DEBUG_ENSURE_UINT_LT(target_bit, Size, "Out of range bit in Bitset.");

		size_t block = compute_block_index(target_bit);
		u8 bit_offset = compute_bit_index(target_bit);

		return this->data[block].is_set(bit_offset);
	}

	bool is_unset(size_t target_bit)
	{
		size_t block = compute_block_index(target_bit);
		u8 bit_offset = compute_bit_index(target_bit);

		return !this->is_set(target_bit);
	}

	u8 find_first_set(size_t target_block)
	{
		//We need to get this one working a little better
		ENSURE_UNIMPLEMENTED();
	}

	size_t storage_bit_count(void)
	{
		return ceil_next_nearest(Size, Bitset32::bit_width());
		// return Size * Bitset32::bit_width();
	}

	size_t user_bit_count(void)
	{
		return Size;
	}

	vds::Result<size_t> find_first_set(void)
	{
		vds::Result<size_t> res;

		for_range_var(i, true_size())
		{
			size_t block_idx = Bitset32::compute_block_index(i);
			vds::Result<u8> bit_search = this->data[block_idx].find_first_set();
			if (bit_search.valid)
			{
				res.valid = vds::ResultStatus_e::Success;
				res.value = bit_search.value * block_idx;

				return res;
			}
		}

		res.valid = vds::ResultStatus_e::Failure;
		res.value = 0;

		return res;
	}

	static size_t compute_block_index(size_t target_bit)
	{
		DEBUG_ENSURE_UINT_LT((target_bit / 32), true_size(), "Out of range bit in Bitset.");

		//Note(zshoals Dec-11-2022): There's no guarantee that the bitset is a power of two,
		//we don't enforce that. We have to use a simple division instead.
		return target_bit / Bitset32::bit_width();
	}

	static u8 compute_bit_index(size_t target_bit)
	{
		DEBUG_ENSURE_UINT_LT(target_bit, Size, "Out of range bit in Bitset.");

		//Note(zshoals): Equivalent to target_bit % bit_width, however, this is faster
		//in debug builds
		constexpr size_t shift = Bits32::pow2_to_bitshift_value(Bitset32::bit_width());
		return target_bit & ( (1 << shift) - 1);
	}

};

}



