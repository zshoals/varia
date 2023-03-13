#pragma once

#include <stdint.h>

enum class vds_option_status_e
{
	None,
	Some
};

template <typename T>
struct vds_option_t
{
	vds_option_status_e _has;
	T _data;

	// explicit vds_option_t(T value) : _data{value}, _has{vds_option_status_e::Some} {}
	// explicit vds_option_t()
	// {
	// 	this->_has = vds_option_status_e::None;
	// }

	T & operator*()
	{
		return this->_data;
	}

	T & unwrap()
	{
		return this->_data;
	}

	operator bool() const
	{
		return (bool)this->_has;
	}

	bool state()
	{
		return (bool)(this->_has);
	}


};

template <typename T>
vds_option_t<T> vds_option_create_some(T value)
{
	vds_option_t<T> out;
	out._data = value;
	out._has = vds_option_status_e::Some;

	return out;
}

template <typename T>
vds_option_t<T> vds_option_create_none(void)
{
	vds_option_t<T> out;
	out._has = vds_option_status_e::None;

	return out;
}
