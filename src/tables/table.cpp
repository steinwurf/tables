#include "table.hpp"

#include <cassert>
#include <iomanip>

namespace tables
{

    bool table::column()
        : m_constant(false)
    { }

    bool table::column(const boost::any& const_value)
        : m_constant(true)
    {
        m_type_hash = const_value.type().hash_code();
        m_values.resize(1, const_value);
    }

    bool table::column::constant() const
    {
        return m_constant;
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
            auto it = m_values.end();
        }
        m_values.insert ( it , value );
    }

    table::table()
        : m_rows(0)
    { }

    void table::add_const_column(const std::string &column,
        const boost::any& value)
    {
        // Check that the column doesn't already exist.
        assert(m_columns.find(column) == m_columns.end());

        m_columns.insert(
            std::pair<std::string,boost::any>(column,column(value)));
    }

    void table::add_column(const std::string &column)
    {
        // Check that the column doesn't already exist.
        assert(m_columns.find(column) == m_columns.end());

        auto& c = m_columns[column];

        c.m_values.resize(m_rows, boost::any());
    }

    void table::set_column_type(const std::string& column,
                                const std::type_info& type_info)
    {
        // Check that the column exists.
        assert(m_columns.find(column) != m_columns.end());

        auto& c = m_columns[column];

        assert(!c.m_type_hash);
        c.m_type_hash = type_info.hash_code();
    }

    bool table::is_column(const std::string& column,
                          const std::type_info& type) const
    {
        // Check that the column exists.
        assert(m_columns.find(column) != m_columns.end());

        const auto& c = m_columns.at(column);
        assert(c.m_type_hash);
        return (*c.m_type_hash) == type.hash_code();

    }

    void table::add_row()
    {
        ++m_rows;

        for(auto& c: m_columns)
        {
            c.second.m_values.resize(m_rows, boost::any());
        }
    }

    void table::set_value(const std::string& column,
                          const boost::any& value)
    {
        assert(!value.empty());

        if(m_columns.find(column) == m_columns.end())
        {
            add_column(column);
        }

        assert(m_columns.find(column) != m_columns.end());

        auto& c = m_columns[column];

        if(!c.m_type_hash)
        {
            set_column_type(column, value.type());
        }

        assert(c.m_updated == false);
        assert(c.m_type_hash);
        assert((*c.m_type_hash) == value.type().hash_code());

        c.m_updated = true;

        assert(m_rows > 0); // Did you forget to call add_row(..)
        assert(c.m_values.size() == m_rows);

        // Access the "current" row if we are on row 1,
        // this is index 0
        c.m_values[m_rows - 1] = value;
    }

    bool table::has_column(const std::string& column) const
    {
        return m_columns.find(column) != m_columns.end();
    }

    void table::drop_column(const std::string& column)
    {
        assert(has_column(column));
        m_columns.erase(column);
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
                set_value(t.first, t.second.m_values[i]);
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
