/**
 * @file utilities_internal.cpp
 * @author Daniel Nichols
 * @version 0.1
 * @date 2019-05-21
 * 
 * @copyright Copyright (c) 2019
 */
#include "utilities_internal.h"

namespace magmadnn {
namespace internal {

int debugf(const char *fmt, ...) {
    #if defined(DEBUG)
    int bytes;
    va_list args;
    va_start(args, fmt);
    bytes = vfprintf(stdout, fmt, args);
    va_end(args);
    return bytes;
    #endif
    return 0;
}

void print_vector(const std::vector<unsigned int>& vec, bool debug, char begin, char end, char delim) {
    int (*print)(const char*,...) = (debug) ? debugf : std::printf;

    print("%c ", begin);
    for (std::vector<unsigned int>::const_iterator it = vec.begin(); it != vec.end(); it++) {
        print("%lu", *it);
        if (it != vec.end()-1) print("%c ", delim);
    }
    print(" %c\n", end);
}

template <typename T>
void print_compute_graph(op::Operation<T> *node, bool debug) {
    std::set<op::Operation<T> *> visited;
    std::deque<op::Operation<T> *> to_visit;
    op::Operation<T> *cur;
    int (*print)(const char *,...);
    typename std::vector<op::Operation<T> *>::const_iterator vit;

    print = (debug) ? debugf : std::printf;

    to_visit.push_back( node );
    visited.insert( node );

    while (!to_visit.empty()) {
        cur = to_visit.front();
        to_visit.pop_front();

        print("Operation [%s]:\n", cur->to_string().c_str());

        print("\tConsumers:");
        std::vector<op::Operation<T> *> const& consumers = cur->get_consumers();
        for (vit = consumers.begin(); vit != consumers.end(); vit++) {
            print(" [%s]", (*vit)->to_string().c_str());

            if (visited.find((*vit)) == visited.end()) {
                to_visit.push_back(*vit);
                visited.insert((*vit));
            }
        }
        
        print("\n\tInputs:");
        std::vector<op::Operation<T> *> const& inputs = cur->get_inputs();
        for (vit = inputs.begin(); vit != inputs.end(); vit++) {
            print(" [%s]", (*vit)->to_string().c_str());

            if (visited.find((*vit)) == visited.end()) {
                to_visit.push_back(*vit);
                visited.insert((*vit));
            }
        }
        print("\n");
    }
}
template void print_compute_graph(op::Operation<int> *node, bool debug);
template void print_compute_graph(op::Operation<float> *node, bool debug);
template void print_compute_graph(op::Operation<double> *node, bool debug);

}   // namespace internal
}   // namespace magmadnn