#include <cstdint>
#include <string>
#include <gtest/gtest.h>
#include <boost/any.hpp>

#include <tables/table.hpp>

TEST(TestTable, test_constructor)
{
    tables::table table;

    EXPECT_EQ(0, table.rows());
}

TEST(TestTable, test_add_column)
{
    tables::table table;

    table.add_column("column1");
    EXPECT_EQ(1, table.columns().size());
}

TEST(TestTable, test_add_const_column)
{
    tables::table table;

    table.add_const_column("column1", 42);
    EXPECT_EQ(1, table.columns().size());
}

TEST(TestTable, test_add_column_insert_zero)
{
    tables::table table;

    table.add_column("column1");
    table.add_row();
    table.set_value("column1", 0);
    EXPECT_EQ(1, table.columns().size());
}

TEST(TestTable, test_add_const_column_insert_zero)
{
    tables::table table;

    table.add_const_column("column1", 0);
    EXPECT_EQ(1, table.columns().size());
}

TEST(TestTable, test_columns)
{
    tables::table table;

    table.add_const_column("column1", 42);
    EXPECT_EQ(1, table.columns().size());
    table.add_column("column2");
    EXPECT_EQ(2, table.columns().size());
    table.add_const_column("column3", 42);
    EXPECT_EQ(3, table.columns().size());
    table.add_column("column4");
    EXPECT_EQ(4, table.columns().size());
    table.add_column("column5");
    EXPECT_EQ(5, table.columns().size());
    table.add_column("column6");
    EXPECT_EQ(6, table.columns().size());
    table.add_const_column("column7", std::string("42"));
    EXPECT_EQ(7, table.columns().size());
    table.add_const_column("column8", 88);
    EXPECT_EQ(8, table.columns().size());
}

TEST(TestTable, test_add_row)
{
    tables::table table;
    uint32_t rows = 10;
    for (uint32_t i = 0; i <= rows; ++i)
    {
        EXPECT_EQ(i, table.rows());
        table.add_row();
    }
    uint32_t value = 42;
    table.add_column("test");
    table.set_value("test", value);
    auto returned_value = table.columns().find("test")->second.value(rows);
    EXPECT_EQ(value, boost::any_cast<uint32_t>(returned_value));
    EXPECT_TRUE(table.columns().find("test")->second.value(rows-1).empty());
}

TEST(TestTable, test_merge)
{
    tables::table table1;
    tables::table table2;

    table1.add_column("t1");
    table2.add_column("t2");

    table1.add_column("t1_const");
    table2.add_column("t2_const");

    table1.add_column("common");
    table2.add_column("common");
    table1.add_column("common_const1");
    table2.add_column("common_const1");

    table1.add_column("common_const2");

    table2.add_column("common_const2");

    table1.add_row();
    table2.add_row();

    table1.set_value("t1", 1);
    table2.set_value("t2", 2);

    table1.set_value("common", 1);
    table2.set_value("common", 2);

    // table1:
    /**********************************************************
     * t1 * t1_const * common * common_const1 * common_const2 *
     * 1  * const    * 1      * 4             * 0             *
     **********************************************************/

    // table2:
    /**********************************************************
     * t2 * t2_const * common * common_const1 * common_const2 *
     * 2  * const    * 2      * 4             * 1             *
     *********************************************************/

    // table1 and table 2 should have similar dimensions.
    EXPECT_EQ(table1.columns().size(), table2.columns().size());
    EXPECT_EQ(table1.rows(), table2.rows());
    table1.merge(table2);
    // table1 merged with table2:
    /**************************************************************************
     * t1 * t2 * t1_const * t2_const * common * common_const1 * common_const2 *
     * 1  *    * const    *          * 1      * 4             * 0             *
     *    * 2  *          * const    * 2      * 4             * 1             *
     *************************************************************************/

    //After merge they should not.
    EXPECT_FALSE(table1.columns().size() == table2.columns().size());
    EXPECT_FALSE(table1.rows() == table2.rows());
    // It should have the following dimensions:
    EXPECT_EQ(7, table1.columns().size());
    EXPECT_EQ(2, table1.rows());

    EXPECT_TRUE(table1.has_column("t1"));
    auto& c1 =table1.columns().find("t1")->second;

    auto r2 = table1.columns().find("t2");
    EXPECT_NE(r2, table1.columns().end());
    auto& c2 =r2->second;

    auto r3 = table1.columns().find("t1_const");
    EXPECT_NE(r3, table1.columns().end());
    auto& c3 =r3->second;

    auto r4 = table1.columns().find("t2_const");
    EXPECT_NE(r4, table1.columns().end());
    auto& c4 =r4->second;

    auto r5 = table1.columns().find("common");
    EXPECT_NE(r5, table1.columns().end());
    auto& c5 =r5->second;

    auto r6 = table1.columns().find("common_const1");
    EXPECT_NE(r6, table1.columns().end());
    auto& c6 =r6->second;

    auto r7 = table1.columns().find("common_const2");
    EXPECT_NE(r7, table1.columns().end());
    auto& c7 =r7->second;

    std::cout << "before error" << std::endl;
    auto error_cause = c1.value(0);
    std::cout << "after error" << std::endl;
    auto compare_value = boost::any_cast<int32_t>(error_cause);

    EXPECT_EQ(1, compare_value);

    EXPECT_TRUE(c1.value(1).empty());

    EXPECT_TRUE(c2.value(0).empty());
    EXPECT_EQ(2, boost::any_cast<int>(c2.value(1)));

    EXPECT_EQ(std::string("const"), boost::any_cast<std::string>(c3.value(0)));
    EXPECT_EQ(std::string("const"), boost::any_cast<std::string>(c3.value(1)));

    EXPECT_EQ(std::string("const"), boost::any_cast<std::string>(c4.value(0)));
    EXPECT_EQ(std::string("const"), boost::any_cast<std::string>(c4.value(1)));

    EXPECT_EQ(1, boost::any_cast<int>(c5.value(0)));
    EXPECT_EQ(2, boost::any_cast<int>(c5.value(1)));

    EXPECT_EQ(4, boost::any_cast<int>(c6.value(0)));
    EXPECT_EQ(4, boost::any_cast<int>(c6.value(1)));

    EXPECT_EQ(0, boost::any_cast<int>(c7.value(0)));
    EXPECT_EQ(1, boost::any_cast<int>(c7.value(1)));
}

/*
void merge(const table& src);
uint32_t rows() const;
std::map<std::string, column> columns() const;
template<class T> bool is_column(const std::string &column_name) const;
bool is_column(const std::string& column_name, const std::type_info& type) const;
bool has_column(const std::string& column_name) const;
void drop_column(const std::string& column_name);
template<class T> std::vector<T> column_as(const std::string &column_name) const;
typedef std::map<std::string, column>::const_iterator const_iterator;
const_iterator begin() const;
const_iterator end() const;
*/