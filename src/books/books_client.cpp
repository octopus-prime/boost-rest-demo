#include "books_client.hpp"

books_client::books_client() : client{"http://127.0.0.1:8080"} {
}

void books_client::ping_books() const {
    boost::urls::static_url<100> url;
    boost::urls::format_to(url, "/books");
    client.execute(http::verb::head, url);
}

void books_client::post_book(const book& b) const {
    boost::urls::static_url<100> url;
    boost::urls::format_to(url, "/books");
    client.execute(http::verb::post, url, json::value_from(b));
}

book books_client::get_book(std::string_view isbn) const {
    boost::urls::static_url<100> url;
    boost::urls::format_to(url, "/books/{}", isbn);
    return json::value_to<book>(client.execute<json_body>(http::verb::get, url));
}

books books_client::get_books(std::initializer_list<boost::urls::param_view> params) const {
    boost::urls::static_url<100> url;
    boost::urls::format_to(url, "/books");
    url.set_params(params);
    return json::value_to<books>(client.execute<json_body>(http::verb::get, url));
}
