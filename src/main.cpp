#include <iostream>
#include <ranges>
#include <map>
#include "f1_client.hpp"

int main() {
    constexpr auto not_a_testing_meeting = [](const meeting& m) {
        return !m.meeting_name.contains("Testing");
    };

    try {
        const f1_client client;

        std::print("\033[2J\033[H");
        std::print("Year: ");
        std::uint64_t year;
        std::cin >> year;

        auto meetings = client.get_meetings(year);
        std::ranges::sort(meetings, {}, &meeting::meeting_key);

        std::print("\033[2J\033[H");
        std::println("Meetings for year {}:", year);
        std::println("");
        std::println("Key  Name");
        std::println("----------------------------------------------------");
        for (const auto& meeting : meetings | std::views::filter(not_a_testing_meeting)) {
            std::println("{}", meeting);
        }
        std::println("");

        std::print("Key: ");
        std::uint64_t meeting_key;
        std::cin >> meeting_key;

        auto meeting = std::ranges::find(meetings, meeting_key, &meeting::meeting_key);

        auto sessions = client.get_sessions(meeting_key);
        std::ranges::sort(sessions, {}, &session::session_key);
        std::print("\033[2J\033[H");
        std::println("Results for meeting {} in year {}:", meeting->meeting_name, year);
        std::println("");
        std::println("Key  Name");
        std::println("----------------------------------------------------");
        for (const auto& session : sessions) {
            std::println("{}", session);
        }
        std::println("");

        std::print("Key: ");
        std::uint64_t session_key;
        std::cin >> session_key;

        auto session = std::ranges::find(sessions, session_key, &session::session_key);

        auto drivers = client.get_drivers(session_key);
        auto positions = client.get_positions(session_key);

        std::map<std::uint64_t, driver> driver_map;
        for (const auto& driver : drivers) {
            auto foo = positions | std::views::filter([&driver](const auto& pos) {
                return pos.driver_number == driver.driver_number;
            });
            auto bar = std::ranges::max_element(foo, {}, &position::date);
            driver_map[bar->position] = driver;
        }

        std::print("\033[2J\033[H");
        std::println("Drivers for session {} in meeting {} in year {}:", session->session_name, meeting->meeting_name, year);
        std::println("");
        std::println("#  Driver");
        std::println("----------------------------------------------------");
        for (const auto& [position, driver] : driver_map) {
            std::println("{:2} {}", position, driver);
        }

    } catch (const std::exception& e) {
        std::cerr << "Error retrieving drivers: " << e.what() << "\n";
    }
    return 0;
}
