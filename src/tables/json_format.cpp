// Copyright (c) 2014 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <string>

#include "json_format.hpp"

namespace tables
{

    void json_format::print_empty(std::ostream& s) const
    {
        s << "null";
    }

    void json_format::print(std::ostream& s, bool val) const
    {
        if (val)
        {
            s << "true";
        }
        else
        {
            s << "false";
        }
    }

    void json_format::print(std::ostream& s, const std::string& val) const
    {
        s << "\"" << val << "\"";
    }

    std::string json_format::vector_begin() const
    {
        return "[";
    }

    std::string json_format::vector_end() const
    {
        return "]";
    }

    void json_format::print(std::ostream& s, const table& val) const
    {
        s << "{";
        bool first = true;
        for (const auto& c: val.columns())
        {
            if (!first)
                s << ",";

            print(s, c);
            s << ":";
            if (val.is_constant(c))
            {
                print(s, val.value(c, 0));
            }
            else
            {
                print(s, val.values(c));
            }
            first = false;
        }
        s << "}";
    }
}
