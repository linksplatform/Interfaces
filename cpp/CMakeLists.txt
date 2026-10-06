cmake_minimum_required(VERSION 3.15)

set(LINKS_PLATFORM_TESTS OFF CACHE BOOL "Whether to compile tests")
set(LINKS_PLATFORM_EXTRA_FLAGS "" CACHE STRING "Extra compiler flags for the tests, e.g. -march=native")
set(LINKS_PLATFORM_SANITIZERS "" CACHE STRING "Sanitizers for the tests, e.g. address;undefined")

project(Platform.Interfaces CXX)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(${PROJECT_NAME}.Library INTERFACE)
target_include_directories(${PROJECT_NAME}.Library INTERFACE ${PROJECT_NAME})

if(${LINKS_PLATFORM_TESTS})
    find_package(GTest REQUIRED)
    enable_testing()
    include(GoogleTest)
    add_executable(${PROJECT_NAME}.Tests ${PROJECT_NAME}.Tests/${PROJECT_NAME}.Tests.cpp)
    set_target_properties(${PROJECT_NAME}.Tests PROPERTIES CXX_STANDARD 20)
    target_link_libraries(${PROJECT_NAME}.Tests PRIVATE GTest::gtest)
    target_link_libraries(${PROJECT_NAME}.Tests PRIVATE GTest::gtest_main)
    target_link_libraries(${PROJECT_NAME}.Tests PRIVATE ${PROJECT_NAME}.Library)

    separate_arguments(extra_flags NATIVE_COMMAND "${LINKS_PLATFORM_EXTRA_FLAGS}")
    target_compile_options(${PROJECT_NAME}.Tests PRIVATE ${extra_flags})
    if(LINKS_PLATFORM_SANITIZERS)
        list(JOIN LINKS_PLATFORM_SANITIZERS "," sanitizers)
        target_compile_options(${PROJECT_NAME}.Tests PRIVATE
            -fsanitize=${sanitizers} -fno-omit-frame-pointer -fno-sanitize-recover=all)
        target_link_options(${PROJECT_NAME}.Tests PRIVATE -fsanitize=${sanitizers})
    endif()

    gtest_discover_tests(${PROJECT_NAME}.Tests)
endif()
