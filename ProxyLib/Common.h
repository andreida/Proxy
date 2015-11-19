#pragma once

namespace ProxyLib {

namespace asio = boost::asio;

enum
{
    ByteArrayMaxSize = 4096
};

enum ConnectionOptions
{
    Connection_KeepAlive,
    Connection_Close
};

typedef std::vector<char> ByteArray;
typedef std::vector<std::string> StringList;

}

