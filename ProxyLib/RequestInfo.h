#pragma once

#include "Common.h"
#include "ResponseInfo.h"

namespace ProxyLib {

class RequestInfo : boost::noncopyable
{
public:
    RequestInfo(std::string host
               , std::uint16_t port
               , ConnectionOptions connectionOptions
               , std::uint32_t messageLength
               , ByteArray requestBuffer);

    const std::string& Host() const;

    std::uint16_t Port() const;

    ByteArray Buffer();

    bool IsComplete() const;

    void SetBytesTransfered(std::size_t size);

    void ProcessChunk(const char* buffer, std::size_t size);

    void SetResponseInfo(ResponseInfoPtr responseInfo);

    ResponseInfo& Response();

private:

    const std::string host_;
    const std::uint16_t port_;
    const ConnectionOptions connectionOptions_;
    const std::uint32_t messageLength_;
    ByteArray requestBuffer_;
    ResponseInfoPtr responseInfo_;
};

typedef std::unique_ptr<RequestInfo> RequestInfoPtr;

}
