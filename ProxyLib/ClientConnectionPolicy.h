#pragma once

#include "RequestInfo.h"

namespace ProxyLib {

struct IConnection;
class ConnectionManager;

class ClientConnectionPolicy : boost::noncopyable
{
public:

    ClientConnectionPolicy(ConnectionManager& connectionManager);

    void ProcessNewMessage(IConnection& server, RequestInfoPtr requestInfo);

    RequestInfo& CurrentMessage();

    void CurrentMessageSent();

    void FillSendBuffer(ByteArray& buffer);

private:

    ConnectionManager& connectionManager_;
};

}
