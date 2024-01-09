cmake_minimum_required(VERSION 3.5.0)
project(Client VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 20)

add_executable(Client main.cpp)

target_include_directories(${PROJECT_NAME}
        PUBLIC
            Network
)

target_link_libraries(${PROJECT_NAME}
        PUBLIC
            Network
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
