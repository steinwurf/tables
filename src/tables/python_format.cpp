// Copyright (c) 2014 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.
#include <string>

#include "python_format.hpp"

namespace tables
{
    void python_format::print_empty(std::ostream& s) const
    {
        s << "None";
    }
    void python_format::print(std::ostream& s, bool val) const
    {
        if (val)
        {
            s << "True";
        }
        else
        {
            s << "False";
        }
    }

    void python_format::print(std::ostream& s,
                              const std::string& val) const
    {
        s << "'" << val << "'";
    }
}
