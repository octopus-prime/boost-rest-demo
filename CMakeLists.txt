cmake_minimum_required(VERSION 3.28)

project(boost_rest_demo LANGUAGES CXX)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_BUILD_TYPE Release)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -O3 -Wall -stdlib=libc++")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (WIN32)
    include(C:/Users/mikeg/source/repos/vcpkg/scripts/buildsystems/vcpkg.cmake)
endif()

find_package(Boost REQUIRED COMPONENTS url json)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)

add_executable(f1_demo src/http/https_client.cpp src/f1/f1_client.cpp src/f1/f1_demo.cpp)
target_link_libraries(f1_demo PRIVATE Boost::url Boost::json Threads::Threads OpenSSL::Crypto OpenSSL::SSL)

add_executable(books_demo src/http/http_client.cpp src/books/books_client.cpp src/books/books_demo.cpp)
target_link_libraries(books_demo PRIVATE Boost::url Boost::json Threads::Threads)

add_executable(broken src/json/broken.cpp)
target_link_libraries(broken PRIVATE Boost::json)
