cmake_minimum_required(VERSION 3.5)

project(FontGenerator-2-0 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/application.qrc")


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(FontGenerator-2-0
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${app_icon_resource_windows}

        font.h font.cpp
        fontslistmodel.h fontslistmodel.cpp
        fontpixelmap.h fontpixelmap.cpp
        utils.h utils.cpp
        fontgenerator.h fontgenerator.cpp
        ledmatrixfontgenerator.h ledmatrixfontgenerator.cpp
        tftfontgenerator.h tftfontgenerator.cpp
        application.qrc
        icons/center.png icons/down.png icons/left.png icons/right.png icons/up.png
        syntaxhighlighter.h syntaxhighlighter.cpp
        codepreview.h codepreview.cpp codepreview.ui

    )
    qt_add_resources(FontGenerator-2-0 "app_icons"
    PREFIX "/"
    FILES
        icons/center.png
        icons/up.png
        icons/down.png
        icons/left.png
        icons/right.png
        icons/underline.png
        icons/bold.png
        icons/italic.png
        icons/select_all.png
        icons/deselect.png
        icons/delete_all.png
        icons/save.png
        icons/copy.png
        icons/app_ico.png
        icons/app_ico.ico
    )

# qt_add_executable(FontGenerator20 main.cpp ${app_icon_resource_windows})

# Define target properties for Android with Qt 6 as:
#    set_property(TARGET FontGenerator-2-0 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(FontGenerator-2-0 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(FontGenerator-2-0
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(FontGenerator-2-0 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.FontGenerator-2-0)
endif()
set_target_properties(FontGenerator-2-0 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS FontGenerator-2-0
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(FontGenerator-2-0)
endif()
