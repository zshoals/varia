#pragma once

#include "varia/ds/VDS-Types.hpp"
#include "varia/ds/VDS-Util.hpp"

template <typename T, Integer_64 SIZE>
struct VDS_Reset_Queue_Storage
{
    T data[SIZE];
    Integer_64 current_head;
    Integer_64 item_count;
};

template <typename T>
struct VDS_Reset_Queue
{
    T * data;
    Integer_64 * current_head;
    Integer_64 * item_count;
    Integer_64 const capacity;
};

template<typename T, Integer_64 SIZE>
VDS_Reset_Queue<T> vds_reset_queue_make_interface(VDS_Reset_Queue_Storage<T, SIZE> * queue)
{
    VDS_Reset_Queue<T> interface = {
        &(queue->data[0]),
        &(queue->current_head),
        &(queue->item_count),
        SIZE
    };

    return interface;
}

template <typename T>
T vds_reset_queue_get_first_element(VDS_Reset_Queue<T> const * queue)
{
    VDS_ASSERT( ( *(queue->current_head) >= 0 ) && ( *(queue->current_head) < queue->capacity ), "Tried to access out of range element in queue");

    if (*(queue->current_head) >= 0 && *(queue->current_head) < queue->capacity)
    {
        return queue->data[*(queue->current_head)];
    }
    else
    {
        return queue->data[0];
    }
}

template <typename T>
Boolean vds_reset_queue_push(VDS_Reset_Queue<T> * queue, T element)
{
    VDS_ASSERT( *(queue->item_count) <= queue->capacity, "Pushing overruns queue");

    if ( *(queue->item_count) < queue->capacity)
    {
        queue->data[*(queue->item_count)] = element;
        *(queue->item_count) += 1;

        return true;
    }

    return false;
}

template <typename T>
void vds_reset_queue_pop(VDS_Reset_Queue<T> * queue)
{
    VDS_ASSERT(*(queue->current_head) < queue->capacity, "Popping runs head over max size; reset the queue.");
    VDS_ASSERT(*(queue->item_count) >= 0, "Somehow underran queue item count");

    if (*(queue->current_head) < queue->capacity && *(queue->item_count) > 0)
    {
        *(queue->current_head) += 1;
        *(queue->item_count) -= 1;
    }
}

template <typename T>
Boolean vds_reset_queue_has_elements(VDS_Reset_Queue<T> const * queue)
{
    return (*(queue->item_count) > 0);
}

template <typename T>
void vds_reset_queue_reset(VDS_Reset_Queue<T> * queue)
{
    *(queue->item_count) = 0;
    *(queue->current_head) = 0;
}
