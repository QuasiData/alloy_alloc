# One explicit registration per example; additional sources may be passed.
function(alloy_alloc_example name)
    add_executable(alloy_alloc_example_${name} ${ARGN})
    target_link_libraries(alloy_alloc_example_${name} PRIVATE alloy::alloc alloy_alloc_compile_options)
    set_target_properties(alloy_alloc_example_${name} PROPERTIES CXX_EXTENSIONS OFF CXX_SCAN_FOR_MODULES OFF)
endfunction()

alloy_alloc_example(hello hello.cpp)
