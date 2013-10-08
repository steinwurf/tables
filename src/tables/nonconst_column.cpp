#include "nonconst_column.hpp"

namespace tables {

    nonconst_column::nonconst_column(uint32_t rows)
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

    void nonconst_column::add_rows(uint32_t rows)
    {
        m_values.resize(m_values.size() + rows, boost::any());
    }

    void nonconst_column::add_row()
    {
        add_rows(1);
    }

    uint32_t nonconst_column::rows() const
    {
        return m_values.size();
    }

    void nonconst_column::set_value(boost::any value)
    {
        // You forgot to call add_row(s), didn't you?
        assert(m_values[m_values.size()-1].empty());

        if(!value.empty())
        {
            if (!m_type_hash)
            {
                m_type_hash = value.type().hash_code();
            }
            assert(value.type().hash_code() == m_type_hash);
        }

        m_values[m_values.size()-1] = value;
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
