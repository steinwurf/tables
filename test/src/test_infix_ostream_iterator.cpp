#include <cstdint>
#include <string>
#include <vector>
#include <gtest/gtest.h>
#include <tables/infix_ostream_iterator.hpp>

template<typename T>
void test_infix_ostream_iterator(std::vector<T> values,
    const std::string& delimiter, const std::string& expected_result)
{
    std::stringstream ss;
    tables::infix_ostream_iterator<T> printer(ss, delimiter.c_str());
    std::copy (values.begin(), values.end(), printer );
    EXPECT_EQ(ss.str(), expected_result);
}

TEST(TestFormat, test_infix_ostream_iterator)
{
    std::vector<uint32_t> v_ints {1,2,3,4,5,6,7,8,9};
    std::vector<std::string> v_strings {"s1","s2","s3","s4","s5","s6","s7"};

    test_infix_ostream_iterator(v_ints, ",", "1,2,3,4,5,6,7,8,9");
    test_infix_ostream_iterator(v_ints, "¤", "1¤2¤3¤4¤5¤6¤7¤8¤9");

    test_infix_ostream_iterator(v_strings, ",", "s1,s2,s3,s4,s5,s6,s7");
    test_infix_ostream_iterator(v_strings, "¤", "s1¤s2¤s3¤s4¤s5¤s6¤s7");
}
