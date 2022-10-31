/* Counts the number of inversions contained within a text file of integers
 */
// #define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#define DOCTEST_CONFIG_DISABLE
#include "doctest.h"
#include <csignal>
#include <fstream>
#include <string>
#include <vector>

using std::string;
using std::vector;
using iterator = std::vector<int>::iterator;

/**
   Read the file containing integers into a vector
 */
vector<int> read_integer_file(string integer_file) {
  std::ifstream integer_file_stream;
  integer_file_stream.open(integer_file);

  int integer;
  vector<int> integers;
  while (integer_file_stream >> integer) {
    integers.push_back(integer);
  }

  return integers;
}

unsigned long combine_and_count(iterator begin, iterator mid, iterator end) {
  vector<int> temp;
  temp.reserve(std::distance(begin, end));
  unsigned long count = 0;
  iterator left = begin;
  iterator right = mid;

  while (left != mid and right != end) {
    if (*left > *right) {
      // add number from right side and increment to next index
      temp.emplace_back(*right++);
      // add inversions to count
      count += std::distance(left, mid);
    } else { // *left < *right
      // add number from left side and increment to next index
      temp.emplace_back(*left++);
    }
  }
  temp.insert(temp.end(), left, mid);
  temp.insert(temp.end(), right, end);

  // Move contents of temp vector to the original vector
  std::move(temp.begin(), temp.end(), begin);
  return count;
}

unsigned long sort_and_count(iterator begin, iterator end) {
  unsigned long count = 0;
  int size = std::distance(begin, end);
  if (size <= 1) {
    return 0;
  } else {
    iterator mid = std::next(begin, size / 2);
    count += sort_and_count(begin, mid);
    count += sort_and_count(mid, end);
    count += combine_and_count(begin, mid, end);
    return count;
  }
}

/**
   Count the number of inversions in a vector
 */
unsigned long count_inversions(vector<int> &integer_vector) {
  // std::raise(SIGTRAP);
  unsigned long count =
      sort_and_count(integer_vector.begin(), integer_vector.end());
  return count;
}

/**
   Converts vector to string format for testing
 */
string vector_to_string(std::vector<int> num_vector) {
  string num_string = "";
  string digit_string;
  for (int digit : num_vector) {
    digit_string = std::to_string(digit);
    num_string.append(digit_string);
    num_string.append(" ");
  }
  return num_string;
}

TEST_CASE("File reading test") {
  string integer_file = "test.txt";
  vector<int> test_vector{1, 14108, 100000};
  CHECK(vector_to_string(read_integer_file(integer_file)) ==
        vector_to_string(test_vector));
}

TEST_CASE("Test inversion counting") {
  SUBCASE("Simple 1 inversion") {
    vector<int> test_vector{1, 3, 2, 4, 5, 6, 7, 8};
    unsigned long inversions = 1;
    CHECK(count_inversions(test_vector) == inversions);
  }
  SUBCASE("Simple inversion") {
    vector<int> test_vector{1, 2, 5, 4, 6, 3, 8, 7};
    unsigned long inversions = 5;
    CHECK(count_inversions(test_vector) == inversions);
  }
  SUBCASE("Simple inversion") {
    vector<int> test_vector{5, 6, 7, 8, 1, 2, 3, 4};
    unsigned long inversions = 16;
    CHECK(count_inversions(test_vector) == inversions);
  }
  SUBCASE("Simple inversion") {
    vector<int> test_vector{11, 17, 19, 12};
    unsigned long inversions = 2;
    CHECK(count_inversions(test_vector) == inversions);
  }
  SUBCASE("Simple inversion") {
    vector<int> test_vector{2, 1, 8, 3, 7, 6, 5, 4};
    unsigned long inversions = 12;
    CHECK(count_inversions(test_vector) == inversions);
  }
}
