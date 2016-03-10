#pragma once

#include <cstdint>

namespace ProxyLib {

class ServerImpl;

class Server
{
public:
    Server(std::uint16_t port);

    Server(const Server&) = delete;

    ~Server();

    void Run();

private:

    ServerImpl* impl_;
};

}
