#include "pch.h"
#include "RequestInfo.h"

namespace ProxyLib {

RequestInfo::RequestInfo(std::string host
                        , std::uint16_t port
                        , ConnectionOptions connectionOptions
                        , std::uint32_t bodyLength
                        , ByteArray requestBuffer)
    : host_(std::move(host))
    , port_(port)
    , connectionOptions_(connectionOptions)
    , bodyLength_(bodyLength)
    , requestBuffer_(std::move(requestBuffer))
{}

const std::string& RequestInfo::Host() const
{
    return host_;
}

std::uint16_t RequestInfo::Port() const
{
    return port_;
}

ByteArray& RequestInfo::Buffer()
{
    return requestBuffer_;
}

void RequestInfo::SetBytesSent(std::size_t size)
{

}

void RequestInfo::SetMessageBodyChunk(ByteArray::const_iterator begin
                                     , ByteArray::const_iterator end)
{

}

std::uint32_t RequestInfo::BodyLength() const
{
    return 0;
}

std::size_t RequestInfo::BytesLeftToSend() const
{
    return 0;
}

}
