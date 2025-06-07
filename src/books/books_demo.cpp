#include "books_client.hpp"
#include <print>

int main() {
    try {
        const books_client client;
        client.ping_books();
        client.post_book(book{"123", "C++ Programming", "John Doe", 2023});
        const auto book = client.get_book("123");
        std::println("Retrieved book: {}", book);
        const auto all_books = client.get_books({{"author", "Foo"}, {"title", "Bar"}, {"year", "2023"}});
        std::println("Books:{}", all_books);
    } catch (const std::exception& e) {
        std::println("Error: {}", e.what());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
