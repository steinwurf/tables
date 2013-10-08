#include <cstdint>
#include <gtest/gtest.h>
#include <tables/csv_format.hpp>
#include <tables/table.hpp>

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

TEST(TestFormat, test_csv_table_format)
{
    tables::table table;

    table.set_const_value("const_c1", uint32_t(99));
    table.set_const_value("const_c2", int8_t(127));
    table.set_const_value("const_c3", double(9.9));
    table.set_const_value("const_c4", std::string("test_const"));

    table.add_row();
    table.set_value("c1", uint32_t(1));
    table.set_value("c2", int8_t(23));
    table.set_value("c3", double(2.3));
    table.set_value("c4", std::string("test1"));

    table.add_row();
    table.set_value("c1", uint32_t(2));
    table.set_value("c2", int8_t(33));
    table.set_value("c3", double(3.3));
    table.set_value("c4", std::string("test2"));

    table.add_row();
    table.set_value("c1", uint32_t(3));
    table.set_value("c2", int8_t(43));
    table.set_value("c3", double(4.3));
    table.set_value("c4", std::string("test3"));

    std::stringstream ss;
    tables::csv_format format;

    format.print(ss, table);

    std::stringstream ss_expect;

    ss_expect << "c1,c2,c3,c4,const_c1,const_c2,const_c3,const_c4" << std::endl
              << "1,23,2.3,test1,99,127,9.9,test_const" << std::endl
              << "2,33,3.3,test2,99,127,9.9,test_const" << std::endl
              << "3,43,4.3,test3,99,127,9.9,test_const" << std::endl;


    EXPECT_EQ(ss_expect.str(),ss.str());
}