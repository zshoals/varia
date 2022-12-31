#pragma once

#include "StaticRingbuf.hpp"

namespace vds
{

template <typename T, typename Context, int Size>
struct FillExecuteContextual
{
	vds::StaticRingbuf<T, Size> buffer;
	void (*callback)(FillExecuteContextual<T, Context, Size> * self,  Context * ctx);
	Context ctx;

	//Note(zshoals 12-30-2022):> I'm gonna be honest, I have no idea what types these are
	//lazy way out...
	auto begin(void) { return buffer.begin(); }
	auto end(void) { return buffer.end(); }

	void initialize(void (*cb)(FillExecuteContextual<T, Context, Size> * self, Context * ctx), Context context)
	{
		this->ctx = context;
		this->callback = cb;
		buffer.initialize();
	}

	void push(T element)
	{
		if (this->buffer.is_almost_full())
		{
			this->buffer.push_back(element);
			callback(this, &this->ctx);
			this->buffer.clear();
		}
		else
		{
			this->buffer.push_back(element);
		}

	}
};

template <typename T, int Size>
struct FillExecute
{
	vds::StaticRingbuf<T, Size> buffer;
	void (*callback)(vds::FillExecute<T, Size> * self);

	//Note(zshoals 12-30-2022):> I'm gonna be honest, I have no idea what types these are
	//lazy way out...
	auto begin(void) { return buffer.begin(); }
	auto end(void) { return buffer.end(); }

	void initialize(void (*cb)(vds::FillExecute<T, Size> * self))
	{
		this->callback = cb;
		buffer.initialize();
	}

	void push(T element)
	{
		if (this->buffer.is_almost_full())
		{
			this->buffer.push_back(element);
			callback(this);
			this->buffer.clear();
		}
		else
		{
			this->buffer.push_back(element);
		}

	}
};

}