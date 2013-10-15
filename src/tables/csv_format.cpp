#include <cstdint>
#include <string>
#include <iterator>

#include "infix_ostream_iterator.hpp"

#include "csv_format.hpp"

namespace tables
{
    void csv_format::print(std::ostream& s, const table& val) const
    {
        // Print headers
        infix_ostream_iterator<std::string> print_headers (s, ",");
        std::copy (val.begin(), val.end(), print_headers );
        s << std::endl;

        // Print rows
        for(uint32_t i = 0; i < val.rows(); ++i)
        {
            bool first = true;
            for(const auto& c_name: val)
            {
                if(!first)
                    s << ",";
                first = false;

                print(s, val.value(c_name, i));
            }
            s << std::endl;
        }
    }

    std::string csv_format::vector_seperator() const
    {
        return ";";
    }
}
