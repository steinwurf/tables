#pragma once

#include "column.hpp"

namespace tables {

    class const_column : public column
    {
    public:

        const_column(const boost::any& value, uint32_t rows = 0);

    public:

        boost::any value(uint32_t row_index) const;
        std::vector<boost::any> values() const;
        void add_rows(uint32_t rows);
        void add_row();
        uint32_t rows() const;
        void set_value(boost::any value);
        bool is_constant() const;
        boost::optional<size_t> type_hash() const;

    private:

        const boost::any m_value;
        uint32_t m_rows;
    };
}