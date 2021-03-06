/**
 * @file tensor.cpp
 * @author Daniel Nichols
 * @version 0.1
 * @date 2019-02-07
 * 
 * @copyright Copyright (c) 2019
 */
#include "tensor/tensor.h"

namespace magmadnn {

template <typename T>
Tensor<T>::Tensor(std::vector<unsigned int> shape) {
    init(shape, {TENSOR_DEFAULT_FILL_TYPE, {}}, TENSOR_DEFAULT_MEM_TYPE, TENSOR_DEFAULT_DEVICE_ID);
 }

 template <typename T>
Tensor<T>::Tensor(std::vector<unsigned int> shape, memory_t mem_type) {
    init(shape, {TENSOR_DEFAULT_FILL_TYPE, {}}, mem_type, TENSOR_DEFAULT_DEVICE_ID);
 }

template <typename T>
Tensor<T>::Tensor(std::vector<unsigned int> shape, memory_t mem_type, device_t device_id) { 
    init(shape, {TENSOR_DEFAULT_FILL_TYPE, {}}, mem_type, device_id);
}

template <typename T>
Tensor<T>::Tensor(std::vector<unsigned int> shape, tensor_filler_t<T> filler) { 
    init(shape, filler, TENSOR_DEFAULT_MEM_TYPE, TENSOR_DEFAULT_DEVICE_ID);
}

template <typename T>
Tensor<T>::Tensor(std::vector<unsigned int> shape, tensor_filler_t<T> filler, memory_t mem_type) { 
    init(shape, filler, mem_type, TENSOR_DEFAULT_DEVICE_ID);
}


template <typename T>
Tensor<T>::Tensor(std::vector<unsigned int> shape, tensor_filler_t<T> filler, memory_t mem_type, device_t device_id) { 
    init(shape, filler, mem_type, device_id);
}

template <typename T>
Tensor<T>::~Tensor() { 
    delete mem_manager;
}


template <typename T>
void Tensor<T>::init(std::vector<unsigned int>& shape, tensor_filler_t<T> filler, memory_t mem_type, device_t device_id) {
    // tensor must have at least 1 axis
    assert( shape.size() != 0 );

    // initialize class variables
    this->shape = shape;
    this->mem_type = mem_type;
    this->device_id = device_id;

    // calculate the total number of elements
    this->size = 1;
    for (unsigned int i = 0; i < shape.size(); i++) {
        this->size *= shape[i];
    }

    // create memory manager
    this->mem_manager = new MemoryManager<T> (size, mem_type, device_id);

    internal::fill_memory(*mem_manager, filler);
}


template <typename T>
magmadnn_error_t Tensor<T>::copy_from(const Tensor<T>& src, unsigned int begin_idx, unsigned int size) {
    assert( this->size == src.get_size() );
    assert( begin_idx+size <= this->size );

    return this->mem_manager->copy_from(*src.get_memory_manager(), begin_idx, size);
}

template <typename T>
magmadnn_error_t Tensor<T>::copy_from(const Tensor<T>& src) {
    return copy_from(src, 0, src.get_size());
}


template <typename T>
T Tensor<T>::get(const std::vector<int>& idx) const { 
    return mem_manager->get( get_flattened_index(idx) );
}

template <typename T>
T Tensor<T>::get(unsigned int flattened_idx) const {
    return mem_manager->get( flattened_idx );
}

template <typename T>
void Tensor<T>::set(const std::vector<int>& idx, T val) { 
    mem_manager->set( get_flattened_index(idx), val );
}

template <typename T>
void Tensor<T>::set(unsigned int flattened_idx, T val) {
    mem_manager->set( flattened_idx, val );
}

template <typename T>
unsigned int Tensor<T>::get_shape(unsigned int idx) const {
    assert( idx < this->size );
    return this->shape[idx];
}

template <typename T>
unsigned int Tensor<T>::get_flattened_index(const std::vector<int>& idx) const {
    unsigned int jump_size = 1; // the total amout to jump to get to next axis
    unsigned int flattened_idx = 0;

    for (int i = ((int)idx.size()) - 1; i >= 0; i--) {
        flattened_idx += idx[i] * jump_size;
        jump_size *= shape[i];
    }
    return flattened_idx;
 }



/* COMPILE FOR INT, FLOAT, AND DOUBLE */
template class Tensor<int>;
template class Tensor<float>;
template class Tensor<double>;

} // namespace magmadnn
