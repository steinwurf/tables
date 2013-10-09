#pragma once

#include <boost/shared_ptr.hpp>
#include "column.hpp"

namespace tables
{

    /// Class for columns containing data for every row.
    class nonconst_column : public column
    {
    public:

        /// Creates a non-constant column
        /// @param rows The initial number of rows
        nonconst_column(uint32_t rows = 0);

        /// Creates a non-constant column from a column (most likely a
        /// const_column)
        /// @param column The source column to initialize data from.
        nonconst_column(const boost::shared_ptr<column> column);

    public:

        /// @copydoc column::value(uint32_t) const
        boost::any value(uint32_t row_index) const;

        /// @copydoc column::values() const
        std::vector<boost::any> values() const;

        /// @copydoc column::add_rows(uint32_t)
        void add_rows(uint32_t rows);

        /// @copydoc column::add_row()
        void add_row();

        /// @copydoc column::rows() const
        uint32_t rows() const;

        /// @copydoc column::empty_rows() const
        uint32_t empty_rows() const;

        /// @copydoc column::set_value(const boost::any&)
        void set_value(const boost::any& value);

        /// @copydoc column::is_constant() const
        boost::optional<size_t> type_hash() const;

        /// @copydoc column::type_hash() const
        bool is_constant() const;

    private:

        /// Stores the columns data for every row.
        std::vector<boost::any> m_values;

        /// Stores a hash of the columns data type
        boost::optional<size_t> m_type_hash;
    };
}
