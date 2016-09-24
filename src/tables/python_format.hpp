// Copyright (c) 2014 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <ostream>
#include <string>

#include "format.hpp"
#include "json_format.hpp"

namespace tables
{
/// Prints to the ostream in Python format. As the Python format is based on
/// the json one, only slight difference exists. These comes from the fact
/// that the Python data types are different from those of Javascript
/// (which is what json is based on).
struct python_format : public json_format
{
public:

    /// Make the json_format::print functions available
    using json_format::print;

public:
    // From format
    /// @copydoc format::print_empty(std::ostream&) const
    virtual void print_empty(std::ostream& s) const;

    /// @copydoc format::print(std::ostream&,bool) const
    virtual void print(std::ostream& s, bool val) const;

    /// @copydoc format::print(std::ostream&, const std::string&) const
    virtual void print(std::ostream& s, const std::string& val) const;
};
}
