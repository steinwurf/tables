#pragma once

#include <cassert>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include <boost/any.hpp>

#include "column.hpp"

namespace tables
{
    class table
    {
    public:

        /// Construct a new table
        table();

        /// Sets the value for the current row in the specified column, if the
        /// doesn't exist, it will be created.
        /// @param column_name The name of the column
        /// @param value The value to set for the current row
        void set_value(const std::string& column_name, const boost::any& value);

        /// Sets a constant value for a new column
        /// @param column_name The name of the new column
        /// @param value The value to set for the current row
        void set_const_value(const std::string& column_name, const boost::any& value);


        /// Called when new results are ready to be registered. This
        /// function essentially adds a new row to the table for all
        /// current columns.
        void add_row();

        /// Merge the source table into this table. All rows in the source
        /// table are added as new rows in the table and columns are
        /// created on-the-fly as needed.
        /// @param src The source table to merge into this table
        void merge(const table& src);

        /// @return The number of rows
        uint32_t rows() const;

        /// @return The column names
        std::vector<std::string> columns() const;

        /// Checks whether the column has a specific data type
        /// @param column_name The name of the column
        /// @return True if the column has the type T
        template<class T>
        bool is_column(const std::string &column_name) const
        {
            return is_column(column_name, typeid(T));
        }

        /// Checks whether the column has a specific data type
        /// @param column_name The name of the column
        /// @param type The data type of the column
        /// @return True if the column has the type
        bool is_column(const std::string& column_name,
                       const std::type_info& type) const;

        /// Returns true if the specific column exists
        /// @param column_name The name of the column
        /// @return True if the column exists
        bool has_column(const std::string& column_name) const;

        /// Drops a specific column
        /// @param column_name The name of the column
        void drop_column(const std::string& column_name);

        /// Returns a specific column.
        /// @param column_name The name of the column
        /// @return The vector containing the results for a specific column
        template<class T>
        std::vector<T> column_as(const std::string &column_name) const
        {
            assert(m_columns.find(column_name) != m_columns.end());

            assert(is_column<T>(column_name));

            std::vector<T> v;

            const auto& c = m_columns.at(column_name);

            for(const auto& i : c.m_values)
            {
                assert(!i.empty());
                T t = boost::any_cast<T>(i);
                v.push_back(t);
            }

            return v;

        }

    public: // Iterator access to the results

        /// The iterator type
        typedef std::map<std::string>::const_iterator
            const_iterator;

        /// @return const iterator to the first column name
        const_iterator begin() const;

        /// @return const iterator to the last column name
        const_iterator end() const;

    private:

        /// Keeps track of the number of rows
        uint32_t m_rows;

        /// Stores the columns and their names
        std::map<std::string, column> m_columns;
    };
}
