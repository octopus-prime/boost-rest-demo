#pragma once

#include "../http/http_client.hpp"
#include "book.hpp"

struct books_client {
    books_client();

    void post_book(const book& b) const;
    book get_book(std::string_view isbn) const;

private:
    http_client client;
};
