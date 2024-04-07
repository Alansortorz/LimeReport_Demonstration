# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\LimeReport_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\LimeReport_autogen.dir\\ParseCache.txt"
  "LimeReport_autogen"
  )
endif()
