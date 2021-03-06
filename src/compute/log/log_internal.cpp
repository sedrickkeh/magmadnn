
#include "compute/log/log_internal.h"

namespace magmadnn {
namespace internal {

template <typename T>
void log_full(Tensor<T> *x, Tensor<T> *out) {
    if (x->get_memory_type() == HOST) {
        T *x_ptr = x->get_ptr();
        T *out_ptr = out->get_ptr();
        unsigned int size = x->get_size();

        for (unsigned int i = 0; i < size; i++) {
            out_ptr[i] = log( x_ptr[i] );
        }
    }
    #if defined(_HAS_CUDA_)
    else { 
        log_full_device(x, out);
    }
    #endif
}
template void log_full(Tensor<int> *x, Tensor<int> *out);
template void log_full(Tensor<float> *x, Tensor<float> *out);
template void log_full(Tensor<double> *x, Tensor<double> *out);

}   // namespace op
}   // namespace magmadnn
