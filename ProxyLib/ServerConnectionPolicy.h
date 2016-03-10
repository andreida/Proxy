#pragma once

#include "ResponseInfo.h"

#include <boost/noncopyable.hpp>

namespace ProxyLib {

struct IConnection;
class ConnectionManager;

class ServerConnectionPolicy : boost::noncopyable
{
public:

    ServerConnectionPolicy(ConnectionManager& connectionManager);

    void ProcessNewMessage(IConnection& client, ResponseInfoPtr responseInfo);

    ResponseInfo& CurrentMessage();

    void CurrentMessageSent();

    void FillSendBuffer(ByteArray& buffer);

private:

    ConnectionManager& connectionManager_;
};

}
