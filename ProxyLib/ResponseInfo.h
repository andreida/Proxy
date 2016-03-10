#pragma once

#include "Common.h"

#include <memory>

namespace ProxyLib {

class ResponseInfo : boost::noncopyable
{
public:
    ResponseInfo();

    ByteArray& Buffer();

    void SetBytesSent(std::size_t size);

    void SetMessageBodyChunk(ByteArray::const_iterator begin
                            , ByteArray::const_iterator end);

    std::size_t BodyLength() const;

    std::size_t BytesLeftToSend() const;
};

typedef std::unique_ptr<ResponseInfo> ResponseInfoPtr;

}
