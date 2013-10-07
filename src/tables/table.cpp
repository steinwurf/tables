
#include <iomanip>



#include "const_column.hpp"
#include "nonconst_column.hpp"

#include "table.hpp"

namespace tables
{
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

        if(m_rows > 0)
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
            if (!kv.second.constant())
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
        // if two tables are merged, the const of a const_column no longer applies.
        for (auto& my_kv : m_columns)
        {
            if (my_kv.second.constant())
                my_kv.second.make_nonconst(m_rows);
        }

        for(uint32_t i = 0; i < src.rows(); ++i)
        {
            add_row();
            for(const auto& kv : src)
            {
                auto name = kv.first;
                auto column = kv.second;

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
