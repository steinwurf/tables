#include "const_column.hpp"

namespace tables
{

    const_column::const_column(const boost::any& value, uint64_t rows)
        : m_value(value), m_rows(rows)
    {
        assert(!value.empty());
    }

    boost::any const_column::value(uint64_t row_index) const
    {
        // did you forget to add rows?
        assert(m_rows > 0);
        // you are out of bounds here.
        assert(row_index < m_rows);
        return m_value;
    }

    std::vector<boost::any> const_column::values() const
    {
        // did you forget to add rows?
        assert(m_rows > 0);
        return std::vector<boost::any>(m_rows, m_value);
    }

    boost::any const_column::default_value() const
    {
        return boost::any();
    }

    void const_column::add_rows(uint64_t rows)
    {
        m_rows += rows;
    }

    void const_column::add_row()
    {
        m_rows += 1;
    }

    uint64_t const_column::rows() const
    {
        return m_rows;
    }

    uint64_t const_column::empty_rows() const
    {
        return 0;
    }

    void const_column::set_value(const boost::any& value)
    {
        (void)value;
        // the value of a const_column cannot be set.
        assert(0);
    }

    void const_column::set_default_value(const boost::any& value)
    {
        (void)value;
        // a const_column cannot have a default value.
        assert(0);
    }


    boost::optional<uint64_t> const_column::type_hash() const
    {
        return m_value.type().hash_code();
    }

    bool const_column::is_constant() const
    {
        return true;
    }
}
