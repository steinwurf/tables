#include <cstdint>

#include <gtest/gtest.h>

#include <tables/json_format.hpp>
#include <tables/table.hpp>

TEST(TestPythonFormat, test_json_format)
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

    EXPECT_EQ(ss.str(), "true-11-11-11-11-3.143.14\"test\"\"test\"[-1,1]");


}

TEST(TestFormat, test_json_table_format)
{
    tables::table table;

    table.add_const_column("const_c1", uint32_t(99));
    table.add_const_column("const_c2", int8_t(127));
    table.add_const_column("const_c3", double(9.9));
    table.add_const_column("const_c4", std::string("test_const"));

    table.add_column("c1");
    table.add_column("c2");
    table.add_column("c3");
    table.add_column("c4");
    table.add_column("c5");

    table.add_row();
    table.set_value("c1", uint32_t(1));
    table.set_value("c2", int8_t(23));
    table.set_value("c3", double(2.3));
    table.set_value("c4", std::string("test1"));
    table.set_value("c5", true);

    table.add_row();
    table.set_value("c1", uint32_t(2));
    table.set_value("c2", int8_t(33));
    table.set_value("c3", double(3.3));
    table.set_value("c4", std::string("test2"));
    table.set_value("c5", false);

    table.add_row();
    table.set_value("c1", uint32_t(3));
    table.set_value("c2", int8_t(43));
    table.set_value("c3", double(4.3));
    table.set_value("c4", std::string("test3"));


    std::stringstream ss;
    tables::json_format format;

    format.print(ss, table);

    std::stringstream ss_expect;

    ss_expect << "{"
              <<    "\"c1\":[1,2,3],"
              <<    "\"c2\":[23,33,43],"
              <<    "\"c3\":[2.3,3.3,4.3],"
              <<    "\"c4\":[\"test1\",\"test2\",\"test3\"],"
              <<    "\"c5\":[true,false,null],"
              <<    "\"const_c1\":99,"
              <<    "\"const_c2\":127,"
              <<    "\"const_c3\":9.9,"
              <<    "\"const_c4\":\"test_const\""
              << "}";


    EXPECT_EQ(ss_expect.str(),ss.str());
}
