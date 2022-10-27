PROJECT(Calc6B)
cmake_minimum_required(VERSION 3.3)

# list of source files
set(calc6B_files
AlgebraicTreeExpression.cpp
AlgebraicTreeExpression.hpp
AlgebraicTreeNode.cpp
AlgebraicTreeNode.hpp
AlgebraicTreeNumber.cpp
AlgebraicTreeNumber.hpp
AlgebraicTreeOperation.cpp
AlgebraicTreeOperation.hpp
CMakeLists.txt
Calc6B.cpp
)

# define the executable
if(WIN32)
  add_executable(calc6B WIN32 ${calc6B_files})
else()
  add_executable(calc6B ${calc6B_files})
endif()

# in Windows + Visual Studio we need this to make it a console application
if(WIN32)
  if(MSVC)
    set_target_properties(calc6B PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
  endif(MSVC)
endif(WIN32)

# install application
set(BIN_DIR ${CMAKE_INSTALL_PREFIX}/bin)

install(TARGETS calc6B DESTINATION ${BIN_DIR})

