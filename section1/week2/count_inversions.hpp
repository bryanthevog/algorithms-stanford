#ifndef ALGOS_HW_COUNT_INVERSIONS
#define ALGOS_HW_COUNT_INVERSIONS

#include <string>
#include <vector>

using std::string;
using std::vector;
using iterator = std::vector<int>::iterator;

vector<int> read_integer_file(string integer_file);
unsigned long count_inversions(vector<int> &integer_vector);

#endif
