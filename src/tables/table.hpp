#pragma once

#include <cassert>
#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <boost/any.hpp>
#include <boost/optional.hpp>

namespace tables
{
    /// The table class represents a set of measurements on for every
    /// run. The table consists of columns and rows, the columns represent
    /// an event we have measured, the rows the values.
    /// In many cases we know the columns in advance,
    /// e.g. when using the time benchmark we have a column called time
    /// which contains the elapsed time. However, in some cases we are
    /// running benchmarks where the "events" we measure are not known in
    /// advance. In these cases the table allows us to dynamically add
    /// columns as they are needed.
    /// The table will always preserve the runs <-> row mapping, this
    /// means that all rows will contain a result for any given run. If
    /// a row is added at a late stage the table will use the fill value
    /// to initialize the row for all previous runs. The same will happen
    /// if a row is not updated for a specific run.
    ///
    /// The table only is meant to store results with the same unit e.g.
    /// microseconds or ticks. If the unit of some event differ they should
    /// be stored in two different tables.
    class table
    {
    public:

        /// Represents a column in the table
        class column
        {
        public:
            column();
            column(const boost::any& const_value);
            bool constant() const;
            boost::optional<size_t> type_hash() const;
            std::vector<boost::any> values() const;
            boost::any value(uint32_t row) const;
            void set_value(uint32_t row, const boost::any& value);
            void resize(uint32_t size);
        private:
            /// Checks whether the value of the colúmn is constant.
            const bool m_constant;

            /// Boolean value determining whether the type has been set
            bool m_type_set;

            /// Stores the data type of the row can be obtained using
            /// typeid(T).hash_code()
            boost::optional<size_t> m_type_hash;

            /// Stores all the values for the table rows
            std::vector<boost::any> m_values;
        };

    public:

        /// Construct a new table
        table();

        /// Create a new column in the table
        /// @param column_name The name of the column
        void add_column(const std::string& column_name);

        /// Create a new constant column in the table
        /// @param column_name The name of the column
        /// @param value The constant value for all rows in this column.
        void add_const_column(const std::string& column_name, const boost::any& value);

        /// Called when new results are ready to be registered. This
        /// function essentially adds a new row to the table for all
        /// current columns.
        void add_row();

        /// Sets the value for the current row in the specified column
        /// @param column_name The name of the column
        /// @param value The value to set for the current row
        void set_value(const std::string& column_name, const boost::any& value);

        /// Sets the value for the current row in the specified column
        /// @param column_name The name of the column
        /// @param value The value to set for the current row
        void set_value(const std::string& column_name, const char* value);

        /// Merge the source table into this table. All rows in the source
        /// table are added as new rows in the table and columns are
        /// created on-the-fly as needed.
        /// @param src The source table to merge into this table
        void merge(const table& src);

        /// @return The number of rows
        uint32_t rows() const;

        /// @return The columns
        std::map<std::string, column> columns() const;

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
        typedef std::map<std::string, column>::const_iterator
            const_iterator;

        /// @return const iterator to the first row
        const_iterator begin() const;

        /// @return const iterator to the last row
        const_iterator end() const;

    private:

        /// Keeps track of the number of columns
        uint32_t m_rows;

        /// Keeps track of which rows have been updated, this
        /// it to prevent multiple writers overwriting each other
        /// by accident
        std::map<std::string, column> m_columns;
    };
}
