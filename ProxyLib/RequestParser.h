#pragma once

#include "Common.h"
#include "RequestInfo.h"

#include <boost/optional/optional_fwd.hpp>

namespace ProxyLib {

enum HttpMethods
{
    Http_Get,
    Http_Post
};

class RequestParser
{
public:
    typedef RequestInfo MessageT;
    typedef RequestInfoPtr ResultT;

    RequestInfoPtr Parse(std::size_t size, ByteArray& buffer);

private:

    RequestInfoPtr ParseRequest();

    void FillRequestBuffer(HttpMethods method
                          , const std::string& host
                          , const boost::optional<std::uint16_t>& port
                          , const boost::optional<std::string>& resource
                          , const boost::optional<StringList>& headers
                          , ByteArray::iterator messageBodyPos);

    ByteArray tempBuffer_;
};

}
