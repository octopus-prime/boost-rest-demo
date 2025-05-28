#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include <format>
#include <chrono>
#include <boost/describe.hpp>

struct driver {
    std::uint64_t driver_number;
    std::string first_name;
    std::string last_name;
    std::string team_name;
};

BOOST_DESCRIBE_STRUCT(driver, (), (driver_number, first_name, last_name, team_name))

template<typename Char>
struct std::formatter<driver, Char> : std::formatter<std::string_view, Char> {
    auto format(const driver& x, auto &ctx) const {
        return format_to(ctx.out(), "{} {} ({})", x.first_name, x.last_name, x.team_name);
    }
};

using drivers = std::vector<driver>;
