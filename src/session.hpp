#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include <format>
#include <chrono>
#include <boost/describe.hpp>

struct session {
    std::uint64_t session_key;
    std::string session_name;
};

BOOST_DESCRIBE_STRUCT(session, (), (session_key, session_name))

template<typename Char>
struct std::formatter<session, Char> : std::formatter<std::string_view, Char> {
    auto format(const session& x, auto &ctx) const {
        return format_to(ctx.out(), "{} {}", x.session_key, x.session_name);
    }
};

using sessions = std::vector<session>;
