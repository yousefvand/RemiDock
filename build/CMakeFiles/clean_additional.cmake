# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles/RemiDock_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/RemiDock_autogen.dir/ParseCache.txt"
  "RemiDock_autogen"
  )
endif()
