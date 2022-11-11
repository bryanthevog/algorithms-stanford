#include "quicksort.hpp"
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;
using iterator = std::vector<int>::iterator;

int main() {
  string integers_file = "quicksort.txt";
  std::cout << "File: " << integers_file;
  vector<int> integers = read_integer_file(integers_file);

  // Output comparisons for each quicksort pivot method
  string method = "first";
  unsigned long count = count_quicksort_comparisons(integers, method);
  std::cout << "Method: " << method << ", Comparisons: " << count << std::endl;

  method = "last";
  count = count_quicksort_comparisons(integers, method);
  std::cout << "Method: " << method << ", Comparisons: " << count << std::endl;

  method = "median";
  count = count_quicksort_comparisons(integers, method);
  std::cout << "Method: " << method << ", Comparisons: " << count << std::endl;

  method = "random";
  count = count_quicksort_comparisons(integers, method);
  std::cout << "Method: " << method << ", Comparisons: " << count << std::endl;
}
