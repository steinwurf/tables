#include <cstdint>
#include <string>
#include <vector>
#include <gtest/gtest.h>
#include <boost/any.hpp>

#include <tables/table.hpp>

TEST(TestTable, test_constructor)
{
    tables::table table;

    EXPECT_EQ(uint32_t(0), table.rows());
}

TEST(TestTable, test_set_value)
{
    tables::table table;
    table.add_row();
    table.add_column("column1");
    table.set_value("column1", 42);
    EXPECT_EQ(uint64_t(1), table.columns().size());
    EXPECT_EQ(std::string("column1"), table.columns()[0]);
}

TEST(TestTable, test_add_const_column)
{
    tables::table table;
    table.add_const_column("column1", 42);
    EXPECT_EQ(uint64_t(1), table.columns().size());
}

TEST(TestTable, test_add_column_insert_zero)
{
    tables::table table;
    table.add_row();
    table.add_column("column1");
    table.set_value("column1", 0);
    EXPECT_EQ(uint64_t(1), table.columns().size());
    EXPECT_EQ(0, boost::any_cast<int>(table.value("column1", 0)));
    EXPECT_EQ(std::string("column1"), table.columns()[0]);
}

TEST(TestTable, test_add_const_column_insert_zero)
{
    tables::table table;
    table.add_row();
    table.add_const_column("column1", 0);
    EXPECT_EQ(uint64_t(1), table.columns().size());
    EXPECT_EQ(0, boost::any_cast<int>(table.value("column1", 0)));
    EXPECT_EQ(std::string("column1"), table.columns()[0]);
}

TEST(TestTable, test_columns)
{
    tables::table table;
    table.add_row();
    table.add_const_column("column1", 42);
    EXPECT_EQ(uint64_t(1), table.columns().size());
    table.add_column("column2");
    table.set_value("column2", 1337);
    EXPECT_EQ(uint64_t(2), table.columns().size());
    table.add_const_column("column3", 42);
    EXPECT_EQ(uint64_t(3), table.columns().size());
    table.add_column("column4");
    table.set_value("column4", 1337);
    EXPECT_EQ(uint64_t(4), table.columns().size());
    table.add_column("column5");
    table.set_value("column5", 1337);
    EXPECT_EQ(uint64_t(5), table.columns().size());
    table.add_column("column6");
    table.set_value("column6", 1337);
    EXPECT_EQ(uint64_t(6), table.columns().size());
    table.add_const_column("column7", std::string("42"));
    EXPECT_EQ(uint64_t(7), table.columns().size());
    table.add_const_column("column8", 88);
    EXPECT_EQ(uint64_t(8), table.columns().size());
}

TEST(TestTable, test_add_row)
{
    tables::table table;
    uint32_t rows = 10;
    for (uint32_t i = 0; i < rows; ++i)
    {
        EXPECT_EQ(i, table.rows());
        table.add_row();
    }
    uint32_t value = 42;
    table.add_column("test");
    table.set_value("test", value);
    auto returned_value = table.value("test", rows-1);
    EXPECT_EQ(value, boost::any_cast<uint32_t>(returned_value));
    EXPECT_TRUE(table.value("test", rows-2).empty());

    // test values()
    EXPECT_EQ(rows, table.values("test").size());
    for (uint32_t i = 0; i < rows-1; ++i)
    {
        EXPECT_TRUE(table.values("test")[i].empty());
    }
    EXPECT_EQ(rows-1, table.empty_rows("test"));
    EXPECT_EQ(value, boost::any_cast<uint32_t>(table.values("test")[rows-1]));
}

