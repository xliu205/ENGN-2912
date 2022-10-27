PROJECT(Calc6A)
cmake_minimum_required(VERSION 3.3)

# list of source files
set(calc6A_files Calc6A.cpp AlgebraicTreeNode.hpp AlgebraicTreeNode.cpp AlgebraicTreeExpression.hpp AlgebraicTreeExpression.cpp)

# define the executable
if(WIN32)
  add_executable(calc6A WIN32 ${calc6A_files})
else()
  add_executable(calc6A ${calc6A_files})
endif()

# in Windows + Visual Studio we need this to make it a console application
if(WIN32)
  if(MSVC)
    set_target_properties(calc6A PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
  endif(MSVC)
endif(WIN32)

# install application
set(BIN_DIR ${CMAKE_INSTALL_PREFIX}/bin)

install(TARGETS calc6A DESTINATION ${BIN_DIR})
