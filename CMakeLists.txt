cmake_minimum_required(VERSION 3.10)
project(goquant_assessment)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Find CURL and OpenSSL
find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)

find_package(cpprestsdk REQUIRED)
add_subdirectory(third_party/cpprestsdk)
find_package(Boost REQUIRED)
# Add include directories
include_directories(/usr/include)
include_directories(third_party/json/single_include)
include_directories(include)
include_directories(third_party/nlohmann)
include_directories(third_party/cpprestsdk)  # Add include directory for headers
include_directories(third_party/cpprestsdk/Release/include/cpprest)
include_directories(third_party/cpprestsdk/Release/include)
include_directories(third_party/cpprestsdk/Release)
set(SOURCES
    src/main.cpp
)

# Link uWebSockets and dependencies
find_package(Boost REQUIRED COMPONENTS system thread)
find_package(OpenSSL REQUIRED)


# Add your executable
add_executable(goquant_assessment 
    src/main.cpp 
    src/mexc_api.cpp
    src/websocket_client.cpp
)



target_link_libraries(goquant_assessment 
    PRIVATE 
    CURL::libcurl 
    OpenSSL::SSL 
    OpenSSL::Crypto
    cpprestsdk::cpprest
    Boost::boost Boost::system Boost::thread
    )

# Specify include directories for the target
target_include_directories(goquant_assessment PRIVATE include third_party third_party/json third_party/cpprestsdk)


add_custom_command(
    TARGET goquant_assessment POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    config ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config
    COMMENT "Copying config directory"
    VERBATIM
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    DEPENDS ${CMAKE_SOURCE_DIR}/config
)
