#pragma once

#include "StaticArray.hpp"

#include "varia/vcommon.hpp"

template <typename T, int KeyRange, int ElementCount>
struct vds_sparse_set_t
{
	static_assert(ElementCount <= KeyRange, "StaticSparseSet contains more elements than keys, making excess elements inaccessible.");

	vds_array_t<u32, KeyRange> sparse_keys;
	vds_array_t<u32, ElementCount> dense_keys;
	vds_array_t<T, ElementCount> data;
	u32 push_idx;

	auto begin(void) { return data.begin(); }
	auto end(void) { return data.end(); }
};

template <typename T, int KeyRange, int ElementCount>
void vds_sparse_set_initialize(vds_sparse_set_t<T, KeyRange, ElementCount> * self)
{
	vds_array_initialize(&self->data);
	vds_array_initialize(&self->dense_keys);
	vds_array_initialize(&self->sparse_keys);

	vds_array_set_all(&self->sparse_keys, UINT32_MAX);

	self->push_idx = 0;
}

template <typename T, int KeyRange, int ElementCount>
bool vds_sparse_set_has(vds_sparse_set_t<T, KeyRange, ElementCount> * self, u32 key)
{
	return ( (*vds_array_get(&self->sparse_keys, key)) != UINT32_MAX );
}

template <typename T, int KeyRange, int ElementCount>
void vds_sparse_set_add(vds_sparse_set_t<T, KeyRange, ElementCount> * self, u32 key, T value)
{
	if (vds_sparse_set_has(self, key)) return;

	vds_array_push(&self->dense_keys, key);
	vds_array_set(&self->sparse_keys, key, self->push_idx);
	++self->push_idx;
	vds_array_push(&self->data, value);
}

template <typename T, int KeyRange, int ElementCount>
void vds_sparse_set_remove(vds_sparse_set_t<T, KeyRange, ElementCount> * self, u32 key)
{
	if (!vds_sparse_set_has(self, key)) return;

	u32 idx_of_dense_ent = *vds_array_get(&self->sparse_keys, key);
	vds_array_set(&self->sparse_keys, key, UINT32_MAX);

	vds_array_swap_and_pop(&self->dense_keys, idx_of_dense_ent);
	vds_array_swap_and_pop(&self->data, idx_of_dense_ent);

	--self->push_idx;
}
