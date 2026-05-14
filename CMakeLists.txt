cmake_minimum_required(VERSION 3.16)
project(wma)

find_package(OpenCV REQUIRED)

add_executable(main src/main.cpp)
target_link_libraries(main ${OpenCV_LIBS})
target_compile_definitions(
    main PRIVATE TEMPLATE_DIR=\"${PROJECT_SOURCE_DIR}/templates\"
)