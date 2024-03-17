cmake_minimum_required(VERSION 3.27)
project(forklab)

set(CMAKE_CXX_STANDARD 23)
include_directories(include lib src)
find_package(OpenSSL REQUIRED)
if (OPENSSL_FOUND)
    set(HTTPLIB_IS_USING_OPENSSL TRUE)
endif ()


file(GLOB all_SRCS
        "src/*.cpp"
        "src/*.c"
)

add_executable(forklab main.cpp ${all_SRCS})
target_link_libraries(${PROJECT_NAME} PUBLIC
        $<$<BOOL:${HTTPLIB_IS_USING_OPENSSL}>:OpenSSL::SSL>
        $<$<BOOL:${HTTPLIB_IS_USING_OPENSSL}>:OpenSSL::Crypto>)
target_compile_definitions(${PROJECT_NAME} PUBLIC
        $<$<BOOL:${HTTPLIB_IS_USING_OPENSSL}>:CPPHTTPLIB_OPENSSL_SUPPORT>
)