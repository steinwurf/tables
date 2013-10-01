#include <cstdint>

#include <gtest/gtest.h>

#include <tables/csv_format.hpp>

TEST(TestFormat, test_csv_format)
{
    std::stringstream ss;
    tables::csv_format format;

    format.print(ss, bool(true));
    format.print(ss, int8_t(-1));
    format.print(ss, uint8_t(1));
    format.print(ss, int16_t(-1));
    format.print(ss, uint16_t(1));
    format.print(ss, int32_t(-1));
    format.print(ss, uint32_t(1));
    format.print(ss, int64_t(-1));
    format.print(ss, uint64_t(1));

    format.print(ss, float(-3.14));
    format.print(ss, double(3.14));

    format.print(ss, "test");
    format.print(ss, std::string("test"));

    std::vector<int8_t> v;
    v.push_back(-1);
    v.push_back(1);

    format.print(ss, v);

    EXPECT_EQ(ss.str(), "1-11-11-11-11-3.143.14testtest-1,1");
}
