project(capd_backend)

set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

add_executable(${PROJECT_NAME}
    main.cpp
    
    carina/capd/map.cpp
    carina/capd/ode_solver.cpp
    carina/capd/timemap.cpp)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)

add_dependencies(${PROJECT_NAME} capd2)

add_subdirectory(CAPD)
target_compile_options(capd2 PRIVATE -Wall -Wextra)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE -lstdc++)
target_link_libraries(${PROJECT_NAME} PRIVATE -lm)

link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/CAPD)
target_link_libraries(${PROJECT_NAME} PRIVATE capd2)
