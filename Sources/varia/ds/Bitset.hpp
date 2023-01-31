#pragma once

#include "Bits.hpp"

#include "varia/ds/Result.hpp"

#include "varia/vcommon.hpp"
#include "varia/validation.hpp"


//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//||                Utility functions                                    ||
//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||


constexpr static inline size_t vds_bitset32_unitlocal_ceil_next_nearest(size_t value, size_t target_multiple)
{
	bool round_up = (value % target_multiple == 0) ? false : true;
	return (((target_multiple - (value % target_multiple)) * round_up) + value);
}

constexpr static inline size_t vds_bitset32_unitlocal_bit_width(void)
{
	return 32;
}

constexpr static inline size_t vds_bitset32_unitlocal_true_size(size_t size)
{
	size_t bitwidth = vds_bitset32_unitlocal_bit_width();
	//Note(zshoals 01-31-2023):> No, the double bitwidth is not an error. Read it carefully.
	return vds_bitset32_unitlocal_ceil_next_nearest(size, bitwidth) / bitwidth;
}



//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//||             Core Bitset32 API                                       ||
//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||

template<int Size>
struct vds_bitset32_t
{

	static_assert(Size > 0, "Bitset element count must be greater than 0");
	//Note(zshoals): We ceil our total size up to the next full u32 block
	u32 data[vds_bitset32_unitlocal_true_size(Size)];
};




template<int Size>
void vds_bitset32_initialize(vds_bitset32_t<Size> * self)
{
	VARIA_ZERO_INIT(self);
}

template<int Size>
void vds_bitset32_and(vds_bitset32_t<Size> * self, vds_bitset32_t<Size> * other)
{
	for_range_var(i, vds_bitset32_unitlocal_true_size(Size))
	{
		self->data[i] = vds_bits32_and(self->data[i], other->data[i]);
	}
}

template<int Size>
void vds_bitset32_or(vds_bitset32_t<Size> * self, vds_bitset32_t<Size> * other)
{
	for_range_var(i, vds_bitset32_unitlocal_true_size(Size))
	{
		self->data[i] = vds_bits32_or(self->data[i], other->data[i]);
	}
}

template<int Size>
void vds_bitset32_not(vds_bitset32_t<Size> * self)
{
	for_range_var(i, vds_bitset32_unitlocal_true_size(Size))
	{
		self->data[i] = vds_bits32_not(self->data[i]);
	}
}

template<int Size>
void vds_bitset32_andNot(vds_bitset32_t<Size> * self, vds_bitset32_t<Size> * other)
{
	for_range_var(i, vds_bitset32_unitlocal_true_size(Size))
	{
		u32 inverse = vds_bits32_not(other->data[i]);
		self->data[i] = vds_bits32_and(self->data[i], inverse);
	}
}

template<int Size>
void vds_bitset32_set(vds_bitset32_t<Size> * self, size_t target_bit)
{
	DEBUG_ENSURE_UINT_LT(target_bit, Size, "Out of range bit in Bitset.");

	size_t block = vds_bitset32_compute_block_index(target_bit);
	u8 bit_offset = vds_bitset32_compute_bit_index(target_bit);

	self->data[block] = vds_bits32_set(self->data[block], bit_offset);
}

template<int Size>
void vds_bitset32_unset(vds_bitset32_t<Size> * self, size_t target_bit)
{
	DEBUG_ENSURE_UINT_LT(target_bit, Size, "Out of range bit in Bitset.");

	size_t block =  vds_bitset32_compute_block_index(target_bit);
	u8 bit_offset = vds_bitset32_compute_bit_index(target_bit);

	self->data[block] = vds_bits32_unset(self->data[block], bit_offset);
}

template<int Size>
void vds_bitset32_toggle(vds_bitset32_t<Size> * self, size_t target_bit)
{
	DEBUG_ENSURE_UINT_LT(target_bit, Size, "Out of range bit in Bitset.");

	size_t block =  vds_bitset32_compute_block_index(target_bit);
	u8 bit_offset = vds_bitset32_compute_bit_index(target_bit);

	self->data[block] = vds_bits32_toggle(self->data[block], bit_offset);
}

template<int Size>
void vds_bitset32_set_all(vds_bitset32_t<Size> * self)
{
	for_range_var(i, vds_bitset32_unitlocal_true_size(Size))
	{
		self->data[i] = vds_bits32_set_all(0);
	}
}

