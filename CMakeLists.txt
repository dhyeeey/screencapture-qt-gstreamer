cmake_minimum_required(VERSION 3.16)

project(FinalScreenCap VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(DEFINED ENV{GSTREAMER_1_0_ROOT_MSVC_X86_64})
    set(GSTREAMER_ROOT "$ENV{GSTREAMER_1_0_ROOT_MSVC_X86_64}")

    set(ENV{PKG_CONFIG_PATH} "${GSTREAMER_ROOT}/lib/pkgconfig")
else()
    message(FATAL_ERROR "GSTREAMER_1_0_ROOT_MSVC_X86_64 not set in environment.")
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets OpenGLWidgets Gui)
find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)

pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)
pkg_check_modules(GST_APP REQUIRED gstreamer-app-1.0)
pkg_check_modules(GST_VIDEO REQUIRED gstreamer-video-1.0)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
)

include_directories(
    ${GSTREAMER_INCLUDE_DIRS}
    ${GST_APP_INCLUDE_DIRS}
    ${GST_VIDEO_INCLUDE_DIRS}
)
link_directories(
    ${GSTREAMER_LIBRARY_DIRS}
    ${GST_APP_LIBRARY_DIRS}
    ${GST_VIDEO_LIBRARY_DIRS}
)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(FinalScreenCap
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        glvideowidget.h glvideowidget.cpp
        screencaptureprocessor.h screencaptureprocessor.cpp
    )
else()
    if(ANDROID)
        add_library(FinalScreenCap SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(FinalScreenCap
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(FinalScreenCap PRIVATE
                                      Qt${QT_VERSION_MAJOR}::Widgets
                                      ${GSTREAMER_LIBRARIES}
                                      ${GST_APP_LIBRARIES}
                                      ${GST_VIDEO_LIBRARIES}
                                      Qt${QT_VERSION_MAJOR}::OpenGLWidgets
                                      Qt${QT_VERSION_MAJOR}::Gui
                                      Qt${QT_VERSION_MAJOR}::OpenGL
                                      OpenGL::GL)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.FinalScreenCap)
endif()
set_target_properties(FinalScreenCap PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS FinalScreenCap
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(FinalScreenCap)
endif()
