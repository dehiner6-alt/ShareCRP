cmake_minimum_required(VERSION 3.21)
project(ShareCRP CXX)

find_package(Geode REQUIRED)

geode_target_include_directories(${PROJECT_NAME} PRIVATE src)

geode_add_mod(${PROJECT_NAME}
    INCLUDES
        src/ShareCRPPopup.hpp
    SOURCES
        src/main.cpp
        src/ShareCRPPopup.cpp
)
