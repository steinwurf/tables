#pragma once

#include <ostream>

#include "format.hpp"
#include "json_format.hpp"

namespace tables
{

    /// Prints to the ostrea in Python format
    struct python_format : public json_format
    {
    public:

        /// Make the format::print functions available
        using format::print;

    public: // From format
        /// @copydoc format::print(std::ostream&,bool) const
        void print(std::ostream &s, bool val) const;

        /// @copydoc format::print(std::ostream&, const std::string&) const
        void print(std::ostream &s, const std::string &val) const;
    };

}
