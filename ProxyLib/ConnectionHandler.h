#pragma once

#include "IConnection.h"
#include "Utility.h"

#include <boost/noncopyable.hpp>

namespace ProxyLib {

class ConnectionHandlerBase : boost::noncopyable
{
public:

    void SetConnections(IConnection& client, IConnection& server);

    bool IsClient(const IConnection* connection) const;

    void Stop();

protected:

    ConnectionHandlerBase();

    ~ConnectionHandlerBase();

    void SetBuffer(std::size_t size, ByteArray& buffer);

    enum
    {
        RequestMaxSize = 1024 * 64
    };

    enum State
    {
        State_WaitMessage,
        State_WaitData,
        State_WaitDataSent
    };

    State        state_;
    ByteArray    buffer_;
    IConnection* client_;
    IConnection* server_;
};

class ConnectionManager;

template <class ParserT, class PolicyT>
class ConnectionHandler : public ConnectionHandlerBase
{
    typedef typename ParserT::ResultT MessagePtrT;
    typedef typename MessagePtrT::element_type MessageT;

public:

    ConnectionHandler(ConnectionManager& connectionManager)
        : connectionPolicy_(connectionManager)
        , parser_()
        , sendBuffer_()
    {}

    void OnDataArrival(std::size_t size, ByteArray& buffer)
    {
        assert(size <= buffer.size());

        SetBuffer(size, buffer);
        
        ByteArray::iterator begin = buffer_.begin();
        ByteArray::iterator end = begin + size;

        while (begin != end)
        {
            if (state_ == State_WaitMessage)
            {
                MessagePtrT message = parser_.Parse(begin, end);

                if (message)
                {
                    connectionPolicy_.ProcessNewMessage(*server_, std::move(message));

                    if (0 < message->BodyLength())
                    {
                        state_ = State_WaitData;
                    }
                }
                else
                {
                    break;
                }
            }
            else
            {
                MessageT& message = connectionPolicy_.CurrentMessage();

                assert((0 < message.BodyLength()) && (state_ == State_WaitData));

                ByteArray::iterator messageEnd = end;
                if (message.BodyLength() <= std::distance(begin, end))
                {
                    messageEnd = begin + message.BodyLength();
                    state_ = State_WaitMessage;
                }

                message.SetMessageBodyChunk(begin, messageEnd);

                begin = messageEnd;
            }
        }

        if (begin != buffer_.begin())
        {
            buffer_.swap(ByteArray(begin, end));
        }

        client_->ReadAsync();
    }

    void OnDataSent(std::size_t size)
    {
        while (0 < size)
        {
            MessageT& message = connectionPolicy_.CurrentMessage();

            const std::size_t bytesLeft = message.BytesLeftToSend();

            message.SetBytesSent(size);

            if (bytesLeft <= size)
            {
                connectionPolicy_.CurrentMessageSent();
                size -= bytesLeft;
            }
            else
            {
                break;
            }
        }

        connectionPolicy_.FillSendBuffer(sendBuffer_);

        server_->WriteAsync(sendBuffer_);
    }

private:

    PolicyT   connectionPolicy_;
    ParserT   parser_;
    ByteArray sendBuffer_;
};

}
