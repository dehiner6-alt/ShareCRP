cmake_minimum_required(VERSION 3.21)
project(ShareCRP CXX)

# Fuerza xzddddd
list(APPEND CMAKE_PREFIX_PATH $ENV{GEODE_SDK})

find_package(Geode REQUIRED)

geode_target_include_directories(${PROJECT_NAME} PRIVATE src)

geode_add_mod(${PROJECT_NAME}
    INCLUDES
        src/ShareCRPPopup.hpp
    SOURCES
        src/main.cpp
        src/ShareCRPPopup.cpp
)
