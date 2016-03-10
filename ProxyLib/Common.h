#pragma once

#include <string>
#include <vector>

namespace ProxyLib {

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

