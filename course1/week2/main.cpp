#include "count_inversions.hpp"
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;
using iterator = std::vector<int>::iterator;

int main() {
  string test_integer_file = "test.txt";
  std::cout << "File: " << test_integer_file;
  vector<int> test_integers = read_integer_file(test_integer_file);
  unsigned long test_count_inversions = count_inversions(test_integers);
  std::cout << ", Inversions: " << test_count_inversions << "\n";

  string integers_file = "integers.txt";
  std::cout << "File: " << integers_file;
  vector<int> integers = read_integer_file(integers_file);
  unsigned long inversions_count = count_inversions(integers);
  std::cout << ", Inversions: " << inversions_count << "\n";
}
