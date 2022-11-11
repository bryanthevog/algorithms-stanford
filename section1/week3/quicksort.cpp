/* Counts the number of inversions contained within a text file of integers
 */
// #define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#define DOCTEST_CONFIG_DISABLE
#include "doctest.h"
#include <csignal>
#include <fstream>
#include <functional>
#include <random>
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

// Different pivot selection methods
iterator select_first_pivot(iterator begin, iterator end) { return begin; }

iterator select_last_pivot(iterator begin, iterator end) { return end - 1; }

iterator select_median_pivot(iterator begin, iterator end) {
  int size = std::distance(begin, end);
  iterator middle = std::next(begin, size / 2);
  iterator pivot;
  if ((*begin < *middle) && (*middle < *end)) {
    pivot = middle;
  } else if ((*middle < *begin) && (*begin < *end)) {
    pivot = begin;
  } else {
    pivot = end;
  }
  return pivot;
}

iterator select_random_pivot(iterator begin, iterator end) {
  int size = std::distance(begin, end);
  std::random_device rd;
  std::default_random_engine generator(rd());
  std::uniform_int_distribution<int> distribution(0, size);
  iterator pivot = std::next(begin, distribution(generator));
  return pivot;
}

void partition(iterator begin, iterator end, iterator pivot) {
  // Swap iterator value to the beginning of vector
  if (pivot != begin) {
    std::iter_swap(begin, pivot);
  }

  // Loop through vector and partition
  iterator i = begin + 1;
  for (iterator j = begin + 1; j != end; j++) {
    if (*j < *begin) {
      std::iter_swap(i, j);
      i++;
    } else {
      std::iter_swap(i, j);
    }
  }

  // Swap pivot value into its rightful location
  std::iter_swap(begin, i - 1);
}

unsigned long
quicksort(iterator begin, iterator end,
          std::function<iterator(iterator, iterator)> select_pivot) {
  if (begin == end) {
    return 0;
  }
  iterator pivot = select_pivot(begin, end);
  partition(begin, end, pivot);
  unsigned long count = std::distance(begin, end) - 1;
  count += quicksort(begin, pivot, select_pivot);
  count += quicksort(pivot + 1, end, select_pivot);
  return count;
}

unsigned long count_quicksort_comparisons(vector<int> &integer_vector,
                                          string method) {
  std::function<iterator(iterator, iterator)> select_pivot;
  if (method == "first") {
    select_pivot = select_first_pivot;
  } else if (method == "last") {
    select_pivot = select_last_pivot;
  } else if (method == "median") {
    select_pivot = select_median_pivot;
  } else if (method == "random") {
    select_pivot = select_random_pivot;
  } else {
    return 0;
  }
  return quicksort(integer_vector.begin(), integer_vector.end(), select_pivot);
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

TEST_CASE("Test quicksort counting") {
  SUBCASE("First element pivot") {
    vector<int> test_vector{8, 3, 4, 5, 1, 7, 6, 2};
    unsigned long comparisons = 21;
    string method = "first";
    CHECK(count_quicksort_comparisons(test_vector, method) == comparisons);
  }
  SUBCASE("Last element pivot") {
    vector<int> test_vector{8, 3, 4, 5, 1, 7, 6, 2};
    unsigned long comparisons = 21;
    string method = "last";
    CHECK(count_quicksort_comparisons(test_vector, method) == comparisons);
  }
  SUBCASE("Median element pivot") {
    vector<int> test_vector{8, 3, 4, 5, 1, 7, 6, 2};
    unsigned long comparisons = 15;
    string method = "median";
    CHECK(count_quicksort_comparisons(test_vector, method) == comparisons);
  }
}
