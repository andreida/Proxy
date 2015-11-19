#include "pch.h"
#include "ResponseInfo.h"

namespace ProxyLib {

ResponseInfo::ResponseInfo()
{
}

ByteArray ResponseInfo::Buffer()
{
    return ByteArray();
}

bool ResponseInfo::IsComplete() const
{
    return false;
}

void ResponseInfo::SetBytesTransfered(std::size_t size)
{

}

void ResponseInfo::ProcessChunk(const char* buffer, std::size_t size)
{

}

}
