#include <boost/json.hpp>
#include <boost/describe.hpp>

struct test {
    int a;
    int b;
};

BOOST_DESCRIBE_STRUCT(test, (), (a, b))

int main() {
    namespace json = boost::json;
    json::value_from(test{});
}