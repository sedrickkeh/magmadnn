/**
 * @file testing_math.cpp
 * @author Daniel Nichols
 * @version 0.1
 * @date 2019-06-10
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "magmadnn.h"
#include "utilities.h"

using namespace magmadnn;

void test_matmul(memory_t mem, unsigned int size);

int main(int argc, char **argv) {
    magmadnn_init();

    test_for_all_mem_types(test_matmul, 50);

    magmadnn_finalize();
}

void test_matmul(memory_t mem, unsigned int size) {

    printf("testing %s matmul...  ", get_memory_type_name(mem));

    Tensor<float> *A = new Tensor<float> ({size, size/2}, {CONSTANT,{1.0f}}, mem);
    Tensor<float> *B = new Tensor<float> ({size, size-5}, {CONSTANT,{6.0f}}, mem);
    Tensor<float> *C = new Tensor<float> ({size/2, size-5}, {ZERO,{}}, mem);

    math::matmul(1.0f, true, A, false, B, 1.0f, C);

    sync(C);

    for (unsigned int i = 0; i < size/2; i++) {
        for (unsigned int j = 0; j < size-5; j++) {
            assert( fequal(C->get({i,j}), 300.0f) );
        }
    }

    delete A;
    delete B;
    delete C;
    show_success();
}