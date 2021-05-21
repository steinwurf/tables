// Copyright (c) 2014 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include <tables/infix_ostream_iterator.hpp>

template <typename T>
void test_infix_ostream_iterator(std::vector<T> values,
                                 const std::string& delimiter,
                                 const std::string& expected_result)
{
    std::stringstream ss;
    tables::infix_ostream_iterator<T> printer(ss, delimiter.c_str());
    std::copy(values.begin(), values.end(), printer);
    EXPECT_EQ(expected_result, ss.str());
}

TEST(TestInfix, test_infix_ostream_iterator)
{
    static const uint32_t a_ints[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<uint32_t> v_ints(a_ints,
                                 a_ints + sizeof(a_ints) / sizeof(a_ints[0]));

    static const std::string a_strings[] = {"s1", "s2", "s3", "s4",
                                            "s5", "s6", "s7"};
    std::vector<std::string> v_strings(
        a_strings, a_strings + sizeof(a_strings) / sizeof(a_strings[0]));

    test_infix_ostream_iterator(v_ints, ",", "1,2,3,4,5,6,7,8,9");
    test_infix_ostream_iterator(v_ints, "¤", "1¤2¤3¤4¤5¤6¤7¤8¤9");

    test_infix_ostream_iterator(v_strings, ",", "s1,s2,s3,s4,s5,s6,s7");
    test_infix_ostream_iterator(v_strings, "¤", "s1¤s2¤s3¤s4¤s5¤s6¤s7");
}
