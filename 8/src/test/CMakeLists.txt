PROJECT(ifsTest)
cmake_minimum_required(VERSION 3.3)

# list of source files
set(ifsTest_files ifstest.cpp)

# define the executable
# if(WIN32)
  add_executable(ifsTest WIN32 ${ifsTest_files})
# else()
#   add_executable(calc1 ${ifsTest_files})
# endif()

# in Windows + Visual Studio we need this to make it a console application
if(WIN32)
  if(MSVC)
    set_target_properties(ifsTest PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
  endif(MSVC)
endif(WIN32)

# install application
set(BIN_DIR ${CMAKE_INSTALL_PREFIX}/bin)

target_link_libraries(ifsTest util ifs)

install(TARGETS ifsTest DESTINATION ${BIN_DIR})

