#pragma once

#include "Common.h"

namespace ProxyLib {

class ResponseInfo : boost::noncopyable
{
public:
    ResponseInfo();

    ByteArray Buffer();

    bool IsComplete() const;

    void SetBytesTransfered(std::size_t size);

    void ProcessChunk(const char* buffer, std::size_t size);

};

typedef std::unique_ptr<ResponseInfo> ResponseInfoPtr;

}
