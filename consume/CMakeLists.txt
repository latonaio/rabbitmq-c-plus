cmake_minimum_required(VERSION 3.26)
project(json_parser_c_plus)

set(CMAKE_CXX_STANDARD 14)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

add_executable(json_parser_c_plus
        main.cpp)

target_link_libraries(json_parser_c_plus PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(json_parser_c_plus PRIVATE SimpleAmqpClient rabbitmq boost_chrono boost_system rt ssl crypto dl pthread)
