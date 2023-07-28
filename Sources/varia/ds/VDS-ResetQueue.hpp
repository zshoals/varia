#pragma once

#include "varia/ds/VDS-Types.hpp"

template <typename T, Integer_64 SIZE>
struct VDS_Reset_Queue
{
    T data[SIZE];
    Integer_64 current_head;
    Integer_64 item_count;
};

template<typename T, Integer_64 SIZE>
T vds_reset_queue_get_first_element(VDS_Reset_Queue<T, SIZE> const * queue)
{
    VDS_ASSERT( (queue->current_head >= 0) && (queue->current_head < SIZE), "Tried to access out of range element in queue");
    if (queue->current_head >= 0 && queue->current_head < SIZE)
    {
        return queue->data[queue->current_head];
    }
    else
    {
        return queue->data[0];
    }
}

template<typename T, Integer_64 SIZE>
Boolean vds_reset_queue_push(VDS_Reset_Queue<T, SIZE> * queue, T element)
{
    VDS_ASSERT(queue->item_count <= SIZE, "Pushing overruns queue");

    if (queue->item_count < SIZE)
    {
        queue->data[queue->current_head] = element;
        queue->item_count += 1;

        return true;
    }

    return false;
}

template<typename T, Integer_64 SIZE>
void vds_reset_queue_pop(VDS_Reset_Queue<T, SIZE> * queue)
{
    VDS_ASSERT(queue->current_head < SIZE, "Popping runs head over max size; reset the queue.");
    VDS_ASSERT(queue->item_count >= 0, "Somehow underran queue item count");

    if (queue->current_head < SIZE && queue->item_count > 0)
    {
        queue->current_head += 1;
        queue->item_count -= 1;
    }
}

template<typename T, Integer_64 SIZE>
Boolean vds_reset_queue_has_elements(VDS_Reset_Queue<T, SIZE> const * queue)
{
    return (queue->item_count > 0);
}

template<typename T, Integer_64 SIZE>
void vds_reset_queue_reset(VDS_Reset_Queue<T, SIZE> * queue)
{
    queue->item_count = 0;
    queue->current_head = 0;
}