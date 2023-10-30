cmake_minimum_required(VERSION 3.12)
project(mpt_project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

file(GLOB_RECURSE SOURCES_LIST src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES_LIST})

add_subdirectory(deps)

target_link_libraries(${PROJECT_NAME} PRIVATE -lpthread)
target_link_libraries(${PROJECT_NAME} PRIVATE -lm)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)


