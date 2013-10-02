#include "table.hpp"

#include <cassert>
#include <iomanip>

namespace tables
{

    table::column::column()
        : m_constant(false)
    { }

    table::column::column(const boost::any& const_value)
        : m_constant(true)
    {
        m_type_hash = const_value.type().hash_code();
        m_values.resize(1, const_value);
    }

    bool table::column::constant() const
    {
        return m_constant;
    }

    void table::column::set_type_hash(size_t value)
    {
        m_type_hash = value;
    }

    boost::optional<size_t> table::column::type_hash() const
    {
        return m_type_hash;
    }

    std::vector<boost::any> table::column::values() const
    {
        return m_values;
    }

    void table::column::resize(uint32_t size)
    {
        m_values.resize(size, boost::any());
    }

    boost::any table::column::value(uint32_t index) const
    {
        if (m_constant)
        {
            return m_values[0];
        }
        else if (m_values.size() < index)
        {
            return m_values[index];
        }
        else
        {
            return boost::any();
        }
    }

    void table::column::set_value(uint32_t index, const boost::any& value)
    {
        assert(!m_constant);
        if (m_values.size() < index)
        {
            m_values.resize(index, boost::any());
        }
        auto it = m_values.end();
        m_values.insert( it , value);
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

    void table::add_column(const std::string& column_name)
    {
        // Check that the column doesn't already exist.
        assert(m_columns.find(column_name) == m_columns.end());

        auto& c = m_columns[column_name];

        c.resize(m_rows);
    }

    void table::set_column_type(const std::string& column_name,
                                const std::type_info& type_info)
    {
        // Check that the column exists.
        assert(m_columns.find(column_name) != m_columns.end());

        auto& c = m_columns[column_name];

        assert(!c.type_hash());
        c.set_type_hash(type_info.hash_code());
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

        if(!c.type_hash())
        {
            set_column_type(column_name, value.type());
        }
        assert(c.type_hash());
        assert((*c.type_hash()) == value.type().hash_code());

        assert(m_rows > 0); // Did you forget to call add_row(..)

        // Access the "current" row if we are on row 1,
        // this is index 0
        c.set_value(m_rows - 1, value);
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
        for(uint32_t i = 0; i < src.rows(); ++i)
        {
            add_row();

            for(const auto& t : src)
            {
                set_value(t.first, t.second.value(i));
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
