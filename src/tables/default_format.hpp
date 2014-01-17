#pragma once

#include <ostream>

#include "format.hpp"
#include "table.hpp"

namespace tables
{
    class default_format : public format
    {
    public:

        /// Make the format::print functions available
        using format::print;

    public: // From format

        /// @copydoc format::print(std::ostream&, const table&)
        void print(std::ostream& s, const table& val) const
        {
            // Because we want to be able to instantiate this format class,
            // this method cannot be made purely virtual.
            (void) s;
            (void) val;
            assert(0);
        }
    };
}
