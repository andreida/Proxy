#pragma once

#include "Common.h"

namespace ProxyLib {

class RequestInfo : boost::noncopyable
{
public:
    RequestInfo(std::string host
               , std::uint16_t port
               , ConnectionOptions connectionOptions
               , std::uint32_t bodyLength
               , ByteArray requestBuffer);

    const std::string& Host() const;

    std::uint16_t Port() const;

    ByteArray& Buffer();

    void SetBytesSent(std::size_t size);

    void SetMessageBodyChunk(ByteArray::const_iterator begin
                            , ByteArray::const_iterator end);

    std::size_t BodyLength() const;

    std::size_t BytesLeftToSend() const;

private:

    const std::string       host_;
    const std::uint16_t     port_;
    const ConnectionOptions connectionOptions_;
    const std::uint32_t     bodyLength_;
    ByteArray               requestBuffer_;
};

typedef std::unique_ptr<RequestInfo> RequestInfoPtr;

}
