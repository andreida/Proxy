#pragma once

#include "Common.h"
#include "RequestInfo.h"

#include <boost/optional/optional_fwd.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/sequence.hpp>

namespace ProxyLib {

enum HttpMethods
{
    Http_Get,
    Http_Post
};

class RequestParser
{
public:
    typedef RequestInfo MessageT;
    typedef RequestInfoPtr ResultT;

    RequestParser();

    RequestInfoPtr Parse(ByteArray::iterator& begin
                        , ByteArray::iterator end);

private:

    typedef boost::fusion::vector<boost::optional<StringList>
        , std::string
        , boost::optional<std::uint16_t>
        , boost::optional <std::string>> UrlTuple;

    typedef boost::spirit::qi::rule<ByteArray::iterator, std::string()> UrlEntryParser;
    typedef boost::spirit::qi::rule<ByteArray::iterator, UrlTuple()> UrlParser;
    typedef boost::spirit::qi::rule<ByteArray::iterator, void()> SpaceParser;
    typedef boost::spirit::qi::rule<ByteArray::iterator, boost::optional<StringList>()> RequestHeadersParser;

    typedef boost::fusion::vector <HttpMethods
        , UrlTuple
        , boost::optional <StringList>> RequestTuple;
    typedef boost::spirit::qi::rule<ByteArray::iterator, RequestTuple()> HttpRequestParser;


    RequestInfoPtr ParseRequest(ByteArray::iterator& begin
                                      , ByteArray::iterator end);

    static ByteArray CreateRequestBuffer(HttpMethods method
                                        , const std::string& host
                                        , const boost::optional<std::string>& resource
                                        , const boost::optional<StringList>& headers);

    static void AppendHeaders(const StringList& clientHeaders
                             , ByteArray& requestBuffer);

    SpaceParser                    spaceParser_;
    UrlEntryParser                 entryParser_;

    boost::spirit::qi::symbols<char, HttpMethods> methodsParser_;
    UrlParser                                     urlParser_;
    RequestHeadersParser                          requestHeadersParser_;
    HttpRequestParser                             requestParser_;
};

}
