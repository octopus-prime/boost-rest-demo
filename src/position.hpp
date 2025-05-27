#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include <format>
#include <chrono>
#include <boost/describe.hpp>

struct position {
    std::uint64_t driver_number;
    std::uint64_t position;
    std::string date;
    // std::chrono::system_clock::time_point date; // "2025-03-14T01:30:00+00:00"
};

BOOST_DESCRIBE_STRUCT(position, (), (driver_number, position, date))

template<typename Char>
struct std::formatter<position, Char> : std::formatter<std::string_view, Char> {
    auto format(const position& x, auto &ctx) const {
        return format_to(ctx.out(), "{} {} {}", x.driver_number, x.position, x.date);
    }
};

using positions = std::vector<position>;
