#include "pch.h"
#include "ConnectionManager.h"

namespace ProxyLib {


ConnectionManager::ConnectionManager(RemoveConnectionCallback removeConnection)
    : removeConnection_(removeConnection)
    , pendingRequests_()
    , clientSide_(*this)
    , serverSide_(*this)
{}

void ConnectionManager::SetConnections(IConnection& client
                                      , IConnection& server)
{
    clientSide_.SetConnections(client, server);
    serverSide_.SetConnections(server, client);
}

void ConnectionManager::OnClientDataArrival(std::size_t size
                                           , ByteArray& buffer)
{
    clientSide_.OnDataArrival(size, buffer);
}

void ConnectionManager::OnDataSentToClient(std::size_t size)
{
    serverSide_.OnDataSent(size);
}

void ConnectionManager::OnServerConnected()
{
    clientSide_.OnConnected();
}

void ConnectionManager::OnServerDataArrival(std::size_t size
                                           , ByteArray& buffer)
{
    serverSide_.OnDataArrival(size, buffer);
}

void ConnectionManager::OnDataSentToServer(std::size_t size)
{
    clientSide_.OnDataSent(size);
}

void ConnectionManager::Stop(const IConnection* sender)
{
    if (clientSide_.IsClient(sender))
    {
        clientSide_.Stop();
        serverSide_.Stop();
    }
    else
    {
        //todo: check if keep-alive connection, or if retries are necessary
    }

    removeConnection_(shared_from_this());
}

PendingRequests& ConnectionManager::Requests()
{
    return pendingRequests_;
}


}
