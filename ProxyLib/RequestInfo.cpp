#include "pch.h"
#include "RequestInfo.h"

namespace ProxyLib {

RequestInfo::RequestInfo(std::string host
                        , std::uint16_t port
                        , ConnectionOptions connectionOptions
                        , std::uint32_t messageLength
                        , ByteArray requestBuffer)
    : host_(std::move(host))
    , port_(port)
    , connectionOptions_(connectionOptions)
    , messageLength_(messageLength)
    , requestBuffer_(std::move(requestBuffer))
    , responseInfo_()
{}

const std::string& RequestInfo::Host() const
{
    return host_;
}

std::uint16_t RequestInfo::Port() const
{
    return port_;
}

ByteArray RequestInfo::Buffer()
{
    return std::move(requestBuffer_);
}

bool RequestInfo::IsComplete() const
{
    return false;
}

void RequestInfo::SetBytesTransfered(std::size_t size)
{

}

void RequestInfo::ProcessChunk(const char* buffer, std::size_t size)
{

}

void RequestInfo::SetResponseInfo(ResponseInfoPtr responseInfo)
{
    responseInfo_ = std::move(responseInfo);
}

ResponseInfo& RequestInfo::Response()
{
    return *responseInfo_;
}

}
