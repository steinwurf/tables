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
        for(uint32_t i = 0; i < val.rows(); ++i)
        {
            bool first = true;
            for(const auto& c: val.columns())
            {
                if(!first)
                    s << ",";

                print(s, c.first);
                s << ":";
                print(s, c.second.m_values);

                first = false;
            }
            s << std::endl;
        }
        s << "}";
    }
}
