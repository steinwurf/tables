// Copyright (c) 2014 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include <tables/csv_format.hpp>

#include "format_test_helper.hpp"

TEST(TestCsvFormat, test_csv_format)
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

    EXPECT_EQ("1-11-11-11-11-3.143.14testtest-1;1", ss.str());
}

TEST(TestCsvFormat, test_nested_csv_format)
{
    std::stringstream ss;
    tables::csv_format fmt;

    std::vector<uint32_t> vc0;

    std::vector<uint32_t> vc1;
    vc1.push_back(uint32_t(24));

    std::vector<uint32_t> vc2;
    vc2.push_back(uint32_t(66));
    vc2.push_back(uint32_t(89));

    std::vector<std::vector<uint32_t>> vp;
    vp.push_back(vc0);
    vp.push_back(vc1);
    vp.push_back(vc2);

    fmt.print(ss, vp);

    EXPECT_EQ(";24;66;89", ss.str());
}

TEST(TestCsvFormat, test_csv_empty_table)
{
    tables::table table;
    tables::csv_format format;

    std::stringstream ss;
    std::stringstream ss_expect;

    // A single empty line is printed for an empty table
    ss_expect << std::endl;

    format.print(ss, table);

    EXPECT_EQ(ss_expect.str(), ss.str());
}

TEST(TestCsvFormat, test_csv_table_format)
{
    std::stringstream ss_expect;

    ss_expect << "c1,c2,c3,c4,c5,c6,const_c1,const_c2,const_c3,const_c4,"
              << "const_c5,const_c6" << std::endl
              << "1,23,2.3,test1,1,1;2;3;4;5,99,127,9.9,test_const,1,"
              << "1;2;3;4;5" << std::endl
              << "2,33,3.3,test2,0,,99,127,9.9,test_const,1,"
              << "1;2;3;4;5" << std::endl
              << "3,43,4.3,test3,,1;2;3;4;5;1337,99,127,9.9,test_const,1,"
              << "1;2;3;4;5" << std::endl;

    test_table_format(ss_expect.str(), tables::csv_format());
}
