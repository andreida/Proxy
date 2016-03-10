#pragma once

#include <vector>
#include <initializer_list>
#include <algorithm>

namespace ProxyLib {

template <class T, class Alloc>
void Append(T val, std::vector<T, Alloc>& v)
{
    v.insert(v.end(), val);
}

template <class Iter, class Alloc>
void Append(Iter begin, Iter end, std::vector<typename Iter::value_type, Alloc>& v)
{
    v.insert(v.end(), begin, end);
}

template <class T, class Alloc>
void Append(std::initializer_list<T> ilist, std::vector<T, Alloc>& v)
{
    v.insert(v.end(), ilist);
}

template <class StringT> 
struct LexicographicalCompare
{
    bool operator()(const StringT& l, const StringT& r) const
    {
        return std::lexicographical_compare(l.begin(), l.end(), r.begin(), r.end()
            , [this](StringT::value_type ch1, StringT::value_type ch2)
              {
                  return std::tolower(ch1, locale_) < std::tolower(ch2, locale_);
              });
    }

private:

    std::locale locale_;
};

}
