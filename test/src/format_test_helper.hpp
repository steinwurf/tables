#pragma once

#include <sstream>
#include <cstdint>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include <tables/format.hpp>
#include <tables/table.hpp>


inline void test_table_format(const tables::format& format,
                              const std::string& expect)
{
    tables::table table;

    std::vector<uint32_t> c6_data;
    c6_data.push_back(uint32_t(1));
    c6_data.push_back(uint32_t(2));
    c6_data.push_back(uint32_t(3));
    c6_data.push_back(uint32_t(4));
    c6_data.push_back(uint32_t(5));

    table.add_const_column("const_c1", uint32_t(99));
    table.add_const_column("const_c2", int8_t(127));
    table.add_const_column("const_c3", double(9.9));
    table.add_const_column("const_c4", std::string("test_const"));
    table.add_const_column("const_c5", true);
    table.add_const_column("const_c6", c6_data);

    table.add_column("c1");
    table.add_column("c2");
    table.add_column("c3");
    table.add_column("c4");
    table.add_column("c5");
    table.add_column("c6");

    table.add_row();
    table.set_value("c1", uint32_t(1));
    table.set_value("c2", int8_t(23));
    table.set_value("c3", double(2.3));
    table.set_value("c4", std::string("test1"));
    table.set_value("c5", true);
    table.set_value("c6", c6_data);

    table.add_row();
    table.set_value("c1", uint32_t(2));
    table.set_value("c2", int8_t(33));
    table.set_value("c3", double(3.3));
    table.set_value("c4", std::string("test2"));
    table.set_value("c5", false);

    c6_data.push_back(1337);

    table.add_row();
    table.set_value("c1", uint32_t(3));
    table.set_value("c2", int8_t(43));
    table.set_value("c3", double(4.3));
    table.set_value("c4", std::string("test3"));
    table.set_value("c6", c6_data);


    std::stringstream ss;

    format.print(ss, table);

    EXPECT_EQ(expect, ss.str());
}
