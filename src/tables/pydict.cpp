#include "pydict.hpp"

namespace tables
{

    pydict::pydict()
        : m_first(true)
    {}

    void pydict::clear()
    {
        m_first = true;
        m_out.str("");
    }

    std::string pydict::str() const
    {
        return "{" + m_out.str() + "}";
    }
}
