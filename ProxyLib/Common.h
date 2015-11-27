#pragma once

namespace ProxyLib {

namespace asio = boost::asio;

enum
{
    ByteArrayDefaultSize = 4096
};

enum ConnectionOptions
{
    Connection_KeepAlive,
    Connection_Close
};

typedef std::vector<char> ByteArray;
typedef std::vector<std::string> StringList;

}

