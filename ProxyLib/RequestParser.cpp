#include "pch.h"
#include "RequestParser.h"
#include "RequestInfo.h"

#include <boost/spirit/include/qi.hpp>
#include <boost/optional.hpp>

#include <algorithm>

namespace ProxyLib {

namespace qi = boost::spirit::qi;
namespace ascii = qi::ascii;

enum
{
    RequestMaxSize = 1024 * 64
};

bool HeadersReseived(const ByteArray& buffer)
{
    const char blankLine[] = "\r\n\r\n";

    ByteArray::const_iterator it = std::search(buffer.begin(), buffer.end()
        , blankLine, &blankLine[sizeof(blankLine - 1)]);

    return (it != buffer.end());
}

ConnectionOptions ProcessConnectionHeader(const boost::optional<StringList>& headers)
{
    ConnectionOptions connectionOpt = Connection_KeepAlive;
    if (headers.is_initialized())
    {
        
    }

    return connectionOpt;
}

std::uint32_t MessageLength(HttpMethods method, boost::optional<StringList> headers)
{
    return 0;
}

void RemoveFolding(ByteArray& buffer)
{
    //todo
}

RequestInfoPtr RequestParser::Parse(std::size_t size, ByteArray& buffer)
{
    if (tempBuffer_.empty())
    {
        tempBuffer_.swap(buffer);
    }
    else
    {
        tempBuffer_.insert(tempBuffer_.end(), &buffer[0], &buffer[size]);
    }

    if (HeadersReseived(tempBuffer_))
    {
        return ParseRequest();
    }

    if (RequestMaxSize <= tempBuffer_.size())
    {
        throw std::exception("too long request");
    }

    return RequestInfoPtr();
}

RequestInfoPtr RequestParser::ParseRequest()
{
    RemoveFolding(tempBuffer_);

    qi::symbols<char, HttpMethods> knownMethods;

    knownMethods.add("GET", Http_Get)
        ("POST", Http_Post);

    auto entry = +(ascii::print - (qi::lit(':') | qi::lit('@') | qi::lit(' ')));

    auto url = "http://"
        >> -(entry >> ':' >> entry >> '@')
        >> entry
        >> (-(':' >> qi::ushort_))
        >> -('/' >> -entry);

    auto space = +(qi::lit('\t') | qi::lit(' '));

    auto requestHeaders = *("\r\n" >> entry >> ':' >> *space >> *ascii::print);

    auto requestParser = *space
        >> knownMethods
        >> +space
        >> url
        >> +space
        >> "HTTP1/1"
        >> requestHeaders
        >> "\r\n\r\n";

    ByteArray::iterator begin = tempBuffer_.begin();

    HttpMethods method = Http_Get;
    boost::optional<StringList> cred;
    std::string host;
    boost::optional<std::uint16_t> port;
    boost::optional<std::string> resource;
    boost::optional<StringList> headers;
    
    if (qi::parse(begin, tempBuffer_.end(), requestParser, method
        , cred, host, port, resource, headers))
    {
        const ConnectionOptions connectionOptions = ProcessConnectionHeader(headers);
        const std::uint32_t messageLength = MessageLength(method, headers);

        FillRequestBuffer(method, host, port, resource, headers, begin);

        return std::make_unique<RequestInfo>(std::move(host), port, connectionOptions
            , messageLength, std::move(tempBuffer_));
    }
    else
    {
        throw std::exception("invalid request");
    }

    return RequestInfoPtr();
}

void RequestParser::FillRequestBuffer(HttpMethods method
                                     , const std::string& host
                                     , const boost::optional<std::uint16_t>& port
                                     , const boost::optional<std::string>& resource
                                     , const boost::optional<StringList>& headers
                                     , ByteArray::iterator messageBodyPos)
{

}

}
