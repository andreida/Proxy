#pragma once

#include "Common.h"
#include "ResponseInfo.h"

namespace ProxyLib {

class ResponseParser
{
public:
    
    typedef ResponseInfoPtr ResultT;

    static ResponseInfoPtr Parse(ByteArray::iterator& begin
                                , ByteArray::iterator end);
};

}
