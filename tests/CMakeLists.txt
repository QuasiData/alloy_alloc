include(FetchContent)

if(NOT TARGET GTest::gtest_main)
    set(INSTALL_GTEST OFF CACHE BOOL "Install GoogleTest")
    set(gtest_force_shared_crt ON CACHE BOOL "Use shared MSVC runtime")
    FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.16.0
    )
    FetchContent_MakeAvailable(googletest)
endif()

add_executable(alloy_alloc_tests test.cpp)
target_link_libraries(alloy_alloc_tests PRIVATE alloy::alloc alloy_alloc_compile_options GTest::gtest_main)
set_target_properties(alloy_alloc_tests PROPERTIES CXX_EXTENSIONS OFF CXX_SCAN_FOR_MODULES OFF)
include(GoogleTest)
gtest_discover_tests(alloy_alloc_tests TEST_PREFIX "alloy_alloc.")
