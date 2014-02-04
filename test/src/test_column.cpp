#include <cstdint>
#include <string>
#include <gtest/gtest.h>
#include <boost/any.hpp>
#include <boost/shared_ptr.hpp>

#include <tables/column.hpp>
#include <tables/nonconst_column.hpp>
#include <tables/const_column.hpp>


TEST(TestColumn, test_column_const_nonconst)
{
    tables::nonconst_column nonconst_column;
    EXPECT_FALSE(nonconst_column.is_constant());
    tables::const_column const_column(std::string("42"));
    const_column.add_row();
    EXPECT_TRUE(const_column.is_constant());
    nonconst_column.add_row();
    nonconst_column.set_value(std::string("42"));
    EXPECT_EQ(boost::any_cast<std::string>(nonconst_column.value(0)),
              boost::any_cast<std::string>(const_column.value(0)));
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

template<class T> void test_nonconst_column_insert_and_retrieve(T value)
{
    uint32_t length = 10;
    for (uint32_t i = 0; i < length; ++i)
    {
        tables::nonconst_column column;

        // insert
        for (uint32_t j = 0; j < length; ++j)
        {
            column.add_row();
            if(j == i)
            {
                column.set_value(value);
            }
        }

        // retrieve
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

TEST(TestColumn, test_nonconst_column_insert_and_retrieve)
{
    // Integer
    test_nonconst_column_insert_and_retrieve(42);
    // String
    test_nonconst_column_insert_and_retrieve(std::string("42"));
    // Custom
    test_nonconst_column_insert_and_retrieve(Custom(43));
}

template<class T> void test_nonconst_column_default_insert_and_retrieve(T value,
    T default_value)
{
    uint32_t length = 10;
    for (uint32_t i = 0; i < length; ++i)
    {
        tables::nonconst_column column;
        column.set_default_value(default_value);
        // insert
        for (uint32_t j = 0; j < length; ++j)
        {
            column.add_row();
            if(j == i)
            {
                column.set_value(value);
            }
        }

        // retrieve
        for (uint32_t j = 0; j < length; ++j)
        {
            if(j == i)
            {
                EXPECT_EQ(value, boost::any_cast<T>(column.value(i)));
            }
            else
            {
                EXPECT_EQ(boost::any_cast<T>(column.value(j)), default_value);
            }
        }
    }
}

TEST(TestColumn, test_nonconst_column_default_insert_and_retrieve)
{
    // Integer
    test_nonconst_column_default_insert_and_retrieve(42, 43);
    // String
    test_nonconst_column_default_insert_and_retrieve(std::string("42"),
        std::string("43"));
    // Custom
    test_nonconst_column_default_insert_and_retrieve(Custom(43),
        Custom(44));
}

template<class T> void test_const_column_insert_and_retrieve(T value)
{
    uint32_t length = 10;

    // insert
    tables::const_column column1(value);
    for (uint32_t j = 0; j < length; ++j)
    {
        column1.add_row();
    }

    // retrieve
    for (uint32_t j = 0; j < length; ++j)
    {
        EXPECT_EQ(value, boost::any_cast<T>(column1.value(j)));
    }

    // insert
    tables::const_column column2(value, length);

    // retrieve
    for (uint32_t j = 0; j < length; ++j)
    {
        EXPECT_EQ(value, boost::any_cast<T>(column2.value(j)));
    }
}

TEST(TestColumn, test_const_column_insert_and_retrieve)
{
    // Integer
    test_const_column_insert_and_retrieve(42);
    // String
    test_const_column_insert_and_retrieve(std::string("42"));
    // Custom
    test_const_column_insert_and_retrieve(Custom(43));
}

template<class T> void test_const_hash_type_after_insert(T value)
{
    tables::const_column const_column(value);
    EXPECT_EQ(typeid(T).hash_code(), const_column.type_hash());
}

template<class T> void test_nonconst_hash_type_after_insert(T value)
{
    tables::nonconst_column nonconst_column;
    nonconst_column.add_row();
    nonconst_column.set_value(value);
    EXPECT_EQ(typeid(T).hash_code(), nonconst_column.type_hash());
}

TEST(TestColumn, test_const_hash_type_after_insert)
{
    // Integer
    test_const_hash_type_after_insert(42);
    // String
    test_const_hash_type_after_insert(std::string("42"));
    // Custom
    test_const_hash_type_after_insert(Custom(43));
}

TEST(TestColumn, test_nonconst_hash_type_after_insert)
{
    // Integer
    test_nonconst_hash_type_after_insert(42);
    // String
    test_nonconst_hash_type_after_insert(std::string("42"));
    // Custom
    test_nonconst_hash_type_after_insert(Custom(43));
}

template<class T> void test_values_after_insert(T value)
{
    uint32_t length = 10;
    for (uint32_t i = 0; i < length; ++i)
    {
        tables::nonconst_column nonconst_column;
        for (uint32_t j = 0; j < i; ++j)
        {
            nonconst_column.add_row();
            nonconst_column.set_value(value);
        }

        for (uint32_t j = 0; j < i; ++j)
        {
            EXPECT_EQ(nonconst_column.value(j), value);
        }
    }
}

template<class T> bool test_value_method(T value,
    boost::shared_ptr<tables::column> column, uint32_t rows)
{
    for (uint32_t row = 0; row < rows; row++)
    {
        if(column->value(row).empty() || boost::any_cast<T>(
            column->value(row)) != value)
            return false;
    }
    return true;
}

TEST(TestColumn, test_value_method)
{
    uint32_t column_size = 4;
    // CONST
    {
        // Integer
        auto value = 42;
        boost::shared_ptr<tables::const_column> const_column(
            new tables::const_column(value, column_size));
        EXPECT_TRUE(test_value_method(value, const_column, column_size));
    }
    {
        // String
        auto value = std::string("42");
        boost::shared_ptr<tables::const_column> const_column(
            new tables::const_column(value, column_size));
        EXPECT_TRUE(test_value_method(value, const_column, column_size));
    }
    {
        // Custom
        auto value = Custom(42);
        boost::shared_ptr<tables::const_column> const_column(
            new tables::const_column(value, column_size));
        EXPECT_TRUE(test_value_method(value, const_column, column_size));
    }

    // NON CONST
    {
        boost::shared_ptr<tables::nonconst_column> nonconst_column(
            new tables::nonconst_column());
        auto value = 42;
        nonconst_column->add_row();
        nonconst_column->set_value(value);
        EXPECT_TRUE(test_value_method(value, nonconst_column, 1));
        nonconst_column->add_row();
        EXPECT_TRUE(nonconst_column->value(1).empty());
    }
    // String
    {
        boost::shared_ptr<tables::nonconst_column> nonconst_column(
            new tables::nonconst_column());
        auto value = "42";
        nonconst_column->add_row();
        nonconst_column->set_value(value);
        EXPECT_TRUE(test_value_method(value, nonconst_column, 1));
        nonconst_column->add_row();
        EXPECT_TRUE(nonconst_column->value(1).empty());
    }
    // Custom
    {
        boost::shared_ptr<tables::nonconst_column> nonconst_column(
            new tables::nonconst_column());
        auto value = Custom(42);
        nonconst_column->add_row();
        nonconst_column->set_value(value);
        EXPECT_TRUE(test_value_method(value, nonconst_column, 1));
        nonconst_column->add_row();
        EXPECT_TRUE(nonconst_column->value(1).empty());
    }
}

template<class T> void test_nonconst_copy_constructor(T value)
{
    uint32_t length = 10;
    boost::shared_ptr<tables::const_column> const_column(
        new tables::const_column(value, length));
    tables::nonconst_column nonconst_column(
        (boost::shared_ptr<tables::column>) const_column);

    EXPECT_TRUE(const_column->is_constant());
    EXPECT_FALSE(nonconst_column.is_constant());
    EXPECT_EQ(nonconst_column.type_hash(), const_column->type_hash());
    for (uint32_t i = 0; i < length; ++i)
    {
        EXPECT_EQ(
            boost::any_cast<T>(const_column->value(i)),
            boost::any_cast<T>(nonconst_column.value(i)));
    }
}

TEST(TestColumn, test_nonconst_copy_constructor)
{
    test_nonconst_copy_constructor(435);
    test_nonconst_copy_constructor("hi");
    test_nonconst_copy_constructor(Custom(33));
}
