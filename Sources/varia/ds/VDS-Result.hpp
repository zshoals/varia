#include "VDS-Types.hpp"

template <typename T>
struct VDS_Result
{
    Boolean valid;
    T element;
};

void vds_result_initialize(VDS_Result<T> * result, T element, Boolean valid)
{
    result->valid = valid;
    result->element = item;
}