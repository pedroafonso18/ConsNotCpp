cmake_minimum_required(VERSION 3.31)
project(ConsNot)

set(CMAKE_CXX_STANDARD 11)

set(MSYS_ROOT "C:/msys64/mingw64")
set(CMAKE_PREFIX_PATH "${MSYS_ROOT}/lib/cmake")

set(OPENSSL_ROOT_DIR "${MSYS_ROOT}")
set(CURL_ROOT_DIR "${MSYS_ROOT}")
set(ZLIB_ROOT "${MSYS_ROOT}")
set(ZLIB_INCLUDE_DIR "${ZLIB_ROOT}/include")
set(ZLIB_LIBRARY "${ZLIB_ROOT}/lib/libz.dll.a")

find_package(fmt REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(CURL REQUIRED)
find_package(ZLIB REQUIRED)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CURL_INCLUDE_DIRS}
        ${OPENSSL_INCLUDE_DIR}
        ${ZLIB_INCLUDE_DIR}
)

add_executable(ConsNot
        src/main.cpp
        src/Config.cpp
        src/Requests.cpp
        include/Config.h
        include/Models.h
        include/Requests.h
        include/Utils.h
        include/Database.h
        lib/dotenv.h
        lib/json.h
)

target_link_libraries(ConsNot
        PRIVATE
        ${CURL_LIBRARIES}
        ${OPENSSL_LIBRARIES}
        ${ZLIB_LIBRARY}
        fmt::fmt
)
