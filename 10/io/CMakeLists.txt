set(NAME io)

set(HEADERS
  LoaderFactory.hpp
  SaverFactory.hpp
  StrException.hpp
  Loader.hpp
  LoaderWrl.hpp
  SaverWrl.hpp
  Tokenizer.hpp
) # HEADERS    

set(SOURCES
  LoaderFactory.cpp
  SaverFactory.cpp
  LoaderWrl.cpp
  SaverWrl.cpp
  Tokenizer.cpp
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
