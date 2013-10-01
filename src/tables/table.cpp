#include "table.hpp"

#include <cassert>
#include <iomanip>

namespace tables
{

    bool table::column::has_fill_value() const
    {
        return !m_fill.empty();
    }

    table::table()
        : m_rows(0)
    { }

    void table::add_column(const std::string &column,
                           const boost::any& fill)
    {
        assert(m_columns.find(column) == m_columns.end());

        auto& c = m_columns[column];
        c.m_values.resize(m_rows, fill);
        c.m_updated = false;

        if(!fill.empty())
        {
            set_column_fill(column, fill);
        }
    }

    void table::set_column_type(const std::string& column,
                                const std::type_info& type_info)
    {
        assert(m_columns.find(column) != m_columns.end());

        auto& c = m_columns[column];

        assert(!c.m_type_hash);
        c.m_type_hash = type_info.hash_code();
    }

    void table::set_column_fill(const std::string& column,
                                const boost::any& value)
    {
        assert(!value.empty());

        if(m_columns.find(column) == m_columns.end())
        {
            add_column(column, value);
        }

        auto& c = m_columns[column];

        if(!c.m_type_hash)
        {
            c.m_type_hash = value.type().hash_code();
        }

        assert(c.m_type_hash == value.type().hash_code());

        c.m_fill = value;
    }

    bool table::is_column(const std::string& column,
                          const std::type_info& type) const
    {
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
            c.second.m_updated = false;
            c.second.m_values.resize(m_rows, c.second.m_fill);
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
