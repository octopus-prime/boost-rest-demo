#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include <format>
#include <chrono>
#include <boost/describe.hpp>

struct meeting {
    std::uint64_t meeting_key;
    std::string meeting_name;
};

BOOST_DESCRIBE_STRUCT(meeting, (), (meeting_key, meeting_name))

template<typename Char>
struct std::formatter<meeting, Char> : std::formatter<std::string_view, Char> {
    auto format(const meeting& x, auto &ctx) const {
        return format_to(ctx.out(), "{} {}", x.meeting_key, x.meeting_name);
    }
};

using meetings = std::vector<meeting>;
