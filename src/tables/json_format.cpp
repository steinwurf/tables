#include "json_format.hpp"

#include "pylist.hpp"
#include "pydict.hpp"

namespace tables
{

    void json_format::print(std::ostream &s, const pylist &val) const
    {
        s << val.str();
    }

    void json_format::print(std::ostream &s, const pydict &val) const
    {
        s << val.str();
    }

    void json_format::print(std::ostream &s, bool val) const
    {
        if(val)
        {
            s << "true";
        }
        else
        {
            s << "false";
        }
    }

    void json_format::print(std::ostream &s, const std::string &val) const
    {
        s << "\"" << val << "\"";
    }
}
