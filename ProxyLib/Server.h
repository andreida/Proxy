#pragma once

namespace ProxyLib {

class ServerImpl;

class Server
{
public:
    Server(unsigned short port);

    Server(const Server&) = delete;

    ~Server();

    void Run();

private:

    ServerImpl* impl_;
};

}