template<int Size>
void vds_bitset32_unset_all(vds_bitset32_t<Size> * self)
{
	for_range_var(i, Bitset32Util::true_size(Size))
	{
		self->data[i] = vds_bits32_unset_all(0);
	}
}

template<int Size>
bool vds_bitset32_is_set(vds_bitset32_t<Size> * self, size_t target_bit)
{
	DEBUG_ENSURE_UINT_LT(target_bit, Size, "Out of range bit in Bitset.");

	size_t block =  vds_bitset32_compute_block_index(target_bit);
	u8 bit_offset = vds_bitset32_compute_bit_index(target_bit);

	return vds_bits32_is_set(self->data[block], bit_offset);
}

template<int Size>
bool vds_bitset32_is_unset(vds_bitset32_t<Size> * self, size_t target_bit)
{
	return !vds_bitset32_is_set(self, target_bit);
}

template<int Size>
size_t vds_bitset32_storage_bit_count(void)
{
	return vds_bitset32_unitlocal_ceil_next_nearest(Size, vds_bitset32_unitlocal_bit_width());
	// return Size * Bitset32::bit_width();
}

template<int Size>
size_t vds_bitset32_user_bit_count(void)
{
	return Size;
}

template<int Size>
vds_result_t<size_t> vds_bitset32_find_first_set(vds_bitset32_t<Size> * self)
{
	vds_result_t<size_t> res;

	for_range_var(i, vds_bitset32_unitlocal_true_size(Size))
	{
		vds_result_t<u8> bit_search = vds_bits32_find_first_set(self->data[i]);
		size_t found_bit = bit_search.value + (vds_bitset32_unitlocal_bit_width() * i);

		//Note(zshoals Dec-11-2022): found_bit must compare against size
		//even in find_first_set (despite being unable to set OOB values)
		//because it's possible that the bitset will be inverted and 
		//bits outside the bitset's public facing size could wind up set
		if (bit_search.valid && found_bit < Size)
		{
			res.valid = VDS_RESULT_STATUS_SUCCESS_E;
			res.value = found_bit;

			return res;
		}
	}

	res.valid = VDS_RESULT_STATUS_FAILURE_E;;
	res.value = 0;

	return res;
}

template<int Size>
vds_result_t<size_t> vds_bitset32_find_first_unset(vds_bitset32_t<Size> * self)
{
	vds_result_t<size_t> res;

	for_range_var(i, vds_bitset32_unitlocal_true_size(Size))
	{
		//Note(zshoals Dec-11-2022): The only difference between this and find_first_set
		//is this inverse.find_first_unset() call here
		vds_result_t<u8> bit_search = vds_bits32_find_first_unset(self->data[i]);
		size_t found_bit = bit_search.value + (vds_bitset32_unitlocal_bit_width() * i);

		//Note(zshoals Dec-11-2022): First, see note in "find_first_set"
		//Second, we must compare against "found_bit" and "Size" because
		//it's possible that the bits out of the user selectable range
		//are affected by various operations inadvertently. 
		if (bit_search.valid && found_bit < Size)
		{
			res.valid = VDS_RESULT_STATUS_SUCCESS_E;
			res.value = found_bit;

			return res;
		}
	}

	res.valid = VDS_RESULT_STATUS_FAILURE_E;
	res.value = 0;

	return res;
}


template<int Size>
size_t vds_bitset32_compute_block_index(size_t target_bit)
{
	DEBUG_ENSURE_UINT_LT((target_bit / 32), vds_bitset32_unitlocal_true_size(Size), "Out of range bit in Bitset.");

	//Note(zshoals Dec-11-2022): There's no guarantee that the bitset is a power of two,
	//we don't enforce that. We have to use a simple division instead.
	return target_bit / vds_bitset32_unitlocal_bit_width();
}



template<int Size>
u8 vds_bitset32_compute_bit_index(size_t target_bit)
{
	DEBUG_ENSURE_UINT_LT(target_bit, Size, "Out of range bit in Bitset.");

	//Note(zshoals Dec-11-2022): Equivalent to target_bit % bit_width, however, this is faster
	//in debug builds
	size_t shift = vds_bits32_pow2_to_bitshift_value(vds_bitset32_unitlocal_bit_width());
	return target_bit & ( (1 << shift) - 1);
}