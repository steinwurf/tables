#pragma once

#include <cassert>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include <boost/any.hpp>
#include <boost/shared_ptr.hpp>

#include "column.hpp"

namespace tables
{
    class table
    {
    public:

        /// Construct a new table
        table();

        /// Adds a column to the table.
        /// @param column_name The name of the new column
        void add_column(const std::string& column_name);

        /// Adds a column with a single constant value for each
        /// row to the table.
        /// @param column_name The name of the new column
        /// @param value The value to set for all rows
        void add_const_column(const std::string& column_name,
            const boost::any& value);

        /// Sets the value for the current row in the specified column, if the
        /// doesn't exist, it will be created.
        /// @param column_name The name of the column
        /// @param value The value to set for the current row
        void set_value(const std::string& column_name, const boost::any& value);

        /// Adds a new row to the table for all columns.
        void add_row();

        /// Merges this table with the source table. All rows in the source
        /// table are added as new rows in the table and columns are
        /// created on-the-fly if needed.
        /// @param src The source table to merge into this table
        void merge(const table& src);

        /// Drops a specific column.
        /// @param column_name The name of the column
        void drop_column(const std::string& column_name);

        /// Returns the number of rows.
        /// @return The number of rows
        uint32_t rows() const;

        /// Returns the names of the columns.
        /// @return The names of the columns
        std::vector<std::string> columns() const;

        /// Returns the value stored on the specified position.
        /// @param column_name The name of the column
        /// @param row_index The index of the row to access
        /// @return The value stored on the specified position
        boost::any value(const std::string& column_name,
            uint32_t row_index) const;

        /// Returns the default value of the column.
        /// @param column_name The name of the column
        /// @return The default value of the column
        boost::any default_value(const std::string& column_name) const;

        /// Returns the values stored in the specified column.
        /// @param column_name The name of the column
        /// @return The values stored in the specified column
        std::vector<boost::any> values(const std::string& column_name) const;

        /// Returns the data of specific column.
        /// @param column_name The name of the column
        /// @return A vector containing the results for a specific column as the
        /// provided data type T
        template<class T>
        std::vector<T> values_as(const std::string& column_name,
            const T& default_value) const
        {
            assert(has_column(column_name));
            assert(is_column<T>(column_name));

            std::vector<T> values;

            const auto& column = m_columns.at(column_name);

            for(const auto& value : column->values())
            {
                if(value.empty())
                {
                    values.push_back(default_value);
                }
                else
                {
                    T casted_value = boost::any_cast<T>(value);
                    values.push_back(casted_value);
                }
            }

            return values;
        }

        /// Returns true if the specified column is constant.
        /// @param column_name The name of the column
        /// @return True if the specified column is constant
        bool is_constant(const std::string& column_name) const;

        /// Returns the number of empty values in a specified column.
        /// @param column_name The name of the column
        /// @return Number of empty values in the specified column
        uint32_t empty_rows(const std::string& column_name) const;

        /// Checks whether the column contains a specific data type.
        /// @param column_name The name of the column
        /// @return True if the column has the type T
        template<class T>
        bool is_column(const std::string& column_name) const
        {
            return is_column(column_name, typeid(T));
        }

        /// Checks whether the column contains a specific data type.
        /// @param column_name The name of the column
        /// @param type The data type of the column
        /// @return True if the column has the type
        bool is_column(const std::string& column_name,
                       const std::type_info& type) const;

        /// Returns true if the column exists.
        /// @param column_name The name of the column
        /// @return True if the column exists
        bool has_column(const std::string& column_name) const;

    public: // Iterator access to the results

        /// The pointer to a column type
        typedef boost::shared_ptr<column> column_ptr;

        /// The column map type
        typedef std::map<std::string, column_ptr> column_map;

        /// The column iterator type
        typedef column_map::const_iterator column_iterator;

        /// The column name iterator type
        class column_name_iterator : public column_iterator
        {
        public:

            column_name_iterator() : column_iterator()
            { };

            column_name_iterator(const column_iterator s) : column_iterator(s)
            { };

            std::string* operator->()
            {
                return (std::string* const)&(
                    column_iterator::operator->()->first);
            }

            std::string operator*()
            {
                return column_iterator::operator*().first;
            }
        };

        /// @return const iterator to the first column name
        column_name_iterator begin() const;

        /// @return const iterator to the last column name
        column_name_iterator end() const;

    private:

        /// Keeps track of the number of rows
        uint32_t m_rows;

        /// Stores the columns and their names
        column_map m_columns;
    };
}
