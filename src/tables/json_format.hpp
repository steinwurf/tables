#pragma once

#include <ostream>
#include <string>

#include "table.hpp"
#include "format.hpp"

namespace tables
{
    /// Prints to the ostrea in Python format
    struct json_format : public format
    {
    public:

        /// Make the format::print functions available
        using format::print;

    public: // From format

        /// @copydoc format::print(std::ostream&,bool) const
        virtual void print(std::ostream &s, bool val) const;

        /// @copydoc format::print(std::ostream&, const std::string&) const
        virtual void print(std::ostream &s, const std::string &val) const;

        /// @copydoc format::vector_begin() const
        virtual std::string vector_begin() const;

        /// @copydoc format::vector_end() const
        virtual std::string vector_end() const;

        /// @copydoc format::void print(std::ostream&, const table&) const
        virtual void print(std::ostream &s, const table &val) const;

    };

}
