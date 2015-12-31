#include "pch.h"
#include "RequestParser.h"
#include "RequestInfo.h"
#include "Utility.h"

#include <boost/optional.hpp>

#include <algorithm>
#include <initializer_list>

namespace ProxyLib {

namespace qi = boost::spirit::qi;
namespace ascii = qi::ascii;


static const std::array<std::string, 2> methods_g = {"GET", "POST"};
static const std::string connectionHeaderStr_g = "Connection";
const auto newLine_g = { '\r', '\n' };
const auto blankLine_g = { '\r', '\n', '\r', '\n' };

bool HeadersReseived(ByteArray::iterator begin, ByteArray::iterator end)
{
    ByteArray::const_iterator it = std::search(begin, end, blankLine_g.begin()
        , blankLine_g.end());

    return (it != end);
}

typedef std::multimap<std::string, std::string
    , LexicographicalCompare<std::string>> HeadersMap;

HeadersMap GetHeadersMap(const StringList& clientHeaders)
{
    HeadersMap headers;

    auto it = clientHeaders.begin();
    while (it != clientHeaders.end())
    {
        auto value = ++it;
        headers.emplace(*it, *value);

        ++it;
    }

    return std::move(headers);
}

ConnectionOptions ProcessConnectionHeader(const boost::optional<StringList>& clientHeaders)
{
    ConnectionOptions connectionOpt = Connection_KeepAlive;
    if (clientHeaders)
    {
        assert((clientHeaders->size() % 2) == 0);

        const HeadersMap headersMap = GetHeadersMap(*clientHeaders);
        
        auto range = headersMap.equal_range(connectionHeaderStr_g);

        StringList connectionTokens;

        if (range.first != range.second)
        {
            for (auto it = range.first; it != range.second; ++it)
            {
                const std::string& headerValue = it->second;

                auto space = +(qi::lit('\t') | qi::lit(' '));
                auto token = +(ascii::print - qi::lit(' ') | qi::lit('\t'));
                
                auto parser = *space
                    >> *(token >> *(*space >> qi::lit(',') >> *space >> token));

                std::string::const_iterator begin = headerValue.begin();
                qi::parse(begin, headerValue.end(), parser, connectionTokens);
            }
        }
    }

    return connectionOpt;
}

std::uint32_t MessageBodyLength(HttpMethods method, boost::optional<StringList> headers)
{
    return 0;
}

void RemoveFolding(ByteArray::iterator begin, ByteArray::iterator end)
{
    //todo
}

RequestParser::RequestParser()
    : spaceParser_()
    , entryParser_()
    , methodsParser_()
    , urlParser_()
    , requestHeadersParser_()
    , requestParser_()
{
    spaceParser_ = +(qi::lit('\t') | qi::lit(' '));

    entryParser_ = +(ascii::print - (qi::lit(':') | qi::lit('@') | qi::lit(' ') | qi::lit('\t')));

    urlParser_ = "http://"
        >> -(entryParser_ >> ':' >> entryParser_ >> '@')
        >> entryParser_
        >> (-(':' >> qi::ushort_))
        >> -('/' >> -entryParser_);

    methodsParser_.add(methods_g.at(Http_Get), Http_Get)
        (methods_g.at(Http_Post), Http_Post);

    requestHeadersParser_ = *("\r\n" >> spaceParser_ >> ':' >> *spaceParser_ >> *ascii::print);

    requestParser_ = *spaceParser_
        >> methodsParser_
        >> +spaceParser_
        >> urlParser_
        >> +spaceParser_
        >> "HTTP1/1"
        >> requestHeadersParser_
        >> "\r\n\r\n";
}

RequestInfoPtr RequestParser::Parse(ByteArray::iterator& begin
                                   , ByteArray::iterator end)
{
    if (HeadersReseived(begin, end))
    {
        RemoveFolding(begin, end);

        return ParseRequest(begin, end);
    }

    return RequestInfoPtr();
}

RequestInfoPtr RequestParser::ParseRequest(ByteArray::iterator& begin
                                          , ByteArray::iterator end)
{
    RequestTuple res;
    qi::parse(begin, end, requestParser_, res);

    /*HttpMethods method = Http_Get;
    boost::optional<StringList> cred;
    std::string host;
    boost::optional<std::uint16_t> port;
    boost::optional<std::string> resource;
    boost::optional<StringList> headers;*/
    
    RequestInfoPtr requestInfo;
    /*if (qi::parse(begin, end, requestParser, method
        , cred, host, port, resource, headers))
    {
        const ConnectionOptions connectionOptions = ProcessConnectionHeader(headers);
        const std::uint32_t messageLength = MessageBodyLength(method, headers);

        ByteArray requestBuffer = CreateRequestBuffer(method, host, resource, headers);

        requestInfo = std::make_unique<RequestInfo>(std::move(host), port, connectionOptions
            , messageLength, std::move(requestBuffer));
    }
    else
    {
        throw std::exception("invalid request");
    }*/

    return std::move(requestInfo);
}

ByteArray RequestParser::CreateRequestBuffer(HttpMethods method
                                            , const std::string& host
                                            , const boost::optional<std::string>& resource
                                            , const boost::optional<StringList>& clientHeaders)
{
    const std::string& methodStr = methods_g.at(method);

    ByteArray requestBuffer(methodStr.begin(), methodStr.end());

    Append(' ', requestBuffer);

    Append('/', requestBuffer);

    if (resource)
    {
        Append(resource->begin(), resource->end(), requestBuffer);
    }

    Append({' ', 'H', 'T', 'T', 'P', '/', '1', '.', '1', '\r', '\n', 'H', 'o'
        , 's', 't', ':', ' ' }, requestBuffer);

    Append(host.begin(), host.end(), requestBuffer);

    if (clientHeaders)
    {
        AppendHeaders(*clientHeaders, requestBuffer);
    }

    Append(blankLine_g, requestBuffer);

    return std::move(requestBuffer);
}

void RequestParser::AppendHeaders(const StringList& clientHeaders
                                 , ByteArray& requestBuffer)
{
    assert((clientHeaders.size() % 2) == 0);

    StringList::const_iterator it = clientHeaders.begin();
    while (it != clientHeaders.end())
    {
        Append(newLine_g, requestBuffer);

        Append(it->begin(), it->end(), requestBuffer);
        
        Append({ ':', ' ' }, requestBuffer);

        ++it;

        Append(it->begin(), it->end(), requestBuffer);

        ++it;
    }
}

}
