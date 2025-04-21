cmake_minimum_required(VERSION 3.30)
project(daedalus)

set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_INCLUDES OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(daedalus)
add_subdirectory(include)
add_subdirectory(src)

include(AtlasModules.cmake)
add_subdirectory(atlas)
target_link_libraries(daedalus PRIVATE atlas)

if(ENABLE_INSTALL)
  install(TARGETS daedalus DESTINATION bin)
endif()
