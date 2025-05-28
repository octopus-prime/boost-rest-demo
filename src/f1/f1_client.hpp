#pragma once

#include "../http/http_client.hpp"
#include "meeting.hpp"
#include "session.hpp"
#include "driver.hpp"
#include "position.hpp"

struct f1_client {
    f1_client();

    meetings get_meetings(std::uint64_t year) const;

    sessions get_sessions(std::uint64_t meeting_key) const;

    drivers get_drivers(std::uint64_t session_key) const;

    positions get_positions(std::uint64_t session_key) const;

private:
    http_client client;
};
