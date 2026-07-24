cmake_minimum_required(VERSION 3.20)

project(guard9 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(PkgConfig REQUIRED)
pkg_check_modules(NFQUEUE REQUIRED libnetfilter_queue)

add_executable(guard9
    src/main.cpp
    src/tls.cpp
    src/tcp.cpp
    src/ip.cpp
)

target_include_directories(guard9 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${NFQUEUE_INCLUDE_DIRS}
)

target_precompile_headers(guard9 PRIVATE
    include/pch.h
)

target_link_libraries(guard9 PRIVATE
    ${NFQUEUE_LIBRARIES}
)

target_compile_options(guard9 PRIVATE
    ${NFQUEUE_CFLAGS_OTHER}
)
