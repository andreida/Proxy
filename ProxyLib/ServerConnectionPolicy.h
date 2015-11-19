#pragma once

#include "ResponseInfo.h"

namespace ProxyLib {

struct IConnection;
class ConnectionManager;

class ServerConnectionPolicy : boost::noncopyable
{
public:

    ServerConnectionPolicy(ConnectionManager& connectionManager);

    void ProcessNewMessage(IConnection& client, ResponseInfoPtr responseInfo);

    ResponseInfo& CurrentMessage();

private:

    ConnectionManager& connectionManager_;
};

}
