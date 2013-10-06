#include "table.hpp"

#include <cassert>
#include <iomanip>

namespace tables
{

    table::column::column()
        : m_constant(false), m_type_set(false)
    { }

    table::column::column(const boost::any& const_value)
        : m_constant(true), m_type_set(true),
        m_type_hash(const_value.type().hash_code())
    {
        m_values.resize(1, const_value);
    }

    bool table::column::constant() const
    {
        return m_constant;
    }

    boost::optional<size_t> table::column::type_hash() const
    {
        return m_type_hash;
    }

    std::vector<boost::any> table::column::values() const
    {
        // It doesn't make sense to get a complete row of constant values.
        // Use the value(index) method instead.
        assert(!m_constant);
        return m_values;
    }

    void table::column::resize(uint32_t size)
    {
        if(!m_constant)
            m_values.resize(size, boost::any());
    }

    boost::any table::column::value(uint32_t row) const
    {
        if (m_constant)
        {
            return m_values[0];
        }
        else if (m_values.size() > row)
        {
            return m_values[row];
        }
        else
        {
            // Out of bounds
            assert(0);
        }
    }

    void table::column::set_value(uint32_t row, const boost::any& value)
    {
        // you can't override the value of a const column.
        assert(!m_constant);

        if(!m_type_set)
        {
            m_type_hash = value.type().hash_code();
        }
        assert(m_type_hash == value.type().hash_code());

        // did you forget to add a row?
        assert(m_values.size() > row);
        assert(m_values[row].empty());

        m_values[row] = value;
    }

    void table::column::make_nonconst(uint32_t size)
    {
        m_constant = false;
        m_values.resize(size, m_values[0]);
    }

    table::table()
        : m_rows(0)
    { }

    void table::add_const_column(const std::string& column_name,
        const boost::any& value)
    {
        // Check that the column doesn't already exist.
        assert(m_columns.find(column_name) == m_columns.end());

        m_columns.insert(
            std::pair<std::string,column>(column_name,column(value)));
    }

    void table::add_const_column(const std::string& column_name,
        const char* value)
    {
        add_const_column(column_name, std::string(value));
    }

    void table::add_column(const std::string& column_name)
    {
        // Check that the column doesn't already exist.
        assert(m_columns.find(column_name) == m_columns.end());

        auto& c = m_columns[column_name];

        c.resize(m_rows);
    }

    bool table::is_column(const std::string& column_name,
                          const std::type_info& type) const
    {
        // Check that the column exists.
        assert(m_columns.find(column_name) != m_columns.end());

        const auto& c = m_columns.at(column_name);
        assert(c.type_hash());
        return (*c.type_hash()) == type.hash_code();

    }

    void table::add_row()
    {
        ++m_rows;

        for(auto& kv: m_columns)
        {
            kv.second.resize(m_rows);
        }
    }

    void table::set_value(const std::string& column_name,
                          const boost::any& value)
    {
        assert(!value.empty());

        if(m_columns.find(column_name) == m_columns.end())
        {
            add_column(column_name);
        }

        assert(m_columns.find(column_name) != m_columns.end());

        auto& c = m_columns[column_name];
        assert(m_rows > 0); // Did you forget to call add_row(..)

        // Access the "current" row if we are on row 1,
        // this is index 0
        c.set_value(m_rows - 1, value);

        assert((*c.type_hash()) == value.type().hash_code());
    }

    void table::set_value(const std::string& column_name, const char* value)
    {
        set_value(column_name, std::string(value));
    }

    bool table::has_column(const std::string& column_name) const
    {
        return m_columns.find(column_name) != m_columns.end();
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

    std::map<std::string, table::column> table::columns() const
    {
        return m_columns;
    }

    void table::merge(const table& src)
    {
        //if two tables are merged, the const of a const_column no longer applies.
        for (auto my_kv = m_columns.begin(); my_kv != m_columns.end(); ++my_kv)
        {
            my_kv->second.make_nonconst(m_rows);
        }
        std::cout << "non const" << std::endl;

        for(uint32_t i = 0; i < src.rows(); ++i)
        {
            add_row();
            std::cout << i << std::endl;
            for(const auto& kv : src)
            {
                auto name = kv.first;
                auto column = kv.second;
                auto this_column_pair = m_columns.find(name);
                
                if(this_column_pair == m_columns.end())
                {
                    std::cout << "making " << name << std::endl;
                    add_column(name);
                }
                set_value(name, column.value(i));
            }
        }
    }

    table::const_iterator table::begin() const
    {
        return m_columns.cbegin();
    }

    table::const_iterator table::end() const
    {
        return m_columns.cend();
    }
}
