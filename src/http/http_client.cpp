#include "http_client.hpp"
// #include <boost/json/src.hpp>
#include <iostream>

http_client::http_client(std::string_view url_) : url{url_}, ioc{}, stream{ioc}, buffer{} {
    tcp::resolver resolver{ioc};
    auto const results = resolver.resolve(url.host(), url.has_port() ? url.port() : url.scheme());
    stream.connect(results);
}

http_client::~http_client() {
    beast::error_code ec;
    stream.socket().shutdown(tcp::socket::shutdown_both, ec);
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

    std::clog << "Request: " << req << std::endl;
    http::write(stream, req);
    http::read(stream, buffer, res);
    std::clog << "Response: " << res << std::endl;

    if (to_status_class(res.result()) != http::status_class::successful) {
        throw std::runtime_error("Request failed");
    }

    return std::move(res);
}


// template http::empty_body::value_type http_client::execute<http::empty_body>(http::verb, boost::urls::url_view) const;
// template http::empty_body::value_type http_client::execute<http::empty_body>(http::verb, boost::urls::url_view, json_body::value_type) const;
template json_body::value_type http_client::execute<json_body>(http::verb, boost::urls::url_view) const;
template json_body::value_type http_client::execute<json_body>(http::verb, boost::urls::url_view, json_body::value_type) const;

template http::dynamic_body::value_type http_client::execute<http::dynamic_body>(http::verb, boost::urls::url_view) const;
template http::dynamic_body::value_type http_client::execute<http::dynamic_body>(http::verb, boost::urls::url_view, json_body::value_type) const;
