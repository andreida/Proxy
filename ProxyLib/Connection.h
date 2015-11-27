#pragma once

#include "IConnection.h"
#include "ConnectionManager.h"

namespace ProxyLib {

typedef std::function<void()> ConnectedHandler;
typedef std::function<void (std::size_t, ByteArray&)> DataArrivalHandler;
typedef std::function<void(std::size_t)> DataSentHandler;
typedef std::function<void(IConnection*)> ErrorHandler;

struct ConnectionHandlers
{
    ConnectedHandler   OnConnected;
    DataArrivalHandler OnDataArrival;
    DataSentHandler    OnDataSent;
    ErrorHandler       OnError;
};

class Connection : public IConnection
    , public std::enable_shared_from_this<Connection>
{
public:
    Connection(asio::ip::tcp::socket socket, const ConnectionHandlers& handlers);

    virtual void ConnectAsync(const std::string& host, std::uint16_t port);

    virtual void ReadAsync();

    virtual void WriteAsync(const ByteArray& buffer);

    virtual void Stop();

private:

    asio::ip::tcp::socket    socket_;
    asio::ip::tcp::resolver  resolver_;
    ByteArray                readBuffer_;
    const ConnectionHandlers handlers_;
};

typedef std::shared_ptr<Connection> ConnectionPtr;

}
