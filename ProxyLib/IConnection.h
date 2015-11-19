#pragma once

#include "Common.h"

namespace ProxyLib {

struct IConnection : boost::noncopyable
{
    virtual void ConnectAsync(const std::string& host, std::uint16_t port) = 0;

    virtual void ReadAsync() = 0;

    virtual void WriteAsync(std::size_t size, ByteArray& buffer) = 0;

    virtual void Stop() = 0;

    virtual ~IConnection()
    {}
};

}
