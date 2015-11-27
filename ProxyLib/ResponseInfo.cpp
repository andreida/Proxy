#include "pch.h"
#include "ResponseInfo.h"

namespace ProxyLib {

ResponseInfo::ResponseInfo()
{
}

ByteArray& ResponseInfo::Buffer()
{
    return ByteArray();
}

void ResponseInfo::SetBytesSent(std::size_t size)
{

}

void ResponseInfo::SetMessageBodyChunk(ByteArray::const_iterator begin
                                      , ByteArray::const_iterator end)
{

}

std::size_t ResponseInfo::BodyLength() const
{
    return 0;
}

std::size_t ResponseInfo::BytesLeftToSend() const
{
    return 0;
}

}
