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
  long size = std::distance(begin, end);
  // If there aren't 3 or more elements, return the first one
  if (size < 3) {
    return begin;
  }
  // Else find the median pivot
  iterator middle = std::next(begin, (size - 1) / 2);
  iterator last = std::next(end, -1);
  iterator pivot;
  if ((*begin < *middle) && (*begin < *last)) { // if begin is the smallest
    if (*middle < *last) {
      pivot = middle;
    } else {
      pivot = last;
    }
  } else if ((*last < *begin) && (*last < *middle)) { // if last is the smallest
    if (*middle < *begin) {
      pivot = middle;
    } else {
      pivot = begin;
    }
  } else { // if middle is the smallest
    if (*begin < *last) {
      pivot = begin;
    } else {
      pivot = last;
    }
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

iterator partition(iterator begin, iterator end, iterator pivot) {
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
    }
  }

  // Swap pivot value into its rightful location
  std::iter_swap(begin, i - 1);
  return i - 1;
}

unsigned long
quicksort(iterator begin, iterator end,
          std::function<iterator(iterator, iterator)> select_pivot) {
  // Base case
  if (std::distance(begin, end) < 2) {
    return 0;
  }

  // Select pivot and partition around it
  iterator pivot = select_pivot(begin, end);
  pivot = partition(begin, end, pivot);

  // Add to count and make recursive calls
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

TEST_CASE("Median pivot picking tests") {

  SUBCASE("1, 2") {
    vector<int> test_vector{1, 2};
    int median = 1;
    iterator pivot =
        select_median_pivot(test_vector.begin(), test_vector.end());
    CHECK(*pivot == median);
  }

  SUBCASE("2, 1") {
    vector<int> test_vector{2, 1};
    int median = 2;
    iterator pivot =
        select_median_pivot(test_vector.begin(), test_vector.end());
    CHECK(*pivot == median);
  }

  SUBCASE("1, 2, 3") {
    vector<int> test_vector{1, 2, 3};
    int median = 2;
    iterator pivot =
        select_median_pivot(test_vector.begin(), test_vector.end());
    CHECK(*pivot == median);
  }

  SUBCASE("2, 1, 3") {
    vector<int> test_vector{2, 1, 3};
    int median = 2;
    iterator pivot =
        select_median_pivot(test_vector.begin(), test_vector.end());
    CHECK(*pivot == median);
  }

  SUBCASE("3, 1, 2") {
    vector<int> test_vector{3, 1, 2};
    int median = 2;
    iterator pivot =
        select_median_pivot(test_vector.begin(), test_vector.end());
    CHECK(*pivot == median);
  }

  SUBCASE("1, 2, 3, 4") {
    vector<int> test_vector{1, 2, 3, 4};
    int median = 2;
    iterator pivot =
        select_median_pivot(test_vector.begin(), test_vector.end());
    CHECK(*pivot == median);
  }

  SUBCASE("4, 1, 3, 2") {
    vector<int> test_vector{4, 1, 3, 2};
    int median = 2;
    iterator pivot =
        select_median_pivot(test_vector.begin(), test_vector.end());
    CHECK(*pivot == median);
  }

  SUBCASE("4, 3, 1, 2") {
    vector<int> test_vector{4, 3, 1, 2};
    int median = 3;
    iterator pivot =
        select_median_pivot(test_vector.begin(), test_vector.end());
    CHECK(*pivot == median);
  }

  SUBCASE("2, 1, 3, 4") {
    vector<int> test_vector{2, 1, 3, 4};
    int median = 2;
    iterator pivot =
        select_median_pivot(test_vector.begin(), test_vector.end());
    CHECK(*pivot == median);
  }
}

TEST_CASE("Test quicksort counting") {
  vector<int> sorted_vector{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  string large_integer_file = "input_dgrcode_16_100000.txt";
  string xlarge_integer_file = "input_dgrcode_19_1000000.txt";

  SUBCASE("First element pivot - basic") {
    vector<int> test_vector{1, 6, 8, 10, 7, 5, 2, 9, 4, 3};
    unsigned long comparisons = 26;
    string method = "first";
    CHECK(count_quicksort_comparisons(test_vector, method) == comparisons);
    CHECK(vector_to_string(test_vector) == vector_to_string(sorted_vector));
  }

  SUBCASE("First element pivot - large vector") {
    vector<int> test_vector = read_integer_file(large_integer_file);
    unsigned long comparisons = 2127173;
    string method = "first";
    CHECK(count_quicksort_comparisons(test_vector, method) == comparisons);
  }

  SUBCASE("First element pivot - xlarge vector") {
    vector<int> test_vector = read_integer_file(xlarge_integer_file);
    unsigned long comparisons = 25030219;
    string method = "first";
    CHECK(count_quicksort_comparisons(test_vector, method) == comparisons);
  }

  SUBCASE("Last element pivot") {
    vector<int> test_vector{1, 6, 8, 10, 7, 5, 2, 9, 4, 3};
    unsigned long comparisons = 21;
    string method = "last";
    CHECK(count_quicksort_comparisons(test_vector, method) == comparisons);
    CHECK(vector_to_string(test_vector) == vector_to_string(sorted_vector));
  }

  SUBCASE("Last element pivot - large vector") {
    vector<int> test_vector = read_integer_file(large_integer_file);
    unsigned long comparisons = 2079088;
    string method = "last";
    CHECK(count_quicksort_comparisons(test_vector, method) == comparisons);
  }

  SUBCASE("Last element pivot - xlarge vector") {
    vector<int> test_vector = read_integer_file(xlarge_integer_file);
    unsigned long comparisons = 23829985;
    string method = "last";
    CHECK(count_quicksort_comparisons(test_vector, method) == comparisons);
  }

  SUBCASE("Median element pivot") {
    vector<int> test_vector{1, 6, 8, 10, 7, 5, 2, 9, 4, 3};
    unsigned long comparisons = 21;
    string method = "median";
    CHECK(count_quicksort_comparisons(test_vector, method) == comparisons);
    CHECK(vector_to_string(test_vector) == vector_to_string(sorted_vector));
  }

  SUBCASE("Median element pivot - large vector") {
    vector<int> test_vector = read_integer_file(large_integer_file);
    unsigned long comparisons = 1749103;
    string method = "median";
    CHECK(count_quicksort_comparisons(test_vector, method) == comparisons);
  }

  SUBCASE("Median element pivot - xlarge vector") {
    vector<int> test_vector = read_integer_file(xlarge_integer_file);
    unsigned long comparisons = 21249115;
    string method = "median";
    CHECK(count_quicksort_comparisons(test_vector, method) == comparisons);
  }

  SUBCASE("Random element pivot") {
    vector<int> test_vector{1, 6, 8, 10, 7, 5, 2, 9, 4, 3};
    unsigned long comparisons = 55;
    string method = "median";
    CHECK_LE(count_quicksort_comparisons(test_vector, method), comparisons);
    CHECK(vector_to_string(test_vector) == vector_to_string(sorted_vector));
  }
}