TEST(TestTable, test_merge)
{
    tables::table table1;
    tables::table table2;

    table1.add_column("t1");
    table1.add_const_column("t1_const", std::string("const"));
    table1.add_column("common");
    table1.set_default_value("common", uint32_t(75));
    table1.add_const_column("common_const1", uint32_t(4));
    table1.add_const_column("common_const2", uint32_t(0));

    table2.add_column("t2");
    table2.add_const_column("t2_const", std::string("const"));
    table2.add_column("common");
    table2.add_const_column("common_const1", uint32_t(4));
    table2.add_const_column("common_const2", uint32_t(1));

    table1.add_row();
    table2.add_row();

    table1.set_value("t1", uint32_t(1));
    table2.set_value("t2", uint32_t(2));

    table1.set_value("common", uint32_t(1));

    
    table1.add_row();
    table2.add_row();

    table2.set_value("common", uint32_t(2));

    // table1:
    /**********************************************************
     * t1 * t1_const * common * common_const1 * common_const2 *
     * 1  * const    * 1      * 4             * 0             *
     *    * const    * 75     * 4             * 0             *     
     **********************************************************/

    // table2:
    /**********************************************************
     * t2 * t2_const * common * common_const1 * common_const2 *
     * 2  * const    *        * 4             * 1             *
     *    * const    * 2      * 4             * 1             *
     *********************************************************/

    // table1 and table 2 should have similar dimensions.
    EXPECT_EQ(table1.columns().size(), table2.columns().size());
    EXPECT_EQ(table1.rows(), table2.rows());
    table1.merge(table2);
    // table1 merged with table2:
    /**************************************************************************
     * t1 * t2 * t1_const * t2_const * common * common_const1 * common_const2 *
     * 1  *    * const    *          * 1      * 4             * 0             *
     *    *    * const    *          * 75     * 4             * 0             *
     *    * 2  *          * const    *        * 4             * 1             *
     *    *    *          * const    * 2      * 4             * 1             *
     *************************************************************************/

    //After merge they should not.
    EXPECT_FALSE(table1.columns().size() == table2.columns().size());
    EXPECT_FALSE(table1.rows() == table2.rows());
    // It should have the following dimensions:
    EXPECT_EQ(uint64_t(7), table1.columns().size());
    EXPECT_EQ(uint64_t(4), table1.rows());

    EXPECT_TRUE(table1.has_column("t1"));
    EXPECT_TRUE(table1.has_column("t2"));
    EXPECT_TRUE(table1.has_column("t1_const"));
    EXPECT_TRUE(table1.has_column("t2_const"));
    EXPECT_TRUE(table1.has_column("common"));
    EXPECT_TRUE(table1.has_column("common_const1"));
    EXPECT_TRUE(table1.has_column("common_const2"));

    // t1
    EXPECT_EQ(uint32_t(1), boost::any_cast<uint32_t>(table1.value("t1", 0)));
    EXPECT_TRUE(table1.value("t1", 1).empty());
    EXPECT_TRUE(table1.value("t1", 2).empty());
    EXPECT_TRUE(table1.value("t1", 3).empty());
    // t2
    EXPECT_TRUE(table1.value("t2", 0).empty());
    EXPECT_TRUE(table1.value("t2", 1).empty());
    EXPECT_EQ(uint32_t(2), boost::any_cast<uint32_t>(table1.value("t2", 2)));
    EXPECT_TRUE(table1.value("t2", 3).empty());
    // t1_const
    EXPECT_EQ(std::string("const"), boost::any_cast<std::string>(
        table1.value("t1_const", 0)));
    EXPECT_EQ(std::string("const"), boost::any_cast<std::string>(
        table1.value("t1_const", 1)));
    EXPECT_TRUE(table1.value("t1_const", 2).empty());
    EXPECT_TRUE(table1.value("t1_const", 3).empty());
    // t2_const
    EXPECT_TRUE(table1.value("t2_const", 0).empty());
    EXPECT_TRUE(table1.value("t2_const", 1).empty());
    EXPECT_EQ(std::string("const"), boost::any_cast<std::string>(
        table1.value("t2_const", 2)));
    EXPECT_EQ(std::string("const"), boost::any_cast<std::string>(
        table1.value("t2_const", 3)));
    // common
    EXPECT_EQ(uint32_t(1), boost::any_cast<uint32_t>(table1.value("common", 0)));
    EXPECT_EQ(uint32_t(75), boost::any_cast<uint32_t>(table1.value("common", 1)));
    EXPECT_TRUE(table1.value("common", 2).empty());
    EXPECT_EQ(uint32_t(2), boost::any_cast<uint32_t>(table1.value("common", 3)));

    // common_const1
    EXPECT_EQ(uint32_t(4), boost::any_cast<uint32_t>(table1.value("common_const1", 0)));
    EXPECT_EQ(uint32_t(4), boost::any_cast<uint32_t>(table1.value("common_const1", 1)));
    EXPECT_EQ(uint32_t(4), boost::any_cast<uint32_t>(table1.value("common_const1", 2)));
    EXPECT_EQ(uint32_t(4), boost::any_cast<uint32_t>(table1.value("common_const1", 3)));

    // common_const2
    EXPECT_EQ(uint32_t(0), boost::any_cast<uint32_t>(table1.value("common_const2", 0)));
    EXPECT_EQ(uint32_t(0), boost::any_cast<uint32_t>(table1.value("common_const2", 1)));
    EXPECT_EQ(uint32_t(1), boost::any_cast<uint32_t>(table1.value("common_const2", 2)));
    EXPECT_EQ(uint32_t(1), boost::any_cast<uint32_t>(table1.value("common_const2", 3)));
}

