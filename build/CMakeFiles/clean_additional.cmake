# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "src/gui/CMakeFiles/ModuleGui_autogen.dir/AutogenUsed.txt"
  "src/gui/CMakeFiles/ModuleGui_autogen.dir/ParseCache.txt"
  "src/gui/ModuleGui_autogen"
  "src/net/CMakeFiles/ModuleNet_autogen.dir/AutogenUsed.txt"
  "src/net/CMakeFiles/ModuleNet_autogen.dir/ParseCache.txt"
  "src/net/ModuleNet_autogen"
  )
endif()
