# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\apprr-launcher_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\apprr-launcher_autogen.dir\\ParseCache.txt"
  "apprr-launcher_autogen"
  )
endif()
