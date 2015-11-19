#include "pch.h"
#include "ServerImpl.h"
#include "Connection.h"

namespace ProxyLib {

ServerImpl::ServerImpl(std::uint16_t port)
    : ioService_()
    , acceptor_(ioService_, {asio::ip::tcp::v4(), port})
    , socket_(ioService_)
{
    DoAccept();
}

void ServerImpl::Run()
{
    ioService_.run();
}

void ServerImpl::RemoveConnection(ConnectionManagerPtr connectionManager)
{
    activeConnections_.erase(connectionManager);
}

void ServerImpl::DoAccept()
{
    acceptor_.async_accept(socket_
        , [this](boost::system::error_code ec)
    {
        using std::placeholders::_1;
        using std::placeholders::_2;

        if (ec)
        {
            throw boost::system::system_error(ec);
        }

        ConnectionManagerPtr manager = std::make_shared<ConnectionManager>(
            std::bind(&ServerImpl::RemoveConnection, this, _1));

        const ConnectionHandlers clientHandlers = { nullptr
            , std::bind(&ConnectionManager::OnClientDataArrival, manager, _1, _2)
            , std::bind(&ConnectionManager::OnDataSentToServer, manager, _1)
            , std::bind(&ConnectionManager::Stop, manager, _1) };

        ConnectionPtr client = std::make_shared<Connection>(std::move(socket_)
            , clientHandlers);

        const ConnectionHandlers serverHandlers = { std::bind(&ConnectionManager::OnServerConnected, manager)
            , std::bind(&ConnectionManager::OnServerDataArrival, manager, _1, _2)
            , std::bind(&ConnectionManager::OnDataSentToClient, manager, _1)
            , std::bind(&ConnectionManager::Stop, manager, _1) };

        ConnectionPtr server = std::make_shared<Connection>(asio::ip::tcp::socket(ioService_)
            , serverHandlers);

        manager->SetConnections(*client, *server);

        activeConnections_.emplace(manager, std::make_pair(client, server));

        client->ReadAsync();

        DoAccept();
    });
}

}
