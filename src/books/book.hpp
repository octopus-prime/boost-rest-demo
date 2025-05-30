#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include <format>
#include <chrono>
#include <boost/describe.hpp>

struct book {
    std::string isbn;
    std::string title;
    std::string author;
    int year;
};

BOOST_DESCRIBE_STRUCT(book, (), (isbn, title, author, year))

template<typename Char>
struct std::formatter<book, Char> : std::formatter<std::string_view, Char> {
    auto format(const book& x, auto &ctx) const {
        return format_to(ctx.out(), "{} {} {} {}", x.isbn, x.title, x.author, x.year);
    }
};

using books = std::vector<book>;
