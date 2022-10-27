set(NAME ifs)

set(HEADERS
  Ifs.hpp
  IfsLoader.hpp
  IfsSaver.hpp
  IfsLoaderFactory.hpp
  IfsSaverFactory.hpp
  IfsLoaderWrl.hpp
  IfsSaverWrl.hpp
  IfsLoaderOpt.hpp
) # HEADERS    

set(SOURCES
  Ifs.cpp
  IfsLoaderFactory.cpp
  IfsSaverFactory.cpp
  IfsLoaderWrl.cpp
  IfsSaverWrl.cpp
  IfsLoaderOpt.cpp
) # SOURCES

qt5_wrap_ui(FORMS
) # FORMS

qt5_add_resources(ASSETS
) # ASSETS

add_library(${NAME}
    ${HEADERS}
    ${SOURCES}
    ${FORMS}
    ${ASSETS}
) # add_library

target_compile_features(${NAME} PRIVATE cxx_lambdas)

target_link_libraries(${NAME} ${LIB_LIST})

