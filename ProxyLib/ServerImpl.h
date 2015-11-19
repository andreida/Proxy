#pragma once

#include "Common.h"

#include "Connection.h"
#include "ConnectionManager.h"

namespace ProxyLib {

typedef std::pair<ConnectionPtr, ConnectionPtr> ConnectionUnit;

class ServerImpl : boost::noncopyable
{
public:
    ServerImpl(std::uint16_t port);

    void Run();

    void RemoveConnection(ConnectionManagerPtr connectionManager);

private:

    void DoAccept();

    asio::io_service ioService_;
    asio::ip::tcp::acceptor acceptor_;
    asio::ip::tcp::socket socket_;
      
    std::map<ConnectionManagerPtr, ConnectionUnit> activeConnections_;
};

}
