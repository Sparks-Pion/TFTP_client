# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "MinSizeRel")
  file(REMOVE_RECURSE
  "CMakeFiles\\TFTP_Client_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\TFTP_Client_autogen.dir\\ParseCache.txt"
  "TFTP_Client_autogen"
  )
endif()
