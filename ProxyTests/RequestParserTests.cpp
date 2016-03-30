#include "pch.h"

#include <RequestParser.h>

#define BOOST_TEST_MODULE TestRoot
#include <boost/test/unit_test.hpp>

using namespace ProxyLib;

BOOST_AUTO_TEST_SUITE(RequestParserSuite)

BOOST_AUTO_TEST_CASE(EmptyRequestTest)
{
    RequestParser parser;

    ByteArray data;

    ByteArray::iterator begin = data.begin();
    BOOST_CHECK(!parser.Parse(begin, data.end()));
}

BOOST_AUTO_TEST_CASE(InvalidHttpRequestTest)
{
    RequestParser parser;

    const char str[] = "invalid request\r\n\r\n";

    ByteArray data(str, str + sizeof(str));

    ByteArray::iterator begin = data.begin();

    BOOST_CHECK_THROW(parser.Parse(begin, data.end()), std::exception);
}

BOOST_AUTO_TEST_CASE(GetRequestTest)
{
    RequestParser parser;

    const char str[] = "GET http://aaa.com/resource HTTP/1.1\r\n\r\n";

    ByteArray data(str, str + sizeof(str));

    ByteArray::iterator begin = data.begin();

    RequestParser::ResultT message = parser.Parse(begin, data.end());

    BOOST_REQUIRE(message);

    BOOST_CHECK_EQUAL(message->Host(), "aaa.com");

    BOOST_CHECK_EQUAL(message->Port(), 80u);

    BOOST_CHECK_EQUAL(message->BodyLength(), 0u);

    const ByteArray& requestBuffer = message->Buffer();

    const char expectedBuffer[] = "GET /resource HTTP/1.1\r\nHost: aaa.com\r\n\r\n";

    BOOST_CHECK_EQUAL_COLLECTIONS(requestBuffer.begin(), requestBuffer.end()
        , expectedBuffer, expectedBuffer + sizeof(expectedBuffer) - 1);
}

BOOST_AUTO_TEST_SUITE_END()
