// Copyright (c) 2014 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>
#include <string>
#include <typeinfo>

#include <boost/any.hpp>

#include "table.hpp"


namespace tables
{
/// Simple helper format struct, used when printing values.
/// The format for specific data types may be overwritten to
/// produce a specific output.
struct format
{
public:

    /// Prints the value to the ostream
    /// @param s The output stream
    /// @param val The value to be printed
    virtual void print(std::ostream& s, bool val) const
    {
        s << val;
    }

    /// Prints the value to the ostream
    /// @param s The output stream
    /// @param val The value to be printed
    virtual void print(std::ostream& s, int8_t val) const
    {
        s << (int32_t) val;
    }

    /// Prints the value to the ostream
    /// @param s The output stream
    /// @param val The value to be printed
    virtual void print(std::ostream& s, uint8_t val) const
    {
        s << (uint32_t) val;
    }

    /// Prints the value to the ostream
    /// @param s The output stream
    /// @param val The value to be printed
    virtual void print(std::ostream& s, int16_t val) const
    {
        s << val;
    }

    /// Prints the value to the ostream
    /// @param s The output stream
    /// @param val The value to be printed
    virtual void print(std::ostream& s, uint16_t val) const
    {
        s << val;
    }

    /// Prints the value to the ostream
    /// @param s The output stream
    /// @param val The value to be printed
    virtual void print(std::ostream& s, int32_t val) const
    {
        s << val;
    }

    /// Prints the value to the ostream
    /// @param s The output stream
    /// @param val The value to be printed
    virtual void print(std::ostream& s, uint32_t val) const
    {
        s << val;
    }

    /// Prints the value to the ostream
    /// @param s The output stream
    /// @param val The value to be printed
    virtual void print(std::ostream& s, int64_t val) const
    {
        s << val;
    }

    /// Prints the value to the ostream
    /// @param s The output stream
    /// @param val The value to be printed
    virtual void print(std::ostream& s, uint64_t val) const
    {
        s << val;
    }

    /// Prints the value to the ostream
    /// @param s The output stream
    /// @param val The value to be printed
    virtual void print(std::ostream& s, float val) const
    {
        s << val;
    }

    /// Prints the value to the ostream
    /// @param s The output stream
    /// @param val The value to be printed
    virtual void print(std::ostream& s, double val) const
    {
        s << val;
    }

    /// Prints the value to the ostream
    /// @param s The output stream
    /// @param val The value to be printed
    virtual void print(std::ostream& s, const char* val) const
    {
        print(s, std::string(val));
    }

    /// Prints the value to the ostream
    /// @param s The output stream
    /// @param val The value to be printed
    virtual void print(std::ostream& s, const std::string& val) const
    {
        s << val;
    }

    /// By default, printing an empty value does nothing. This can however
    /// be changed in super classes
    /// @param s The output stream
    virtual void print_empty(std::ostream& s) const
    {
        (void) s;
        return;
    }

    /// Prints the value to the ostream
    /// @param s The output stream
    /// @param val The value to be printed
    virtual void print(std::ostream& s, const boost::any& val) const
    {
        if (val.empty())
        {
            return print_empty(s);
        }
        if (typeid(bool) == val.type())
        {
            return print(s, boost::any_cast<bool>(val));
        }
        if (typeid(int8_t) == val.type())
        {
            return print(s, boost::any_cast<int8_t>(val));
        }
        if (typeid(uint8_t) == val.type())
        {
            return print(s, boost::any_cast<uint8_t>(val));
        }
        if (typeid(int16_t) == val.type())
        {
            return print(s, boost::any_cast<int16_t>(val));
        }
        if (typeid(uint16_t) == val.type())
        {
            return print(s, boost::any_cast<uint16_t>(val));
        }
        if (typeid(int32_t) == val.type())
        {
            return print(s, boost::any_cast<int32_t>(val));
        }
        if (typeid(uint32_t) == val.type())
        {
            return print(s, boost::any_cast<uint32_t>(val));
        }
        if (typeid(int64_t) == val.type())
        {
            return print(s, boost::any_cast<int64_t>(val));
        }
        if (typeid(uint64_t) == val.type())
        {
            return print(s, boost::any_cast<uint64_t>(val));
        }
        if (typeid(float) == val.type())
        {
            return print(s, boost::any_cast<float>(val));
        }
        if (typeid(double) == val.type())
        {
            return print(s, boost::any_cast<double>(val));
        }
        if (typeid(std::string) == val.type())
        {
            return print(s, boost::any_cast<std::string>(val));
        }
        if (typeid(table) == val.type())
        {
            return print(s, boost::any_cast<table>(val));
        }
        // Vector container types
        if (typeid(std::vector<bool>) == val.type())
        {
            return print(s, boost::any_cast<std::vector<bool>>(val));
        }
        if (typeid(std::vector<int8_t>) == val.type())
        {
            return print(s, boost::any_cast<std::vector<int8_t>>(val));
        }
        if (typeid(std::vector<uint8_t>) == val.type())
        {
            return print(s, boost::any_cast<std::vector<uint8_t>>(val));
        }
        if (typeid(std::vector<int16_t>) == val.type())
        {
            return print(s, boost::any_cast<std::vector<int16_t>>(val));
        }
        if (typeid(std::vector<uint16_t>) == val.type())
        {
            return print(s, boost::any_cast<std::vector<uint16_t>>(val));
        }
        if (typeid(std::vector<int32_t>) == val.type())
        {
            return print(s, boost::any_cast<std::vector<int32_t>>(val));
        }
        if (typeid(std::vector<uint32_t>) == val.type())
        {
            return print(s, boost::any_cast<std::vector<uint32_t>>(val));
        }
        if (typeid(std::vector<int64_t>) == val.type())
        {
            return print(s, boost::any_cast<std::vector<int64_t>>(val));
        }
        if (typeid(std::vector<uint64_t>) == val.type())
        {
            return print(s, boost::any_cast<std::vector<uint64_t>>(val));
        }
        if (typeid(std::vector<float>) == val.type())
        {
            return print(s, boost::any_cast<std::vector<float>>(val));
        }
        if (typeid(std::vector<double>) == val.type())
        {
            return print(s, boost::any_cast<std::vector<double>>(val));
        }
        if (typeid(std::vector<std::string>) == val.type())
        {
            return print(s, boost::any_cast<std::vector<std::string>>(val));
        }
        if (typeid(std::vector<table>) == val.type())
        {
            return print(s, boost::any_cast<std::vector<table>>(val));
        }
        if (typeid(std::vector<boost::any>) == val.type())
        {
            return print(s, boost::any_cast<std::vector<boost::any>>(val));
        }

        // We don't know how to convert this type
        std::cout << "Unknown type: " << val.type().name() << std::endl;
        assert(0);
    }

    virtual std::string vector_begin() const
    {
        return "";
    }

    virtual std::string vector_seperator() const
    {
        return ",";
    }

    virtual std::string vector_end() const
    {
        return "";
    }

    /// Prints the value to the ostream
    /// @param s The output stream
    /// @param val The value to be printed
    template<class T>
    void print(std::ostream& s, const std::vector<T>& val) const
    {
        s << vector_begin();
        uint32_t i = 0;
        for (const T& v : val)
        {
            print(s, v);
            i++;
            if (i == val.size())
                break;
            s << vector_seperator();
        }
        s << vector_end();
    }

    virtual void print(std::ostream& s, const table& val) const
    {
        // Because we want to be able to instantiate this format class,
        // this method cannot be made purely virtual.
        (void) s;
        (void) val;
        assert(0);
    }

    virtual ~format()
    { }
};
}
