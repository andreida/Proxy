#include "pch.h"
#include "Connection.h"
#include "ConnectionManager.h"

namespace ProxyLib {

Connection::Connection(asio::ip::tcp::socket socket
                      , const ConnectionHandlers& handlers)
    : socket_(std::move(socket))
    , resolver_(socket_.get_io_service())
    , readBuffer_(ByteArrayMaxSize)
    , writeBuffer_(ByteArrayMaxSize)
    , handlers_(handlers)
{}

void Connection::ConnectAsync(const std::string& host, std::uint16_t port)
{
    using asio::ip::tcp;

    if (socket_.is_open())
    {
        return;
    }

    ConnectionPtr self(shared_from_this());

    tcp::resolver::query query(host, std::to_string(port));
    resolver_.async_resolve(query
        , [self, this](const boost::system::error_code& ec, tcp::resolver::iterator it)
    {
        try
        {
            if (it == tcp::resolver::iterator())
            {
                throw boost::system::system_error(ec);
            }

            asio::async_connect(socket_, it
                , [self, this](const boost::system::error_code& ec, tcp::resolver::iterator it)
            {
                try
                {
                    if (it == tcp::resolver::iterator())
                    {
                        throw boost::system::system_error(ec);
                    }

                    self->handlers_.OnConnected();
                }
                catch (const std::exception& e)
                {
                    self->handlers_.OnError(this);
                }
            });
        }
        catch (const std::exception& e)
        {
            self->handlers_.OnError(this);
        }
    });
}

void Connection::ReadAsync()
{
    assert(socket_.is_open());

    ConnectionPtr self(shared_from_this());
    
    socket_.async_read_some(asio::buffer(readBuffer_)
        , [this, self](const boost::system::error_code& ec, std::size_t bytesRead)
    {
        try
        {
            if (ec)
            {
                throw boost::system::system_error(ec);
            }

            handlers_.OnDataArrival(bytesRead, readBuffer_);
        }
        catch (const std::exception& e)
        {
            handlers_.OnError(this);
        }
    });
}

void Connection::WriteAsync(std::size_t size, ByteArray& buffer)
{
    assert(socket_.is_open());

    ConnectionPtr self(shared_from_this());

    asio::async_write(socket_, asio::buffer(buffer, size)
        , [this, self](const boost::system::error_code& ec, std::size_t size)
    {
        try
        {
            if (ec)
            {
                throw boost::system::system_error(ec);
            }

            handlers_.OnDataSent(size);
        }
        catch (const std::exception& e)
        {
            handlers_.OnError(this);
        }
    });
}

void Connection::Stop()
{
    socket_.close();
}

}
