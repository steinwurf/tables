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

    table1.add_column("table1");
    table2.add_column("table2");

    table1.add_const_column("table1_const", "const");    
    table2.add_const_column("table2_const", "const");
    
    table1.add_column("common");
    table2.add_column("common");

    table1.add_const_column("common_const", 4);
    table2.add_const_column("common_const", 4);

    table1.add_row();
    table2.add_row();

    table1.set_value("table1", 1);
    table2.set_value("table2", 2);
    
    table1.set_value("common", 1);
    table2.set_value("common", 2);
    
    EXPECT_EQ(table1.columns().size(), table2.columns().size());
    EXPECT_EQ(table1.rows(), table2.rows());    

    table1.merge(table2);

    EXPECT_FALSE(table1.columns().size() == table2.columns().size());
    EXPECT_FALSE(table1.rows() == table2.rows());    


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