set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Network)

add_library(ModuleNet STATIC EXCLUDE_FROM_ALL 
   server.cpp
   client.cpp)

target_include_directories(ModuleNet PUBLIC 
   ${CMAKE_SOURCE_DIR}/src
   ${CMAKE_SOURCE_DIR}/libs/spdlog 
   ${CMAKE_SOURCE_DIR}/libs/)

target_link_libraries(ModuleNet PUBLIC Qt6::Network)

if (WIN32)
   target_link_libraries(ModuleNet PUBLIC ws2_32 mswsock)
endif()
