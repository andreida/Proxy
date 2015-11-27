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

    static RequestInfoPtr Parse(ByteArray::iterator& begin
                               , ByteArray::iterator end);

private:

    static RequestInfoPtr ParseRequest(ByteArray::iterator& begin
                                      , ByteArray::iterator end);

    static ByteArray CreateRequestBuffer(HttpMethods method
                                        , const std::string& host
                                        , const boost::optional<std::string>& resource
                                        , const boost::optional<StringList>& headers);

    static void AppendHeaders(const StringList& clientHeaders
                             , ByteArray& requestBuffer);
};

}
