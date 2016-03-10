#pragma once

#include "Common.h"

#include <boost/noncopyable.hpp>

#include <cstdint>

namespace ProxyLib {

struct IConnection : boost::noncopyable
{
    virtual void ConnectAsync(const std::string& host, std::uint16_t port) = 0;

    virtual void ReadAsync() = 0;

    virtual void WriteAsync(const ByteArray& buffer) = 0;

    virtual void Stop() = 0;

    virtual ~IConnection()
    {}
};

}
