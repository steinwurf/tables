// Copyright (c) 2014 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <map>
#include <utility>
#include <vector>
#include <string>

#include "const_column.hpp"
#include "nonconst_column.hpp"
#include "table.hpp"

namespace tables
{
table::table() :
    m_rows(0)
{ }

void table::add_column(const std::string& column_name)
{
    assert(!has_column(column_name));
    m_columns.insert(std::pair<std::string, column_ptr>(
        column_name, column_ptr(new nonconst_column(m_rows))));
}

void table::add_const_column(const std::string& column_name,
                             const boost::any& value)
{
    // You can't add a const column which already exists.
    assert(!has_column(column_name));

    m_columns.insert(
        std::pair<std::string, column_ptr>(
            column_name, column_ptr(new const_column(value, m_rows))));
}

void table::set_value(const std::string& column_name,
                      const boost::any& value)
{
    // You can not insert a value without a row to put it in.
    assert(m_rows > 0);

    // You must create the column before you insert values
    assert(has_column(column_name));

    // You cannot insert values into a constant column
    assert(!is_constant(column_name));

    auto& c = m_columns.at(column_name);
    c->set_value(value);
    assert(value.empty() ||
           c->type_hash() == value.type().hash_code());
}

void table::set_default_value(const std::string& column_name,
                              const boost::any& value)
{
    // You must create the column before you set the default value
    assert(has_column(column_name));

    // You cannot set the default value on a constant column
    assert(!is_constant(column_name));

    auto& c = m_columns.at(column_name);
    c->set_default_value(value);
    assert(value.empty() ||
           c->type_hash() == value.type().hash_code());
}

void table::add_row()
{
    ++m_rows;

    for (auto& kv : m_columns)
    {
        kv.second->add_row();
    }
}

void table::merge(const table& src)
{
    // if two tables are merged, the const of a const_column no longer
    // applies.
    std::map<std::string, column_ptr> converted_columns;
    for (auto& my_kv : m_columns)
    {
        if (my_kv.second->is_constant())
        {
            converted_columns.insert(
                std::pair<std::string, column_ptr>(
                    my_kv.first,
                    column_ptr(new nonconst_column(my_kv.second))));
        }
    }

    for (auto& converted_column : converted_columns)
    {
        m_columns.erase(converted_column.first);
        m_columns.insert(std::pair<std::string, column_ptr>(
            converted_column.first, converted_column.second));
    }

    for (uint32_t i = 0; i < src.rows(); ++i)
    {
        add_row();
        for (const auto& kv : src.m_columns)
        {
            auto name = kv.first;

            if (!has_column(name))
                add_column(name);

            auto column = kv.second;

            set_value(name, column->value(i));
        }
    }
}

void table::drop_column(const std::string& column_name)
{
    assert(has_column(column_name));
    m_columns.erase(column_name);
}

uint32_t table::rows() const
{
    return m_rows;
}

std::vector<std::string> table::columns() const
{
    std::vector<std::string> column_names;
    for (auto& kv : m_columns)
    {
        column_names.push_back(kv.first);
    }
    return column_names;
}

boost::any table::value(const std::string& column_name,
                        uint32_t row_index) const
{
    assert(has_column(column_name));
    return m_columns.at(column_name)->value(row_index);
}

std::vector<boost::any> table::values(const std::string& column_name) const
{
    assert(has_column(column_name));
    return m_columns.at(column_name)->values();
}

boost::any table::default_value(const std::string& column_name) const
{
    assert(has_column(column_name));
    return m_columns.at(column_name)->default_value();
}

bool table::is_constant(const std::string& column_name) const
{
    assert(has_column(column_name));
    return m_columns.at(column_name)->is_constant();
}

uint32_t table::empty_rows(const std::string& column_name) const
{
    assert(has_column(column_name));
    return m_columns.at(column_name)->empty_rows();
}

bool table::is_column(const std::string& column_name,
                      const std::type_info& type) const
{
    assert(has_column(column_name));

    const auto& c = m_columns.at(column_name);
    assert(c->type_hash());
    return (*c->type_hash()) == type.hash_code();
}

bool table::has_column(const std::string& column_name) const
{
    return m_columns.find(column_name) != m_columns.end();
}

table::column_name_iterator table::begin() const
{
    return m_columns.cbegin();
}

table::column_name_iterator table::end() const
{
    return m_columns.cend();
}
}
