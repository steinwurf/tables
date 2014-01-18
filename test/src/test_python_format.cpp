#include <cstdint>

#include <gtest/gtest.h>

#include <tables/python_format.hpp>

#include "format_test_helper.hpp"

TEST(TestPythonFormat, test_python_format)
{
    std::stringstream ss;
    tables::python_format fmt;

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

    EXPECT_EQ("True-11-11-11-11-3.143.14'test''test'[-1,1]", ss.str());
}

TEST(TestPythonFormat, test_python_table_format)
{
    std::stringstream ss_expect;

    ss_expect << "{"
              <<    "'c1':[1,2,3],"
              <<    "'c2':[23,33,43],"
              <<    "'c3':[2.3,3.3,4.3],"
              <<    "'c4':['test1','test2','test3'],"
              <<    "'c5':[True,False,None],"
              <<    "'c6':[[1,2,3,4,5],None,[1,2,3,4,5,1337]],"
              <<    "'const_c1':99,"
              <<    "'const_c2':127,"
              <<    "'const_c3':9.9,"
              <<    "'const_c4':'test_const',"
              <<    "'const_c5':True,"
              <<    "'const_c6':[1,2,3,4,5]"
              << "}";

    test_table_format(ss_expect.str(), tables::python_format());
}
