#include "f1_client.hpp"

// chrono

// struct my_chrono_support{};

// std::chrono::year_month_day tag_invoke(
//     boost::json::value_to_tag<std::chrono::year_month_day>,
//     boost::json::value const& jv,
//     my_chrono_support)
// {
//     auto const& js = jv.as_string();
//     int year = 0, month = 0, day = 0;
//     if (3 != std::sscanf(js.c_str(), "%4d-%2d-%2d", &year, &month, &day))
//         throw std::invalid_argument("bad date");
//     return std::chrono::year{year} / month / day;
// }

// std::chrono::system_clock::time_point tag_invoke(
//     boost::json::value_to_tag<std::chrono::system_clock::time_point>,
//     boost::json::value const& jv,
//     my_chrono_support)
// {
//     auto const& js = jv.as_string();
//     int year, month, day, hour, minute, second;
//     if (6 != std::sscanf(js.c_str(), "%4d-%2d-%2dT%2d:%2d:%2d", &year, &month, &day, &hour, &minute, &second))
//         throw std::invalid_argument("bad date");
//     return std::chrono::sys_days(std::chrono::year{year} / month / day) + 
//            std::chrono::hours{hour} + 
//            std::chrono::minutes{minute} + 
//            std::chrono::seconds{second};
// }

#if (BOOST_VERSION < 108700)
// the voodoo magic to convert a value to a specific type and ignores unknown fields, until boost 1.87
template<class T> void extract( boost::json::value const & val, char const * name, T & value )
{
    // value = boost::json::value_to<T>( val, my_chrono_support{} );
    value = boost::json::value_to<T>( val );
}

template<class T,
    class D1 = boost::describe::describe_members<T,
        boost::describe::mod_public | boost::describe::mod_protected>,
    class D2 = boost::describe::describe_members<T, boost::describe::mod_private>,
    class En = std::enable_if_t<boost::mp11::mp_empty<D2>::value && !std::is_union<T>::value> >

    T tag_invoke( boost::json::value_to_tag<T> const&, boost::json::value const& v )
{
    auto const& obj = v.as_object();

    T t{};

    boost::mp11::mp_for_each<D1>([&](auto D){
        // if constexpr (std::__is_std_optional<std::remove_cvref_t<decltype(t.*D.pointer)>>::value) {
        //     if( auto ptr = obj.if_contains(D.name) )
        //         extract(*ptr, D.name, t.*D.pointer);          
        // } else {
            extract(obj.at(D.name), D.name, t.*D.pointer);
        // }
    });

    return t;
}
// -------
#endif

f1_client::f1_client() : client{"https://api.openf1.org", ssl::context::tlsv13_client} {
}

meetings f1_client::get_meetings(std::uint64_t year) const {
    boost::urls::static_url<100> url;//{client.base_url()};
    boost::urls::format_to(url, "/v1/meetings?year={}", year);
    // url.set_path("/v1/meetings");
    // url.set_params({{"year", year}});
    return value_to<meetings>(client.execute<json_body>(http::verb::get, url));//, my_chrono_support{});
    // return value_to<meetings>(client.execute<json_body>(http::verb::get, url, json::value_from(meeting{})));//, my_chrono_support{});
}

sessions f1_client::get_sessions(std::uint64_t meeting_key) const {
    boost::urls::static_url<100> url;//{client.base_url()};
    boost::urls::format_to(url, "/v1/sessions?meeting_key={}", meeting_key);
    // url.set_path("/v1/sessions");
    // url.set_params({{"meeting_key", "latest"}});
    return value_to<sessions>(client.execute<json_body>(http::verb::get, url));//, my_chrono_support{});
}

drivers f1_client::get_drivers(std::uint64_t session_key) const {
    boost::urls::static_url<100> url;//{client.base_url()};
    boost::urls::format_to(url, "/v1/drivers?session_key={}", session_key);
    // url.set_path("/v1/drivers");
    // url.set_params({{"meeting_key", "latest"}, {"session_key", "latest"}});
    return value_to<drivers>(client.execute<json_body>(http::verb::get, url));
}

positions f1_client::get_positions(std::uint64_t session_key) const {
    boost::urls::static_url<100> url;//{client.base_url()};
    boost::urls::format_to(url, "/v1/position?session_key={}", session_key);
    // url.set_path("/v1/positions");
    // url.set_params({{"meeting_key", "latest"}, {"session_key", "latest"}});
    return value_to<positions>(client.execute<json_body>(http::verb::get, url));//, my_chrono_support{});
}
