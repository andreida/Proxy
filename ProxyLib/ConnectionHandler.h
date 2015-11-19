#pragma once

#include "IConnection.h"

namespace ProxyLib {

class ConnectionManager;

template <class ParserT, class PolicyT>
class ConnectionHandler : boost::noncopyable
{
    typedef typename ParserT::MessageT MessageT;
    typedef typename ParserT::ResultT MessagePtrT;

public:

    ConnectionHandler(ConnectionManager& connectionManager)
        : connectionPolicy_(connectionManager)
        , state_(State_WaitMessage)
        , parser_()
        , client_()
        , server_()
    {}

    void SetConnections(IConnection& client
        , IConnection& server)
    {
        client_ = &client;
        server_ = &server;
    }

    void OnDataArrival(std::size_t size, ByteArray& buffer)
    {
        if (state_ == State_WaitMessage)
        {
            MessagePtrT message = parser_.Parse(size, buffer);

            if (message)
            {
                connectionPolicy_.ProcessNewMessage(*server_, std::move(message));

                state_ = State_WaitDataSent;
            }
        }
        else
        {
            assert(state_ == State_WaitData);

            MessageT& message = connectionPolicy_.CurrentMessage();
            message.ProcessChunk(buffer.data(), size);

            server_->WriteAsync(size, buffer);

            state_ = State_WaitDataSent;
        }

        if (state_ != State_WaitDataSent)
        {
            client_->ReadAsync();
        }
    }

    void OnDataSent(std::size_t size)
    {
        assert(state_ == State_WaitDataSent);

        MessageT& message = connectionPolicy_.CurrentMessage();

        message.SetBytesTransfered(size);

        if (message.IsComplete())
        {
            state_ = State_WaitMessage;
            server_->ReadAsync();
        }
        else
        {
            state_ = State_WaitData;
        }

        client_->ReadAsync();
    }

    void OnConnected()
    {
        assert(state_ == State_WaitDataSent);

        MessageT& message = connectionPolicy_.CurrentMessage();

        ByteArray buffer = message.Buffer();

        const std::size_t size = buffer.size();
        server_->WriteAsync(size, std::move(buffer));
    }

    bool IsClient(const IConnection* connection) const
    {
        return (client_ == connection);
    }

    void Stop()
    {
        client_->Stop();
    }

private:

    enum State
    {
        State_WaitMessage,
        State_WaitData,
        State_WaitDataSent
    };

    PolicyT connectionPolicy_;
    State   state_;
    ParserT parser_;
    IConnection* client_;
    IConnection* server_;
};

}
