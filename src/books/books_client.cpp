#include "books_client.hpp"

books_client::books_client() : client{"http://127.0.0.1:8080"} {
}

void books_client::post_book(const book& b) const {
    boost::urls::static_url<100> url;//{client.base_url()};
    boost::urls::format_to(url, "/books");
    client.execute<json_body>(http::verb::post, url, json::value{});// json::value_from(b));
}

book books_client::get_book(std::string_view isbn) const {
    boost::urls::static_url<100> url;//{client.base_url()};
    boost::urls::format_to(url, "/books/{}", isbn);
    return json::value_to<book>(client.execute<json_body>(http::verb::get, url));
}
