set(NAME IfsViewer2021)

set(HEADERS
  IfsAboutDialog.hpp
  IfsMainWindow.hpp
  IfsGLWidget.hpp
  IfsGLBuffer.hpp
  IfsGLShader.hpp
  IfsGLHandles.hpp
  IfsViewerData.hpp
  QtLogo.hpp
) # HEADERS    

set(SOURCES
  IfsAboutDialog.cpp
  IfsViewerApp.cpp
  IfsMainWindow.cpp
  IfsGLWidget.cpp
  IfsGLBuffer.cpp
  IfsGLShader.cpp
  IfsGLHandles.cpp
  IfsViewerData.cpp
  QtLogo.cpp
) # SOURCES

qt5_wrap_ui(FORMS
) # FORMS

qt5_add_resources(ASSETS
) # ASSETS

if (APPLE)
  set(APP_ICON ${ASSETS_DIR}/IfsViewer.icns)
  set_source_files_properties(${APP_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
  add_executable(${NAME} MACOSX_BUNDLE ${APP_ICON}
      ${HEADERS}
      ${SOURCES}
      ${FORMS}
      ${ASSETS}
  ) # add_executable
  set(MACOSX_BUNDLE_ICON_FILE "IfsViewer.icns" )
else(APPLE)
  add_executable(${NAME}
      ${HEADERS}
      ${SOURCES}
      ${FORMS}
      ${ASSETS}
  ) # add_executable
endif(APPLE)

target_compile_features(${NAME} PRIVATE cxx_right_angle_brackets cxx_lambdas)

target_link_libraries(${NAME} ${LIB_LIST})

install(TARGETS ${NAME} DESTINATION ${BIN_DIR})
