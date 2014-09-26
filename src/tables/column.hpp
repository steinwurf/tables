// Copyright (c) 2014 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <boost/any.hpp>
#include <boost/optional.hpp>

#include <cstdint>
#include <cstdlib>
#include <vector>


namespace tables
{
    /// An abstract class containing only purely virtual methods. This is used
    /// as a base class for the different columns.
    class column
    {
    public:

        /// Returns the value located at the specified location
        /// @param row_index The index of the row on which the data should be
        /// fetched
        /// @returns A boost any object containing the data
        virtual boost::any value(uint32_t row_index) const = 0;

        /// Returns a vector all the values in the column
        /// @return A vector containing all the values
        virtual std::vector<boost::any> values() const = 0;

        /// Returns the column's default value
        /// @return The column's default value
        virtual boost::any default_value() const = 0;

        /// Adds the specfied number of rows
        /// @param rows The number of rows to add
        virtual void add_rows(uint32_t rows) = 0;

        /// Adds a row to the column
        virtual void add_row() = 0;

        /// Returns the number of rows
        /// @return The number of rows
        virtual uint32_t rows() const = 0;

        /// Returns the number of empty rows
        /// @return The number of empty rows
        virtual uint32_t empty_rows() const = 0;

        /// Sets a value in the column, at the current row
        /// @param value The value to set
        virtual void set_value(const boost::any& value) = 0;

        /// Sets the default value for the column. This is used instead
        /// when values in the column is empty.
        /// @param value The value to set
        virtual void set_default_value(const boost::any& value) = 0;

        /// Returns true if the column is constant
        /// @return True if the column is constant
        virtual bool is_constant() const = 0;

        /// Returns the type hash of the columns data
        /// @return The type hash of the columns data
        virtual boost::optional<size_t> type_hash() const = 0;

        /// A virtual destructor on a base class with virtual methods is needed
        virtual ~column() {}
    };
}
