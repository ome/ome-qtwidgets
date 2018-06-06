# #%L
# OME QtWidgets libraries (cmake build infrastructure)
# %%
# Copyright © 2006 - 2015 Open Microscopy Environment:
#   - Massachusetts Institute of Technology
#   - National Institutes of Health
#   - University of Dundee
#   - Board of Regents of the University of Wisconsin-Madison
#   - Glencoe Software, Inc.
# %%
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
# The views and conclusions contained in the software and documentation are
# those of the authors and should not be interpreted as representing official
# policies, either expressed or implied, of any organization.
# #L%

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config-internal.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/config-internal.h @ONLY)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(QTWIDGETS_SOURCES
    GLContainer.cpp
    GLWindow.cpp
    GLView2D.cpp
    module.cpp
    NavigationDock2D.cpp
    TexelProperties.cpp)

set(QTWIDGETS_HEADERS
    GLContainer.h
    GLWindow.h
    GLView2D.h
    glm.h
    module.h
    NavigationDock2D.h
    TexelProperties.h)

set(OME_QTWIDGETS_GENERATED_PRIVATE_HEADERS
    ${CMAKE_CURRENT_BINARY_DIR}/config-internal.h)

set(QTWIDGETS_GL_SOURCES
    gl/Axis2D.cpp
    gl/Grid2D.cpp
    gl/Image2D.cpp
    gl/Util.cpp)

set(QTWIDGETS_GL_HEADERS
    gl/Axis2D.h
    gl/Grid2D.h
    gl/Image2D.h
    gl/Util.h)

set(QTWIDGETS_GL_V33_SOURCES
    gl/v33/V33Axis2D.cpp
    gl/v33/V33Grid2D.cpp
    gl/v33/V33Image2D.cpp)

set(QTWIDGETS_GL_V33_HEADERS
    gl/v33/V33Axis2D.h
    gl/v33/V33Grid2D.h
    gl/v33/V33Image2D.h)

set(QTWIDGETS_GLSL_V330_SOURCES
    glsl/v330/V330GLFlatShader2D.cpp
    glsl/v330/V330GLImageShader2D.cpp
    glsl/v330/V330GLLineShader2D.cpp)

set(QTWIDGETS_GLSL_V330_HEADERS
    glsl/v330/V330GLFlatShader2D.h
    glsl/v330/V330GLImageShader2D.h
    glsl/v330/V330GLLineShader2D.h)

add_library(ome-qtwidgets
            ${QTWIDGETS_SOURCES}
            ${QTWIDGETS_HEADERS}
            ${OME_QTWIDGETS_GENERATED_PRIVATE_HEADERS}
            ${QTWIDGETS_GL_SOURCES}
            ${QTWIDGETS_GL_HEADERS}
            ${QTWIDGETS_GL_V33_SOURCES}
            ${QTWIDGETS_GL_V33_HEADERS}
            ${QTWIDGETS_GLSL_V330_SOURCES}
            ${QTWIDGETS_GLSL_V330_HEADERS}
            ${ome-qtwidgets_HEADERS_MOC}
            ${ome-qtwidgets_RESOURCES})

target_include_directories(ome-qtwidgets PUBLIC
                           $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/lib>
                           $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/lib>
                           $<BUILD_INTERFACE:${OPENGL_INCLUDE_DIR}>
                           $<BUILD_INTERFACE:${GLM_INCLUDE_DIR}>)

target_link_libraries(ome-qtwidgets OME::Files
                      Boost::filesystem
                      Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Svg
                      ${OPENGL_gl_LIBRARY} ${TIFF_LIBRARIES})

set_target_properties(ome-qtwidgets PROPERTIES VERSION ${ome-qtwidgets_VERSION})

add_library(OME::QtWidgets ALIAS ome-qtwidgets)

if(WIN32)
  set(ome_qtwidgets_config_dir "cmake")
else()
  set(ome_qtwidgets_config_dir "${CMAKE_INSTALL_LIBDIR}/cmake/OMEQtWidgets")
endif()

install(TARGETS ome-qtwidgets
        EXPORT OMEQtWidgetsInternal
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT "runtime"
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
install(EXPORT OMEQtWidgetsInternal
        DESTINATION "${ome_qtwidgets_config_dir}"
        NAMESPACE "ome_qtwidgets_"
        COMPONENT "development")

include(CMakePackageConfigHelpers)
configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/OMEQtWidgetsConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/OMEQtWidgetsConfig.cmake"
  INSTALL_DESTINATION "${ome_qtwidgets_config_dir}")
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/OMEQtWidgetsConfigVersion.cmake
  VERSION "${PROJECT_VERSION}"
  COMPATIBILITY SameMajorVersion)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/OMEQtWidgetsConfig.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/OMEQtWidgetsConfigVersion.cmake
        DESTINATION "${ome_qtwidgets_config_dir}"
        COMPONENT "development")

set(ome_qtwidgets_includedir "${CMAKE_INSTALL_INCLUDEDIR}/ome/qtwidgets")

install(FILES ${OME_QTWIDGETS_HEADERS}
        DESTINATION ${ome_qtwidgets_includedir}
        COMPONENT "development")
install(FILES ${OME_QTWIDGETS_GL__HEADERS}
        DESTINATION ${ome_qtwidgets_includedir}/gl
        COMPONENT "development")
install(FILES ${OME_QTWIDGETS_GL_V33_HEADERS}
        DESTINATION ${ome_qtwidgets_includedir}/gl/v33
        COMPONENT "development")
install(FILES ${OME_QTWIDGETS_GLSL_V330_HEADERS}
        DESTINATION ${ome_qtwidgets_includedir}/glsl/v330
        COMPONENT "development")

# Dump header list for testing
header_include_list_write(QTWIDGETS_HEADERS "" ome/qtwidgets ${PROJECT_BINARY_DIR}/test/ome-qtwidgets)
