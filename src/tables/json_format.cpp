#include "json_format.hpp"

namespace tables
{

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

    std::string json_format::vector_begin() const
    {
        return "[";
    }

    std::string json_format::vector_end() const
    {
        return "]";
    }

    void json_format::print(std::ostream &s, const table &val) const
    {
        s << "{";
        bool first = true;
        for(const auto& c: val.columns())
        {
            if(!first)
                s << ",";

            print(s, c.first);
            s << ":";
            if (c.second.has_fill_value())
            {
                print(s, c.second.m_values[0]);
            }
            else
            {
                print(s, c.second.m_values);
            }
            first = false;
        }
        s << "}";
    }
}
