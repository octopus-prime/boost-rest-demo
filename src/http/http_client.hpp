#pragma once

#include <boost/json.hpp>
#include <boost/url.hpp>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
// #include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
// #include <boost/asio/ssl/error.hpp>
// #include <boost/asio/ssl/stream.hpp>

#include "json_body.hpp"

namespace beast = boost::beast; // from <boost/beast.hpp>
namespace http = beast::http;   // from <boost/beast/http.hpp>
namespace net = boost::asio;    // from <boost/asio.hpp>
namespace ssl = net::ssl;       // from <boost/asio/ssl.hpp>

using tcp = net::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

struct http_client {
    http_client(std::string_view url_);

    ~http_client();

    // const boost::urls::url_view& base_url() const {
    //     return url;
    // }

    template <typename ResponseBody = http::dynamic_body>
    ResponseBody::value_type execute(http::verb verb, boost::urls::url_view url) const;

    template <typename ResponseBody = http::dynamic_body>
    ResponseBody::value_type execute(http::verb verb, boost::urls::url_view url, json_body::value_type body) const;

private:
    template <typename ResponseBody, typename RequestBody>
    http::response<ResponseBody> execute(http::request<RequestBody> req) const;

    boost::urls::url_view url;
    net::io_context ioc;
    mutable beast::tcp_stream stream;
    mutable beast::flat_buffer buffer;
};
