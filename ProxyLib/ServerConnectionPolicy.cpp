#include "pch.h"
#include "ServerConnectionPolicy.h"
#include "IConnection.h"
#include "ConnectionManager.h"

namespace ProxyLib {

ServerConnectionPolicy::ServerConnectionPolicy(ConnectionManager& connectionManager)
    : connectionManager_(connectionManager)
{}

void ServerConnectionPolicy::ProcessNewMessage(IConnection& client
                                              , ResponseInfoPtr responseInfo)
{
    client.WriteAsync(responseInfo->Buffer());

    PendingResponses& responses = connectionManager_.Responses();

    responses.push(std::move(responseInfo));
}

ResponseInfo& ServerConnectionPolicy::CurrentMessage()
{
    PendingResponses& responses = connectionManager_.Responses();
    
    return *responses.back();
}

void ServerConnectionPolicy::CurrentMessageSent()
{
    PendingResponses& responses = connectionManager_.Responses();

    responses.pop();
}

void ServerConnectionPolicy::FillSendBuffer(ByteArray& buffer)
{

}

}
