cmake_minimum_required(VERSION 3.28)
project(alloy_alloc VERSION 0.1.0 LANGUAGES CXX)

option(ALLOY_ALLOC_BUILD_TESTS "Build unit tests" ${PROJECT_IS_TOP_LEVEL})
option(ALLOY_ALLOC_BUILD_BENCHMARKS "Build microbenchmarks" OFF)
option(ALLOY_ALLOC_BUILD_EXAMPLES "Build examples" ${PROJECT_IS_TOP_LEVEL})
option(ALLOY_ALLOC_BUILD_DOCS "Build API documentation target" OFF)

add_library(alloy_alloc_compile_options INTERFACE)

target_compile_features(
    alloy_alloc_compile_options INTERFACE cxx_std_20
)

if(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
    target_compile_options(alloy_alloc_compile_options INTERFACE
        /W4
    )
elseif(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
    target_compile_options(alloy_alloc_compile_options INTERFACE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

add_library(alloy_alloc STATIC src/arena.cpp)
add_library(alloy::alloc ALIAS alloy_alloc)
target_compile_features(alloy_alloc PUBLIC cxx_std_20)
set_target_properties(alloy_alloc PROPERTIES CXX_EXTENSIONS OFF CXX_SCAN_FOR_MODULES OFF)
target_include_directories(alloy_alloc PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
)
target_link_libraries(alloy_alloc PRIVATE alloy_alloc_compile_options)

if(ALLOY_ALLOC_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
if(ALLOY_ALLOC_BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()
if(ALLOY_ALLOC_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
if(ALLOY_ALLOC_BUILD_DOCS)
    add_subdirectory(docs)
endif()

if(PROJECT_IS_TOP_LEVEL AND CMAKE_EXPORT_COMPILE_COMMANDS)
    add_custom_target(copy_compile_commands ALL
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_BINARY_DIR}/compile_commands.json"
            "${PROJECT_SOURCE_DIR}/compile_commands.json"
        COMMENT "Updating compilation database"
        VERBATIM
    )
endif()