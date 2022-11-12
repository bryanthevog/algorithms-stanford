#include "quicksort.hpp"
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;
using iterator = std::vector<int>::iterator;

int main() {
  string integers_file = "quicksort.txt";
  std::cout << "Quicksort of file: " << integers_file << std::endl;

  // Output comparisons for each quicksort pivot method
  string method = "first";
  vector<int> integers = read_integer_file(integers_file);
  unsigned long count = count_quicksort_comparisons(integers, method);
  std::cout << "Method: " << method << ", Comparisons: " << count << std::endl;

  method = "last";
  integers = read_integer_file(integers_file);
  count = count_quicksort_comparisons(integers, method);
  std::cout << "Method: " << method << ", Comparisons: " << count << std::endl;

  method = "median";
  integers = read_integer_file(integers_file);
  count = count_quicksort_comparisons(integers, method);
  std::cout << "Method: " << method << ", Comparisons: " << count << std::endl;

  method = "random";
  integers = read_integer_file(integers_file);
  count = count_quicksort_comparisons(integers, method);
  std::cout << "Method: " << method << ", Comparisons: " << count << std::endl;
}
