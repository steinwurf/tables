#include <cstdint>
#include <string>

#include "csv_format.hpp"

namespace tables
{
    void csv_format::print(std::ostream &s, const table &val) const
    {
        // Print headers first, then rows.
        bool print_headers = true;
        for(uint32_t i = 0; i < val.rows()+1; ++i)
        {
            bool first = true;
            for(const auto& c: val.columns())
            {
                if(!first)
                    s << ",";

                if (print_headers)
                {
                    s << c.first;
                }
                else
                {
                    // i-1 to accomedate for the header print run.
                    print(s, c.second.m_values[i-1]);
                }

                first = false;
            }
            print_headers = false;
            s << std::endl;
        }
    }
}