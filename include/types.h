/**
 * @file types.h
 * @author Daniel Nichols
 * @version 0.1
 * @date 2019-02-07
 * 
 * @copyright Copyright (c) 2019
 */
#pragma once

#include <vector> // for tensor_filler_t

namespace magmadnn {

typedef enum memory_t {
	HOST,
	#if defined(_HAS_CUDA_)
	DEVICE,
	MANAGED,
	CUDA_MANAGED
	#endif
} memory_t;

typedef unsigned int device_t;
typedef unsigned int magmadnn_error_t;


/**	 Different ways to initialize the tensor on creation.
 */
enum tensor_fill_t {
	UNIFORM,
	GLOROT,
	CONSTANT,
	ZERO,
	ONE,
	DIAGONAL,
	IDENTITY,
	NONE
};	

/** Defines how to fill a tensor and with what parameters.
 * fill_type: use UNIFORM, GLOROT, CONSTANT, ZERO, ONE, or NONE @see tensor_fill_t
 * values: the parameters for the fill_type
 */
template <typename T>
struct tensor_filler_t {
	tensor_fill_t fill_type;
	std::vector<T> values;
};

} // namespace magmadnn