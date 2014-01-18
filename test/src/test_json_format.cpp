#include <cstdint>
#include <sstream>

#include <gtest/gtest.h>

#include <tables/json_format.hpp>

#include "format_test_helper.hpp"


TEST(TestJsonFormat, test_json_format)
{
    std::stringstream ss;
    tables::json_format fmt;

    fmt.print(ss, bool(true));
    fmt.print(ss, int8_t(-1));
    fmt.print(ss, uint8_t(1));
    fmt.print(ss, int16_t(-1));
    fmt.print(ss, uint16_t(1));
    fmt.print(ss, int32_t(-1));
    fmt.print(ss, uint32_t(1));
    fmt.print(ss, int64_t(-1));
    fmt.print(ss, uint64_t(1));

    fmt.print(ss, float(-3.14));
    fmt.print(ss, double(3.14));

    fmt.print(ss, "test");
    fmt.print(ss, std::string("test"));

    std::vector<int8_t> v;
    v.push_back(-1);
    v.push_back(1);

    fmt.print(ss, v);

    EXPECT_EQ("true-11-11-11-11-3.143.14\"test\"\"test\"[-1,1]", ss.str());
}

template<typename T>
void test_nested_json_format(const T& child1_value, const T& child2_value,
    const std::string& expected)
{
    std::stringstream ss;
    tables::json_format fmt;

    std::vector<T> vc0;

    std::vector<T> vc1;
    vc1.push_back(T(child1_value));

    std::vector<T> vc2;
    vc2.push_back(child2_value);
    vc2.push_back(child2_value);

    std::vector<std::vector<T>> vp;
    vp.push_back(vc0);
    vp.push_back(vc1);
    vp.push_back(vc2);

    fmt.print(ss, vp);

    EXPECT_EQ(expected, ss.str());
}

TEST(TestJsonFormat, test_nested_json_format)
{
    test_nested_json_format(uint32_t(12), uint32_t(49), "[[],[12],[49,49]]");
    test_nested_json_format(
        std::string("12"), std::string("49"), "[[],[\"12\"],[\"49\",\"49\"]]");
}

TEST(TestJsonFormat, test_json_table_format)
{
    std::stringstream ss_expect;

    ss_expect << "{"
              <<    "\"c1\":[1,2,3],"
              <<    "\"c2\":[23,33,43],"
              <<    "\"c3\":[2.3,3.3,4.3],"
              <<    "\"c4\":[\"test1\",\"test2\",\"test3\"],"
              <<    "\"c5\":[true,false,null],"
              <<    "\"c6\":[[1,2,3,4,5],null,[1,2,3,4,5,1337]],"
              <<    "\"const_c1\":99,"
              <<    "\"const_c2\":127,"
              <<    "\"const_c3\":9.9,"
              <<    "\"const_c4\":\"test_const\","
              <<    "\"const_c5\":true,"
              <<    "\"const_c6\":[1,2,3,4,5]"
              << "}";

    test_table_format(ss_expect.str(), tables::json_format());
}
