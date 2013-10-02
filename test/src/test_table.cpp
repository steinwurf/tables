#include <cstdint>

#include <gtest/gtest.h>

#include <tables/table.hpp>

TEST(TestTable, test_column_const_nonconst)
{
    tables::table::column non_const_column;
    EXPECT_FALSE(non_const_column.constant());
    tables::table::column const_column(42);
    EXPECT_TRUE(const_column.constant());
    ASSERT_DEATH({ const_column.set_value(0, 43); }, "Assertion `!m_constant' failed.");
}

TEST(TestTable, test_column)
{
/*
    boost::optional<size_t> type_hash() const;
    void set_type_hash(size_t type_hash);
    std::vector<boost::any> values() const;
    boost::any value(uint32_t index) const;
    void set_value(uint32_t index, const boost::any& value);
    void resize(uint32_t size);
*/
}

TEST(TestTable, test_table)
{

}
