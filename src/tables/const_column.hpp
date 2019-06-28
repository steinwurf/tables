// Copyright (c) 2014 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <vector>

#include "column.hpp"

namespace tables
{
/// Class for columns containing the same data for every row.
class const_column : public column
{
public:
    /// Creates a constant column
    /// @param value The value stored by this column
    /// @param rows The initial number of rows
    const_column(const boost::any& value, uint32_t rows = 0);

public:
    /// @copydoc column::value(uint32_t) const
    boost::any value(uint32_t row_index) const;

    /// @copydoc column::default_value() const
    boost::any default_value() const;

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

    /// @copydoc column::reserve()
    void reserve(uint32_t rows);

    /// @copydoc column::set_value(const boost::any&)
    void set_value(const boost::any& value);

    /// @copydoc column::set_default_value(const boost::any&)
    void set_default_value(const boost::any& value);

    /// @copydoc column::is_constant() const
    bool is_constant() const;

    /// @copydoc column::type_hash() const
    boost::optional<size_t> type_hash() const;

private:
    /// Stores the single value of the column
    const boost::any m_value;

    /// Keeps track of the column's size
    uint32_t m_rows;
};
}
