#pragma once

#include "column.hpp"

namespace tables {

    class nonconst_column : public column
    {
    public:

        nonconst_column(const column& column);

    public:
        boost::any value(uint32_t row_index) const;
        std::vector<boost::any> values() const;
        void add_rows(uint32_t rows);
        void add_row();
        uint32_t rows() const;
        void set_value(boost::any value);
        boost::optional<size_t> type_hash() const;
        bool is_constant() const;

    private:

        std::vector<boost::any> m_values;
        boost::optional<size_t> m_type_hash;
    };
}