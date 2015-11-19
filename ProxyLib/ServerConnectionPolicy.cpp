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
    ByteArray responseBuffer = responseInfo->Buffer();
    const std::size_t requestBufferSize = responseBuffer.size();

    client.WriteAsync(requestBufferSize, responseBuffer);

    PendingRequests& requests = connectionManager_.Requests();

    RequestInfo& requestInfo = *requests.back();

    requestInfo.SetResponseInfo(std::move(responseInfo));
}

ResponseInfo& ServerConnectionPolicy::CurrentMessage()
{
    PendingRequests& requests = connectionManager_.Requests();

    RequestInfo& requestInfo = *requests.back();
    
    return requestInfo.Response();
}

}
