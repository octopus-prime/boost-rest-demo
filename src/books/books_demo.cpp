#include "books_client.hpp"
#include <print>

int main() {
    try {
        const books_client client;
        const auto book = client.get_book("123");
        std::println("Retrieved book: {}", book);

    } catch (const std::exception& e) {
        std::println("Error: {}", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
