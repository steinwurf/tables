#pragma once

#include <cstdint>
#include <cstdlib>
#include <vector>
#include <boost/any.hpp>
#include <boost/optional.hpp>

namespace tables {

    class column
    {
    public:
        virtual boost::any value(uint32_t row_index) const = 0;
        virtual std::vector<boost::any> values() const = 0;
        virtual void add_rows(uint32_t rows) = 0;
        virtual void add_row() = 0;
        virtual uint32_t rows() const = 0;
        virtual void set_value(boost::any value) = 0;
        virtual bool is_constant() const = 0;
        virtual boost::optional<size_t> type_hash() const = 0;

        virtual ~column() {}
    };
}