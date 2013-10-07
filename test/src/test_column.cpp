#include <cstdint>
#include <string>
#include <gtest/gtest.h>
#include <boost/any.hpp>

#include <tables/table.hpp>

TEST(TestColumn, test_column_const_nonconst)
{
    tables::table::column column;
    EXPECT_FALSE(column.constant());
    tables::table::column const_column(42);
    EXPECT_TRUE(const_column.constant());
    column.resize(1);
    column.set_value(0, 43);
}

template<class T> void test_column_insert_and_retrieve(T value)
{
    uint32_t length = 10;
    for (uint32_t i = 0; i < length; ++i)
    {
        tables::table::column column;
        column.resize(length);
        column.set_value(i, value);
        for (uint32_t j = 0; j < length; ++j)
        {
            if(j == i)
            {
                EXPECT_EQ(value, boost::any_cast<T>(column.value(i)));
            }
            else
            {
                EXPECT_TRUE(column.value(j).empty());
            }
        }
    }
}

class Custom
{
public:
    Custom(uint32_t value)
    {
        m_value = value;
    }

    bool operator== (const Custom& other) const {
        return m_value == other.m_value;
    }

    bool operator != (const Custom& other) const {
        return !(m_value == other.m_value);
    }

private:
    uint32_t m_value;
};

TEST(TestColumn, test_column_insert_and_retrieve)
{
    // Integer
    test_column_insert_and_retrieve(42);
    // String
    test_column_insert_and_retrieve(std::string("42"));
    // Custom
    test_column_insert_and_retrieve(Custom(43));
}

template<class T> void test_hash_type_after_insert(T value)
{
    tables::table::column column;
    column.resize(1);
    column.set_value(0, value);
    EXPECT_EQ(typeid(T).hash_code(), column.type_hash());
}

TEST(TestColumn, test_hash_type_after_insert)
{
    // Integer
    test_hash_type_after_insert(42);
    // String
    test_hash_type_after_insert(std::string("42"));
    // Custom
    test_hash_type_after_insert(Custom(43));
}

template<class T> void test_hash_type_after_const_creation(T value)
{
    tables::table::column column(value);
    EXPECT_EQ(typeid(T).hash_code(), column.type_hash());
}

TEST(TestColumn, test_hash_type_after_const_creation)
{
    // Integer
    test_hash_type_after_insert(42);
    // String
    test_hash_type_after_insert(std::string("42"));
    // Custom
    test_hash_type_after_insert(Custom(43));
}

template<class T> void test_values_after_insert(T value)
{
    uint32_t length = 10;
    for (uint32_t i = 0; i < length; ++i)
    {
        tables::table::column column;
        column.resize(i);
        EXPECT_EQ(i, column.values().size());
        for (uint32_t j = 0; j < i; ++j)
        {
            column.set_value(i, value);
        }
        for (uint32_t j = 0; j < i; ++j)
        {
            EXPECT_EQ(column.value(j), value);
        }
    }
}

TEST(TestColumn, test_values_after_insert)
{
    // Integer
    test_hash_type_after_insert(42);
    // String
    test_hash_type_after_insert(std::string("42"));
    // Custom
    test_hash_type_after_insert(Custom(43));
}

template<class T> bool test_value_method(T value,
    tables::table::column column, uint32_t rows)
{
    for (uint32_t row = 0; row < rows; ++row)
    {
        if(column.value(row).empty() || boost::any_cast<T>(
            column.value(row)) != value)
            return false;
    }
    return true;
}

TEST(TestColumn, test_value_method)
{
    // CONST
    {
        // Integer
        auto value = 42;
        tables::table::column const_column(value);
        EXPECT_TRUE(test_value_method(value, const_column, 10));
    }
    {
        // String
        auto value = std::string("42");
        tables::table::column const_column(value);
        EXPECT_TRUE(test_value_method(value, const_column, 10));
    }
    {
        // Custom
        auto value = Custom(42);
        tables::table::column const_column(value);
        EXPECT_TRUE(test_value_method(value, const_column, 10));
    }

    // NON CONST
    {
        tables::table::column column;
        column.resize(1);
        auto value = 42;
        column.set_value(0, value);
        EXPECT_TRUE(test_value_method(value, column, 1));
        column.resize(2);
        EXPECT_FALSE(test_value_method(value, column, 2));
    }
    // String
    {
        tables::table::column nonconst_column;
        nonconst_column.resize(1);
        auto value = std::string("42");
        nonconst_column.set_value(0, value);
        // Integer
        EXPECT_TRUE(test_value_method(value, nonconst_column, 1));
        nonconst_column.resize(2);
        EXPECT_FALSE(test_value_method(value, nonconst_column, 2));
    }
    // Custom
    {
        tables::table::column nonconst_column;
        nonconst_column.resize(1);
        auto value = Custom(43);
        nonconst_column.set_value(0, value);
        // Integer
        EXPECT_TRUE(test_value_method(value, nonconst_column, 1));
        nonconst_column.resize(2);
        EXPECT_FALSE(test_value_method(value, nonconst_column, 2));
    }
}
