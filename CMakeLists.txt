cmake_minimum_required(VERSION 3.31)
project(ConsNot)

set(CMAKE_CXX_STANDARD 17)

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
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/lib
        ${CURL_INCLUDE_DIRS}
        ${OPENSSL_INCLUDE_DIR}
        ${ZLIB_INCLUDE_DIR}
        ${MSYS_ROOT}/include
)

add_executable(ConsNot
        src/main.cpp
        src/Config.cpp
        src/Requests.cpp
        src/D_Insert.cpp
        src/D_Fetch.cpp
        include/Config.h
        include/Models.h
        include/Requests.h
        include/Utils.h
        include/Database.h
        include/D_Insert.h
        include/D_Fetch.h
        lib/dotenv.h
        lib/json.h
        include/Process.h
        src/Process.cpp
)

target_link_libraries(ConsNot
        PRIVATE
        ${CURL_LIBRARIES}
        ${OPENSSL_LIBRARIES}
        ${ZLIB_LIBRARY}
        fmt::fmt
        pqxx
        pq
)

add_executable(ConsNotTests
        lib/catch_amalgamated.cpp
        tests/CatchMain.cpp
        tests/Utils.cpp
)

target_include_directories(ConsNotTests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/lib
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

target_link_libraries(ConsNotTests
        PRIVATE
        ${CURL_LIBRARIES}
        ${OPENSSL_LIBRARIES}
        ${ZLIB_LIBRARY}
        fmt::fmt
        pqxx
        pq
)

enable_testing()
add_test(NAME ConsNotTests COMMAND ConsNotTests)
