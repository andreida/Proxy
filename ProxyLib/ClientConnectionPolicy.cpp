#include "pch.h"
#include "ClientConnectionPolicy.h"
#include "ConnectionManager.h"
#include "IConnection.h"

namespace ProxyLib {

ClientConnectionPolicy::ClientConnectionPolicy(ConnectionManager& connectionManager)
    : connectionManager_(connectionManager)
{}

void ClientConnectionPolicy::ProcessNewMessage(IConnection& server
                                              , RequestInfoPtr requestInfo)
{
    server.ConnectAsync(requestInfo->Host(), requestInfo->Port());

    PendingRequests& requests = connectionManager_.Requests();

    requests.push(std::move(requestInfo));
}

RequestInfo& ClientConnectionPolicy::CurrentMessage()
{
    PendingRequests& requests = connectionManager_.Requests();

    return *requests.front();
}

}
