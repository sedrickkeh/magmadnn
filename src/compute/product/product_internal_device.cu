/**
 * @file product_internal_device.cu
 * @author Daniel Nichols
 * @version 0.1
 * @date 2019-05-21
 * 
 * @copyright Copyright (c) 2019
 */
#include "compute/product/product_internal.h"

namespace magmadnn {
namespace internal {

template <typename T>
__global__ void kernel_product_full_device(T alpha, T *a, T *b, T *out, unsigned int arr_size) {
    unsigned int idx = blockIdx.x * blockDim.x + threadIdx.x;
    unsigned int stride = blockDim.x * gridDim.x;

    for (unsigned int i = idx; i < arr_size; i += stride) {
        out[i] = a[i] * b[i];
    }
}

template <typename T>
void product_full_device(T alpha, Tensor<T> *a, Tensor<T> *b, Tensor<T> *out) {
    kernel_product_full_device <<< 1, a->get_size() >>> (alpha, a->get_ptr(), b->get_ptr(), out->get_ptr(), a->get_size());
}
template void product_full_device(int alpha, Tensor<int> *a, Tensor<int> *b, Tensor<int> *out);
template void product_full_device(float alpha, Tensor<float> *a, Tensor<float> *b, Tensor<float> *out);
template void product_full_device(double alpha, Tensor<double> *a, Tensor<double> *b, Tensor<double> *out);

}   // namespace op
}   // namespace magmadnn