TEST(TestTable, test_is_column)
{
    tables::table table;

    table.add_const_column("const_c1", uint32_t(99));
    table.add_const_column("const_c2", int8_t(127));
    table.add_const_column("const_c3", double(9.9));
    table.add_const_column("const_c4", std::string("test_const"));

    table.add_row();

    table.add_column("c1");
    table.set_default_value("c1", uint32_t());
    table.add_column("c2");
    table.set_default_value("c2", int8_t());
    table.add_column("c3");
    table.set_default_value("c3", double());
    table.add_column("c4");
    table.set_default_value("c4", std::string());

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

    EXPECT_TRUE(table.is_column<uint32_t>("c1"));
    EXPECT_FALSE(table.is_column<int8_t>("c1"));
    EXPECT_FALSE(table.is_column<double>("c1"));
    EXPECT_FALSE(table.is_column<std::string>("c1"));

    EXPECT_FALSE(table.is_column<uint32_t>("c2"));
    EXPECT_TRUE(table.is_column<int8_t>("c2"));
    EXPECT_FALSE(table.is_column<double>("c2"));
    EXPECT_FALSE(table.is_column<std::string>("c2"));

    EXPECT_FALSE(table.is_column<uint32_t>("c3"));
    EXPECT_FALSE(table.is_column<int8_t>("c3"));
    EXPECT_TRUE(table.is_column<double>("c3"));
    EXPECT_FALSE(table.is_column<std::string>("c3"));

    EXPECT_FALSE(table.is_column<uint32_t>("c4"));
    EXPECT_FALSE(table.is_column<int8_t>("c4"));
    EXPECT_FALSE(table.is_column<double>("c4"));
    EXPECT_TRUE(table.is_column<std::string>("c4"));

    EXPECT_TRUE(table.is_column<uint32_t>("const_c1"));
    EXPECT_FALSE(table.is_column<int8_t>("const_c1"));
    EXPECT_FALSE(table.is_column<double>("const_c1"));
    EXPECT_FALSE(table.is_column<std::string>("const_c1"));

    EXPECT_FALSE(table.is_column<uint32_t>("const_c2"));
    EXPECT_TRUE(table.is_column<int8_t>("const_c2"));
    EXPECT_FALSE(table.is_column<double>("const_c2"));
    EXPECT_FALSE(table.is_column<std::string>("const_c2"));

    EXPECT_FALSE(table.is_column<uint32_t>("const_c3"));
    EXPECT_FALSE(table.is_column<int8_t>("const_c3"));
    EXPECT_TRUE(table.is_column<double>("const_c3"));
    EXPECT_FALSE(table.is_column<std::string>("const_c3"));

    EXPECT_FALSE(table.is_column<uint32_t>("const_c4"));
    EXPECT_FALSE(table.is_column<int8_t>("const_c4"));
    EXPECT_FALSE(table.is_column<double>("const_c4"));
    EXPECT_TRUE(table.is_column<std::string>("const_c4"));

    std::vector<uint32_t> vuint32_t = table.values_as<uint32_t>("c1");
    std::vector<int8_t> vint8_t = table.values_as<int8_t>("c2");
    std::vector<double> vdouble = table.values_as<double>("c3");
    std::vector<std::string> vstring = table.values_as<std::string>("c4");

    std::vector<uint32_t> vconstuint32_t = table.values_as<uint32_t>("const_c1");
    std::vector<int8_t> vconstint8_t = table.values_as<int8_t>("const_c2");
    std::vector<double> vconstdouble = table.values_as<double>("const_c3");
    std::vector<std::string> vconststring = table.values_as<std::string>(
        "const_c4");

    EXPECT_EQ(uint64_t(3), vuint32_t.size());
    EXPECT_EQ(uint64_t(3), vint8_t.size());
    EXPECT_EQ(uint64_t(3), vdouble.size());
    EXPECT_EQ(uint64_t(3), vstring.size());

    EXPECT_EQ(uint64_t(3), vconstuint32_t.size());
    EXPECT_EQ(uint64_t(3), vconstint8_t.size());
    EXPECT_EQ(uint64_t(3), vconstdouble.size());
    EXPECT_EQ(uint64_t(3), vconststring.size());
}

TEST(TestTable, test_drop_column)
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

    table.add_row();
    table.set_value("c1", uint32_t(1));
    table.set_value("c2", int8_t(23));
    table.set_value("c3", double(2.3));
    table.set_value("c4", std::string("test1"));

    EXPECT_EQ(uint64_t(8), table.columns().size());
    table.drop_column("const_c1");
    EXPECT_EQ(uint64_t(7), table.columns().size());
    table.drop_column("c1");
    EXPECT_EQ(uint64_t(6), table.columns().size());
}

TEST(TestTable, test_table_iterator)
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

    table.add_row();
    table.set_value("c1", uint32_t(1));
    table.set_value("c2", int8_t(23));
    table.set_value("c3", double(2.3));
    table.set_value("c4", std::string("test1"));
    uint32_t counter = 0;
    for(const auto& c_name : table)
    {
        EXPECT_EQ(table.columns()[counter].size(), c_name.size());
        EXPECT_EQ(table.columns()[counter], c_name);
        counter++;
    }
}
