// Copyright (c) 2014 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

// This code has been copied from
// http://codereview.stackexchange.com/questions/13176/infix-iterator-code
// Created by Jerry Coffin

#include <ostream>
#include <iterator>
#include <string>

namespace tables
{
template <
    class T,
    class charT = char,
    class traits = std::char_traits<charT>>
class infix_ostream_iterator :
    public std::iterator<std::output_iterator_tag, void, void, void, void>
{
    std::basic_ostream<charT, traits>* os;
    std::basic_string<charT> delimiter;
    std::basic_string<charT> real_delim;

public:

    typedef charT char_type;
    typedef traits traits_type;
    typedef std::basic_ostream<charT, traits> ostream_type;

    explicit infix_ostream_iterator(ostream_type& s) :
        os(&s)
    { }

    infix_ostream_iterator(ostream_type& s, charT const* d) :
        os(&s),
        real_delim(d)
    { }

    infix_ostream_iterator<T, charT, traits>& operator=(T const& item)
    {
        *os << delimiter;
        *os << item;
        delimiter = real_delim;
        return *this;
    }

    infix_ostream_iterator<T, charT, traits>& operator*()
    {
        return *this;
    }

    infix_ostream_iterator<T, charT, traits>& operator++()
    {
        return *this;
    }

    infix_ostream_iterator<T, charT, traits>& operator++(int value)
    {
        (void)value;
        return *this;
    }
};
}
