set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_library(ModuleGui STATIC EXCLUDE_FROM_ALL
   gui-main.cpp
   table-node.cpp)

target_link_libraries(ModuleGui PUBLIC ModuleNet Qt6::Widgets)

target_include_directories(ModuleGui PUBLIC 
   ${CMAKE_SOURCE_DIR}/src
   ${CMAKE_SOURCE_DIR}/libs/
   ${CMAKE_SOURCE_DIR}/libs/spdlog)
