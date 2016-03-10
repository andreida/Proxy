#pragma once

#include "Common.h"

#include "RequestParser.h"
#include "ResponseParser.h"
#include "ClientConnectionPolicy.h"
#include "ServerConnectionPolicy.h"

#include "ConnectionHandler.h"

#include <memory>
#include <queue>
#include <functional>

namespace ProxyLib {

struct IConnection;

class ConnectionManager;
class ClientConnectionPolicy;
class ServerConnectionPolicy;

typedef std::shared_ptr<ConnectionManager> ConnectionManagerPtr;
typedef std::function<void(ConnectionManagerPtr)> RemoveConnectionCallback;

typedef std::queue<RequestInfoPtr> PendingRequests;
typedef std::queue<ResponseInfoPtr> PendingResponses;

class ConnectionManager : boost::noncopyable
    , public std::enable_shared_from_this<ConnectionManager>
{
public:
    ConnectionManager(RemoveConnectionCallback removeConnection);

    void SetConnections(IConnection& client, IConnection& server);

    void OnClientDataArrival(std::size_t size
                            , ByteArray& buffer);

    void OnDataSentToClient(std::size_t size);

    void OnServerConnected();

    void OnServerDataArrival(std::size_t size
                            , ByteArray& buffer);

    void OnDataSentToServer(std::size_t size);

    void Stop(const IConnection* sender);

    PendingRequests& Requests();

    PendingResponses& Responses();

private:

    RemoveConnectionCallback removeConnection_;
    PendingRequests pendingRequests_;
    PendingResponses pendingResponses_;
    ConnectionHandler<RequestParser, ClientConnectionPolicy> clientSide_;
    ConnectionHandler<ResponseParser, ServerConnectionPolicy> serverSide_;
};

}
