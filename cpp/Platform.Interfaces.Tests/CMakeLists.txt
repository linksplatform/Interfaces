include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()


add_executable(${CMAKE_LOCAL_PROJECT_NAME}.Tests ${CMAKE_LOCAL_PROJECT_NAME}.Tests.cpp)
target_compile_features(${CMAKE_LOCAL_PROJECT_NAME}.Tests PUBLIC cxx_std_20)
target_link_libraries(${CMAKE_LOCAL_PROJECT_NAME}.Tests PUBLIC gtest gtest_main)
