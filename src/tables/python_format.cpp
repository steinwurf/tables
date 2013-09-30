#include "json_format.hpp"

#include "pylist.hpp"
#include "pydict.hpp"

namespace tables
{

    void python_format::print(std::ostream &s, bool val) const
    {
        if(val)
        {
            s << "True";
        }
        else
        {
            s << "False";
        }
    }

    void python_format::print(std::ostream &s, const std::string &val) const
    {
        s << "'" << val << "'";
    }

}
