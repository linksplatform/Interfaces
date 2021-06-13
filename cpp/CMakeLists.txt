cmake_minimum_required(VERSION 3.15)

set(CMAKE_LOCAL_PROJECT_NAME Platform.Interfaces)
project(${CMAKE_LOCAL_PROJECT_NAME})

add_subdirectory(${CMAKE_LOCAL_PROJECT_NAME})

get_directory_property(IS_SUBPROJECT PARENT_DIRECTORY)

macro(include_linksplatform_extension EXTENSION_NAME)
    add_subdirectory(${CMAKE_LOCAL_PROJECT_NAME}.${EXTENSION_NAME})
    target_link_libraries(${CMAKE_LOCAL_PROJECT_NAME}.${EXTENSION_NAME} PUBLIC ${CMAKE_LOCAL_PROJECT_NAME}.Library)
endmacro()

include_linksplatform_extension(Tests)

