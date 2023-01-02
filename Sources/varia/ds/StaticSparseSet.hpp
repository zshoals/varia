#pragma once

#include "StaticArray.hpp"

#include "varia/vcommon.hpp"

namespace vds
{

template <typename T, int KeyRange, int ElementCount>
struct StaticSparseSet
{
	static_assert(ElementCount <= KeyRange, "StaticSparseSet contains more elements than keys, making excess elements inaccessible.");

	StaticArray<u32, KeyRange> sparse_keys;
	StaticArray<u32, ElementCount> dense_keys;
	StaticArray<T, ElementCount> data;
	u32 push_idx;


	auto begin(void) { return data.begin(); }
	auto end(void) { return data.end(); }

	void initialize(void)
	{
		this->data.initialize();
		this->dense_keys.initialize();

		this->sparse_keys.initialize();
		this->sparse_keys.set_all(UINT32_MAX);

		this->push_idx = 0;
	}

	bool has(u32 key)
	{
		return (*sparse_keys.get(key) != UINT32_MAX);
	}

	void add(u32 key, T value)
	{
		if (has(key)) return;

		dense_keys.push(key);
		sparse_keys.set(key, this->push_idx);
		++this->push_idx;
		data.push(value);
	}

	void remove(u32 key)
	{
		if (!has(key)) return;

		u32 idx_of_dense_ent = *sparse_keys.get(key);
		sparse_keys.set(key, UINT32_MAX);

		dense_keys.swap_and_pop(idx_of_dense_ent);
		data.swap_and_pop(idx_of_dense_ent);

		--this->push_idx;
	}

};

}