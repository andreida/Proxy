#pragma once

#include "Common.h"

#include "Connection.h"
#include "ConnectionManager.h"

#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>

#include <cstdint>
#include <map>

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

    boost::asio::io_service                        ioService_;
    boost::asio::ip::tcp::acceptor                 acceptor_;
    boost::asio::ip::tcp::socket                   socket_;
      
    std::map<ConnectionManagerPtr, ConnectionUnit> activeConnections_;
};

}
