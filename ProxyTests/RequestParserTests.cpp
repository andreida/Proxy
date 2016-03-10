#include "pch.h"

#include <RequestParser.h>

#define BOOST_TEST_MODULE TestRoot
#include <boost/test/unit_test.hpp>

using namespace ProxyLib;

BOOST_AUTO_TEST_SUITE(RequestParserSuite)

BOOST_AUTO_TEST_CASE(EmptyInputTest)
{
    RequestParser parser;

    ByteArray data;

    ByteArray::iterator begin = data.begin();
    BOOST_CHECK(!parser.Parse(begin, data.end()));
}

BOOST_AUTO_TEST_CASE(NotHttpRequestTest)
{
    RequestParser parser;

    const char str[] = "not a http request";

    ByteArray data(str, str + sizeof(str));

    ByteArray::iterator begin = data.begin();

    BOOST_CHECK(!parser.Parse(begin, data.end()));
}

BOOST_AUTO_TEST_SUITE_END()

