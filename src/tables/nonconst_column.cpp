// Copyright (c) 2014 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <algorithm>
#include <vector>

#include "nonconst_column.hpp"

namespace tables
{
    nonconst_column::nonconst_column(uint32_t rows)
        : m_default_value(boost::any())
    {
        add_rows(rows);
    }

    nonconst_column::nonconst_column(const boost::shared_ptr<column> column)
    {
        for (const auto& v : column->values())
        {
            add_row();
            set_value(v);
        }
        m_default_value = column->default_value();
    }

    boost::any nonconst_column::value(uint32_t row_index) const
    {
        assert(row_index < m_values.size());
        return m_values[row_index];
    }

    std::vector<boost::any> nonconst_column::values() const
    {
        return m_values;
    }

    boost::any nonconst_column::default_value() const
    {
        return m_default_value;
    }

    void nonconst_column::add_rows(uint32_t rows)
    {
        // Check for overflow
        assert(m_values.size() + rows >= rows);

        // Check we can increase the size of the vector
        assert(m_values.size() + rows < m_values.max_size());

        m_values.resize(m_values.size() + rows, m_default_value);
    }

    void nonconst_column::add_row()
    {
        add_rows(1);
    }

    uint32_t nonconst_column::rows() const
    {
        return m_values.size();
    }

    // Helper function for the empty_rows;
    bool value_is_empty(const boost::any& value) { return value.empty(); }

    uint32_t nonconst_column::empty_rows() const
    {
        return std::count_if(m_values.begin(), m_values.end(), value_is_empty);
    }

    void nonconst_column::set_value(const boost::any& value)
    {
        // You forgot to call add_row(s), didn't you?
        assert(m_values.size() > 0);
        // If the default is not set we can check if the value was previously
        // set, otherwise we cannot.
        assert(!m_default_value.empty() || m_values[m_values.size()-1].empty());

        if (!value.empty())
        {
            if (!m_type_hash)
            {
                m_type_hash = value.type().hash_code();
            }
            assert(value.type().hash_code() == m_type_hash);
        }

        m_values[m_values.size()-1] = value;
    }

    void nonconst_column::set_default_value(const boost::any& value)
    {
        // The default default value is an empty boost::any anyway.
        assert(!value.empty());
        // You can't set the default value twice.
        assert(m_default_value.empty());

        if (!m_type_hash)
        {
            m_type_hash = value.type().hash_code();
        }
        assert(value.type().hash_code() == m_type_hash);

        m_default_value = value;
    }

    boost::optional<size_t> nonconst_column::type_hash() const
    {
        return m_type_hash;
    }

    bool nonconst_column::is_constant() const
    {
        return false;
    }
}
