#pragma once

#include "varia/ds/VDS-Types.hpp"

template <typename T, Integer_64 SIZE>
struct VDS_Simple_Queue
{
    T data[SIZE];
    Integer_64 current_head;
    Integer_64 item_count;
};

template<typename T, Integer_64 SIZE>
T vds_simple_queue_get_first_element(VDS_Simple_Queue<T, SIZE> const * queue)
{
    VDS_ASSERT( (queue->current_head >= 0) && (queue->current_head < SIZE), "Tried to access out of range element in queue");

    return queue->data[queue->current_head];
}

template<typename T, Integer_64 SIZE>
void vds_simple_queue_push(VDS_Simple_Queue<T, SIZE> * queue, T element)
{
    VDS_ASSERT(queue->item_count <= SIZE, "Pushing overruns queue");

    queue->data[queue->current_head] = element;
    queue->item_count += 1;
}

template<typename T, Integer_64 SIZE>
void vds_simple_queue_pop(VDS_Simple_Queue<T, SIZE> * queue)
{
    VDS_ASSERT(queue->current_head < SIZE, "Popping runs head over max size; reset the queue.");
    VDS_ASSERT(queue->item_count >= 0, "Somehow underran queue item count");

    queue->current_head += 1;
    queue->item_count -= 1;
}

template<typename T, Integer_64 SIZE>
Boolean vds_simple_queue_has_elements(VDS_Simple_Queue<T, SIZE> const * queue)
{
    return (queue->item_count > 0);
}

template<typename T, Integer_64 SIZE>
void vds_simple_queue_reset(VDS_Simple_Queue<T, SIZE> * queue)
{
    queue->item_count = 0;
    queue->current_head = 0;
}