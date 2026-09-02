cmake_minimum_required(VERSION 3.20)

project(qt-secl VERSION 1.0)

#
# options
#
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD            23)
set(CMAKE_CXX_STANDARD_REQUIRED   ON)

if(BUILD_TYPE)
   set(BUILD_TYPE "${BUILD_TYPE}" 
      CACHE STRING "DEBUG, RELEASE, SAN, TSAN, PROFILE" FORCE)
else()
   set(BUILD_TYPE "DEBUG" 
      CACHE STRING "DEBUG, RELEASE, SAN, TSAN, PROFILE")
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
   # common options
   add_compile_options(-g
                       -Wall
                       -Wextra
                       -Wshadow
                       -Wpedantic
                       -Wdouble-promotion
                       -Wunused-parameter
                       -Wno-unused-private-field
                       -fno-omit-frame-pointer)
   # release
   if (BUILD_TYPE STREQUAL "RELEASE")
      add_compile_options(-O2
                          -flto=auto
                          -march=native
                          -fomit-frame-pointer)
      add_link_options(-flto=auto -fuse-ld=lld)
   # profiler
   elseif (BUILD_TYPE STREQUAL "PROFILE")
      add_compile_options(-O3
                          -flto=auto
                          -march=native)
      add_link_options(-flto=auto -fuse-ld=lld)
   # sanitizers build
   # using: ASan, UBSan, UAFSan
   elseif (BUILD_TYPE STREQUAL "SAN")
      add_compile_options(-O1
                          -fsanitize=address,undefined
                          -fsanitize-address-use-after-return=always
                          -D_GLIBCXX_DEBUG)
      add_link_options(-fsanitize=address,undefined)
   # sanitizer build for threads (TSan)
   elseif (BUILD_TYPE STREQUAL "TSAN")
      add_compile_options(-O1
                          -fsanitize=thread
                          -D_GLIBCXX_DEBUG)
      add_link_options(-fsanitize=thread)
   # debug
   else()
      add_compile_options(-O0)
   endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
   # TODO: MSVC options
   # ...
endif()

#
# modules
#
add_subdirectory(src/gui)
add_subdirectory(src/net)

message("=======================================")
message("BUILD_TYPE........: ${BUILD_TYPE}")
message("=======================================")

#
# executables
#
add_executable(server src/server-main.cpp)
target_link_libraries(server PRIVATE 
   ModuleNet
   ModuleGui)

add_executable(client src/client-main.cpp)
target_link_libraries(client PRIVATE 
   ModuleNet)
