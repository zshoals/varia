#pragma once

#include "StaticRingbuf.hpp"



//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//||                   Fill Execute WITH Context                         ||
//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
template <typename T, typename Context, int Size>
struct vds_fill_execute_contextual_t
{
	// static_assert(Size > 1, "A FillExecuteContextual construct must have at least 2 elements.");

	vds_ringbuf_t<T, Size> buffer;
	void (*callback)(FillExecuteContextual<T, Context, Size> * self,  Context * ctx);
	Context ctx;

	//Note(zshoals 12-30-2022):> I'm gonna be honest, I have no idea what types these are
	//lazy way out...
	auto begin(void) { return buffer.begin(); }
	auto end(void) { return buffer.end(); }
};

template<typename T, typename Context, int Size>
void vds_fill_execute_contextual_initialize
(
	vds_fill_execute_contextual_t<T, Context, Size> * self, 
	void (*cb)(vds_fill_execute_contextual_t<T, Context, Size> * self, Context * ctx),
	Context context
)
{
	vds_ringbuf_initialize(&self->buffer);
	self->callback = cb;
	self->ctx = context;
}

template<typename T, typename Context, int Size>
void vds_fill_execute_contextual_push(vds_fill_execute_contextual_t<T, Context, Size> * self, T element)
{
	if (vds_ringbuf_is_almost_full(&self->buffer))
	{
		vds_ringbuf_push_back(&self->buffer, element);
		callback(self, &self->ctx);
		vds_ringbuf_clear(&self->buffer);
	}
	else
	{
		vds_ringbuf_push_back(&self->buffer, element);
	}
}





//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
//||                  Fill Execute                                       ||
//||_____________________________________________________________________||
//||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||
template <typename T, int Size>
struct vds_fill_execute_t
{
	// static_assert(Size > 1, "A FillExecute construct must have at least 2 elements.");

	vds_ringbuf_t<T, Size> buffer;
	void (*callback)(vds_fill_execute_t<T, Size> * self);

	//Note(zshoals 12-30-2022):> I'm gonna be honest, I have no idea what types these are
	//lazy way out...
	auto begin(void) { return buffer.begin(); }
	auto end(void) { return buffer.end(); }
};

template <typename T, int Size>
void vds_fill_execute_initialize(vds_fill_execute_t<T, Size> * self, void (*cb)(vds_fill_execute_t<T, Size> * self))
{
	self->callback = cb;
	vds_ringbuf_initialize(&self->buffer);
}

template <typename T, int Size>
void vds_fill_execute_push(vds_fill_execute_t<T, Size> * self, T element)
{
	if (vds_ringbuf_is_almost_full(&self->buffer))
	{
		vds_ringbuf_push_back(&self->buffer, element);
		callback(self);
		vds_ringbuf_clear(&self->buffer);
	}
	else
	{
		vds_ringbuf_push_back(&self->buffer, element);
	}
}