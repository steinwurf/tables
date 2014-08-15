// Copyright (c) 2014 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <ostream>
#include <string>

#include "format.hpp"
#include "table.hpp"

namespace tables
{
    /// Prints to the ostream in csv format, as specified here:
    /// http://tools.ietf.org/html/rfc4180
    class csv_format : public format
    {
    public:

        /// Make the format::print functions available
        using format::print;

    public:
        // From format

        /// @copydoc format::print(std::ostream&, const table&)
        virtual void print(std::ostream& s, const table& val) const;
        virtual std::string vector_seperator() const;
    };
}
