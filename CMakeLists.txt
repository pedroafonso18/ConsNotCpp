cmake_minimum_required(VERSION 3.31)
project(ConsNot)

set(CMAKE_CXX_STANDARD 11)

add_executable(ConsNot src/main.cpp
        include/Config.h
        lib/dotenv.h
        src/Config.cpp
        include/Models.h
        include/Requests.h
        src/Requests.cpp
        lib/json.h)
