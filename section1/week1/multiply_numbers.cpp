/* Finds the product of two large integers based on Karatsuba's
   multiplication algorithm. See:
   https://en.wikipedia.org/wiki/Karatsuba_algorithm
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <cmath>
#include <csignal>
#include <iostream>
#include <string>
#include <vector>

// Function to handle adding two numbers in vector form
std::vector<int> add_numbers(std::vector<int> &num1, std::vector<int> &num2) {
  int num1_size = num1.size();
  int num2_size = num2.size();
  std::vector<int> big_num;
  std::vector<int> little_num;
  if (num1_size > num2_size) {
    big_num = num1;
    little_num = num2;
  } else {
    big_num = num2;
    little_num = num1;
  }
  int big_num_size = big_num.size();
  int little_num_size = little_num.size();
  std::vector<int> sum(big_num_size + 1, 0);
  int sum_size = sum.size();

  int carry_over = 0; // carry over value in addition
  int big_num_digit;
  int little_num_digit;
  int digits_sum;
  // Loop through digits from end to beginning and add similar to
  // grade school long addition
  for (int i = 1; i < sum_size + 1; i++) {
    big_num_digit = 0;
    little_num_digit = 0;
    // Check to see if index is in big and little nums
    if (i <= big_num_size) {
      big_num_digit = big_num[big_num_size - i];
    }
    if (i <= little_num_size) {
      little_num_digit = little_num[little_num_size - i];
    }
    digits_sum = carry_over + big_num_digit + little_num_digit;
    sum[sum_size - i] = digits_sum % 10; // only get digit at end
    carry_over = digits_sum / 10;        // set remainder for next addition
  }

  // Remove leading zero if it exists
  if (sum[0] == 0) {
    sum.erase(sum.begin());
  }
  return sum;
}

// Function to handle subtracting two numbers in vector form
// Assumes that num1 is larger than num2
std::vector<int> subtract_numbers(std::vector<int> &num1,
                                  std::vector<int> &num2) {
  std::vector<int> sum(num1.size(), 0);
  int num1_digit;
  int num2_digit;

  // Follows long subtraction method from gradeschool
  int carry_over = 0;
  for (int i = 1; i < num1.size() + 1; i++) {
    num1_digit = num1[num1.size() - i];
    if (i <= num2.size()) {
      num2_digit = num2[num2.size() - i];
    } else {
      num2_digit = 0;
    }
    if (num1_digit - carry_over < num2_digit) {
      sum[num1.size() - i] = 10 + num1_digit - carry_over - num2_digit;
      carry_over = 1;
    } else {
      sum[num1.size() - i] = num1_digit - carry_over - num2_digit;
      carry_over = 0;
    }
  }
  if (sum[0] == 0) {
    sum.erase(sum.begin());
  }
  return sum;
}

// Extracts portion of vector from [0, index)
std::vector<int> upper_slice(std::vector<int> &num, int index) {
  std::vector<int> tmp(index, 0);
  for (int i = 0; i < index; i++) {
    tmp[i] = num[i];
  }
  return tmp;
}
// Extracts portion of vector from [index, end]
std::vector<int> lower_slice(std::vector<int> &num, int index) {
  std::raise(SIGTRAP);
  std::vector<int> tmp(num.size() - index, 0);
  for (int i = 0; i < (num.size() - index); i++) {
    tmp[i] = num[i];
  }
  return tmp;
}

// Converts vector representation to string
std::string vector_to_string(std::vector<int> num_vector) {
  std::string num_string = "";
  std::string digit_string;
  for (int digit : num_vector) {
    digit_string = std::to_string(digit);
    num_string.append(digit_string);
  }
  return num_string;
}

// Converts vector representation to integer form
int vector_to_integer(std::vector<int> num_vector) {
  int num_integer = 0;
  int power = num_vector.size() - 1;
  for (int i = 0; i < num_vector.size(); i++) {
    num_integer += num_vector[i] * std::pow(10, power);
    power--; // decrease power for next index
  }
  return num_integer;
}

// Basic multiplication function
std::vector<int> basic_multiply(std::vector<int> num1, std::vector<int> num2) {
  int product = vector_to_integer(num1) * vector_to_integer(num2);
  std::vector<int> product_vector;
  product_vector.push_back(product % 10);
  while (product / 10 > 0) {
    product = product / 10; // shift number down i.e. 123 -> 12
    product_vector.push_back(product % 10);
  }
  std::reverse(product_vector.begin(), product_vector.end());
  return product_vector;
}

// Function to calculate multiplication of two numbers in vector form
std::vector<int> karatsuba(std::vector<int> &num1, std::vector<int> &num2) {
  int num1_size = num1.size();
  int num2_size = num2.size();

  // Do basic multiplication if one of the numbers are single digit
  if ((num1.size() < 2) or (num2.size() < 2)) {
    return basic_multiply(num1, num2);
  }

  // Calculate the size of the numbers
  int m = num1_size;
  if (num2_size < num1_size) {
    m = num2_size;
  }
  int m2 = m / 2;

  // Find lower and upper splits for both numbers
  std::vector<int> upper1 = upper_slice(num1, m2);
  std::vector<int> lower1 = lower_slice(num1, m2);
  std::vector<int> upper2 = upper_slice(num2, m2);
  std::vector<int> lower2 = lower_slice(num2, m2);

  // Recursive calls made to numbers ~half size of input
  std::vector<int> sum_1 = add_numbers(lower1, upper1);
  std::vector<int> sum_2 = add_numbers(lower2, upper2);
  std::vector<int> z0 = karatsuba(lower1, lower2);
  std::vector<int> z1 = karatsuba(sum_1, sum_2);
  std::vector<int> z2 = karatsuba(upper1, upper2);

  // Returns:
  // (z2 × 10 ^ (m2 × 2)) + ((z1 - z2 - z0) × 10 ^ m2) + z0
  // Part 1               + Part 2                     + z0
  std::vector<int> part1(z2.size() + (m2 * 2), 0);
  for (int i = 0; i < z2.size(); i++) {
    part1[i] = z2[i];
  }
  std::vector<int> part2 = subtract_numbers(z1, z2);
  std::vector<int> part2_2 = subtract_numbers(part2, z0);
  std::vector<int> part2_padded(part2_2.size(), 0);
  for (int i = 0; i < z2.size(); i++) {
    part2_padded[i] = part2_2[i];
  }
  std::vector<int> parts_sum = add_numbers(part1, part2_padded);
  std::vector<int> parts_sum_2 = add_numbers(parts_sum, z0);
  return parts_sum_2;
}

// int main() {
//   std::vector<int> num1 = {1, 2, 7, 8, 4, 2, 1, 5};
//   std::vector<int> num2 = {3, 8, 1, 0, 5, 6, 7, 1};
//   std::vector<int> sum_1 = {5, 0, 8, 8, 9, 8, 8, 6};
//   std::cout << "Num1: " << vector_to_string(num1) << "\n";
//   std::cout << "Num2: " << vector_to_string(num2) << "\n";
//   std::cout << "Sum: " << vector_to_string(sum_1) << "\n";
//   return 0;
// }

TEST_CASE("Testing number adding function") {
  SUBCASE("Test adding two large numbers") {
    std::vector<int> num1_1 = {1, 2, 7, 8, 4, 2, 1, 5};
    std::vector<int> num2_1 = {3, 8, 1, 0, 5, 6, 7, 1};
    std::vector<int> sum_1 = {5, 0, 8, 8, 9, 8, 8, 6};
    CHECK(vector_to_string(add_numbers(num1_1, num2_1)) ==
          vector_to_string(sum_1));
  }
  SUBCASE("Test adding numbers of different length") {
    std::vector<int> num1_2 = {1, 2, 7, 0, 0, 0, 0};
    std::vector<int> num2_2 = {3, 4, 6, 1, 2};
    std::vector<int> sum_2 = {1, 3, 0, 4, 6, 1, 2};
    CHECK(vector_to_string(add_numbers(num1_2, num2_2)) ==
          vector_to_string(sum_2));
  }
  // Test adding two numbers where output vector is larger
  std::vector<int> num1_3 = {9, 9, 9, 9};
  std::vector<int> num2_3 = {9, 9, 9, 9};
  std::vector<int> sum_3 = {1, 9, 9, 9, 8};
  CHECK(vector_to_string(add_numbers(num1_3, num2_3)) ==
        vector_to_string(sum_3));
  // Test adding where num2 is a larger vector than num1
  std::vector<int> num1_4 = {1, 2, 3};
  std::vector<int> num2_4 = {1, 2, 3, 4};
  std::vector<int> sum_4 = {1, 3, 5, 7};
  CHECK(vector_to_string(add_numbers(num1_4, num2_4)) ==
        vector_to_string(sum_4));
}

TEST_CASE("Testing number subtracting function") {
  SUBCASE("Test subtracting two large numbers") {
    std::vector<int> num1_1 = {1, 2, 7, 8, 4, 2, 1, 5};
    std::vector<int> num2_1 = {8, 1, 0, 5, 6, 7, 1};
    std::vector<int> sum_1 = {4, 6, 7, 8, 5, 4, 4};
    CHECK(vector_to_string(subtract_numbers(num1_1, num2_1)) ==
          vector_to_string(sum_1));
  }
  SUBCASE("Test multi-subtraction") {
    std::vector<int> num1_2 = {1, 2, 7, 8, 4, 2, 1, 5};
    std::vector<int> num2_2 = {1, 4, 5, 6, 7};
    std::vector<int> sum_2 = {1, 2, 7, 5, 5, 0, 8, 1};
    std::vector<int> sub_1 = subtract_numbers(num1_2, num2_2);
    CHECK(vector_to_string(subtract_numbers(sub_1, num2_2)) ==
          vector_to_string(sum_2));
  }
}

TEST_CASE("Testing vector to integer conversion") {
  SUBCASE("Single digit") {
    std::vector<int> single_digit = {9};
    CHECK(vector_to_integer(single_digit) == 9);
  }
  SUBCASE("Two digits") {
    std::vector<int> single_digit = {1, 2};
    CHECK(vector_to_integer(single_digit) == 12);
  }
  SUBCASE("Three digits") {
    std::vector<int> single_digit = {1, 2, 3};
    CHECK(vector_to_integer(single_digit) == 123);
  }
}

TEST_CASE("Testing basic multiplication") {
  SUBCASE("Two one digit numbers with product greater than 9") {
    std::vector<int> num1 = {9};
    std::vector<int> num2 = {7};
    CHECK(vector_to_integer(basic_multiply(num1, num2)) == 63);
  }
  SUBCASE("Two one digit numbers with product less than 10") {
    std::vector<int> num1 = {1};
    std::vector<int> num2 = {2};
    CHECK(vector_to_integer(basic_multiply(num1, num2)) == 2);
  }
  SUBCASE("One 2 digit and one 1 digit number with 3 digit product") {
    std::vector<int> num1 = {2};
    std::vector<int> num2 = {9, 9};
    CHECK(vector_to_integer(basic_multiply(num1, num2)) == 198);
  }
  SUBCASE("One 2 digit and one 1 digit number with 2 digit product") {
    std::vector<int> num1 = {3, 4};
    std::vector<int> num2 = {2};
    CHECK(vector_to_integer(basic_multiply(num1, num2)) == 68);
  }
}

TEST_CASE("Testing Karatsuba multiplication function") {
  SUBCASE("Test multiplying two small numbers") {
    std::vector<int> num1_1 = {1, 2};
    std::vector<int> num2_1 = {3, 8};
    std::vector<int> product_1 = {4, 5, 6};
    CHECK(vector_to_string(karatsuba(num1_1, num2_1)) ==
          vector_to_string(product_1));
  }
  SUBCASE("Test multiplying two numbers") {
    std::vector<int> num1_1 = {1, 2, 7, 8};
    std::vector<int> num2_1 = {3, 8, 1, 0};
    std::vector<int> product_1 = {4, 8, 6, 9, 1, 8, 0};
    CHECK(vector_to_string(karatsuba(num1_1, num2_1)) ==
          vector_to_string(product_1));
  }
  SUBCASE("Test multiplying two large numbers") {
    std::vector<int> num1_1 = {1, 2, 7, 8, 4, 2, 1, 5};
    std::vector<int> num2_1 = {3, 8, 1, 0, 5, 6, 7, 1};
    std::vector<int> product_1 = {4, 8, 7, 1, 5, 1, 0, 9, 0, 7, 8, 3, 2, 6, 5};
    CHECK(vector_to_string(karatsuba(num1_1, num2_1)) ==
          vector_to_string(product_1));
  }
}
