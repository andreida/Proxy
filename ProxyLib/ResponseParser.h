#pragma once

#include "Common.h"
#include "ResponseInfo.h"

namespace ProxyLib {

class ResponseParser
{
public:
    
    typedef ResponseInfo MessageT;
    typedef ResponseInfoPtr ResultT;

    ResponseInfoPtr Parse(std::size_t size, ByteArray& buffer);
};

}
