#ifndef ALGOS_HW_COUNT_INVERSIONS
#define ALGOS_HW_COUNT_INVERSIONS

#include <string>
#include <vector>

using std::string;
using std::vector;
using iterator = std::vector<int>::iterator;

/** Reads integers from a text file and returns a vector of ints
 */
vector<int> read_integer_file(string integer_file);

/** Counts the number of comparisons made when using different quicksort pivot
 * picking methods
 * @param integer_vector is the vector containing integers to sort
 * @param method is the pivot picking method name (first, last, median, random)
 */
unsigned long count_quicksort_comparisons(vector<int> &integer_vector,
                                          string method);

#endif
