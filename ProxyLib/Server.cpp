#include "pch.h"
#include "Server.h"
#include "ServerImpl.h"

namespace ProxyLib {

Server::Server(unsigned short port)
    : impl_(new ServerImpl(port))
{}

Server::~Server()
{
    delete impl_;
}

void Server::Run()
{
    impl_->Run();
}

}
