#include "pch.h"
#include "ConnectionHandler.h"


namespace ProxyLib {

ConnectionHandlerBase::ConnectionHandlerBase()
    : state_(State_WaitMessage)
    , buffer_()
    , client_()
    , server_()
{}

ConnectionHandlerBase::~ConnectionHandlerBase()
{}

void ConnectionHandlerBase::SetConnections(IConnection& client
                                          , IConnection& server)
{
    client_ = &client;
    server_ = &server;
}

bool ConnectionHandlerBase::IsClient(const IConnection* connection) const
{
    return (client_ == connection);
}

void ConnectionHandlerBase::Stop()
{
    client_->Stop();
}

void ConnectionHandlerBase::SetBuffer(std::size_t size, ByteArray& buffer)
{
    if (RequestMaxSize < (size + buffer_.size()))
    {
        throw std::exception("too long request");
    }

    Append(buffer.begin(), buffer.begin() + size, buffer_);
}

}
