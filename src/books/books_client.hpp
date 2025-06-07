#pragma once

#include "../http/http_client.hpp"
#include "book.hpp"

struct books_client {
    books_client();

    void ping_books() const;
    void post_book(const book& b) const;
    book get_book(std::string_view isbn) const;
    books get_books(std::initializer_list<boost::urls::param_view> params) const;

private:
    http_client client;
};
