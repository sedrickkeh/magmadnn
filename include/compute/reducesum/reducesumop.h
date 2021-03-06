
#pragma once

#include "compute/operation.h"
#include "tensor/tensor.h"
#include "compute/reducesum/reducesum_internal.h"

namespace magmadnn {

namespace internal {
enum reduce_sum_op_t {
	ELEM_REDUCE,
	COL_REDUCE,
	ROW_REDUCE,
	TENSOR_REDUCE	/* Row Reduce for arbitrary tensors (TODO) */
};
}	// namespace internal

namespace op {

template <typename T>
class ReduceSumOp : public Operation<T> {
public:
	ReduceSumOp(Operation<T> *x, int axis, bool copy=true, bool needs_grad=true);

	virtual ~ReduceSumOp() {
		if (ones != NULL) delete ones;
	}

	Tensor<T> *eval(bool recompute=true);
	Operation<T> *grad(Operation<T> *consumer, Operation<T> *var, Operation<T> *grad);
	
	std::string to_string() { return "ReduceSum( " + x->to_string() + " )"; }
protected:
	Operation<T> *x;
	Tensor<T> *x_tensor;

	Tensor<T> *ones;

	int axis;
	bool copy;

	internal::reduce_sum_op_t op_type;
};

template <typename T>
ReduceSumOp<T>* reducesum(Operation<T> *x, int axis, bool copy=true, bool needs_grad=true);

} // namespace op
} // namespace magmadnn