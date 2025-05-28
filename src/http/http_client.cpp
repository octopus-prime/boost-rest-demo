#include "http_client.hpp"
// #include <boost/json/src.hpp>
#include <iostream>

http_client::http_client(std::string_view url_, ssl::context::method method) : url{url_}, ioc{}, ctx{method}, stream{ioc, ctx}, buffer{} {
    ctx.set_verify_mode(ssl::verify_peer);
    tcp::resolver resolver{ioc};

    // Set SNI Hostname (many hosts need this to handshake successfully)
    if(! SSL_set_tlsext_host_name(stream.native_handle(), url.host().c_str()))
    {
        boost::system::error_code ec{static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category()};
        throw boost::system::system_error{ec};
    }

    auto const results = resolver.resolve(url.host(), url.port());
    beast::get_lowest_layer(stream).connect(results);
    stream.handshake(ssl::stream_base::client);    
}

http_client::~http_client() {
    beast::error_code ec;
    stream.shutdown(ec);
}

template <typename ResponseBody>
ResponseBody::value_type http_client::execute(http::verb verb, boost::urls::url_view url) const {
    return execute<ResponseBody>(http::request<http::empty_body>{verb, url, 11}).body();
}

template <typename ResponseBody>
ResponseBody::value_type http_client::execute(http::verb verb, boost::urls::url_view url, json_body::value_type body) const {
    return execute<ResponseBody>(http::request<json_body>{verb, url, 11, body}).body();
}

template <typename ResponseBody, typename RequestBody>
http::response<ResponseBody> http_client::execute(http::request<RequestBody> req) const {
    req.set(http::field::host, url.host());
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    if constexpr (std::same_as<ResponseBody, json_body>) {
        req.set(http::field::accept, "application/json");
    }
    if constexpr (std::same_as<RequestBody, json_body>) {
        req.set(http::field::content_type, "application/json");
        req.prepare_payload();
    }

    http::response<ResponseBody> res;

    // std::clog << "Request: " << req << std::endl;
    http::write(stream, req);
    http::read(stream, buffer, res);
    // std::clog << "Response: " << res << std::endl;

    if (to_status_class(res.result()) != http::status_class::successful) {
        throw std::runtime_error("Request failed");
    }

    return std::move(res);
}


template http::empty_body::value_type http_client::execute<http::empty_body>(http::verb, boost::urls::url_view) const;
template http::empty_body::value_type http_client::execute<http::empty_body>(http::verb, boost::urls::url_view, json_body::value_type) const;
template json_body::value_type http_client::execute<json_body>(http::verb, boost::urls::url_view) const;
template json_body::value_type http_client::execute<json_body>(http::verb, boost::urls::url_view, json_body::value_type) const;
