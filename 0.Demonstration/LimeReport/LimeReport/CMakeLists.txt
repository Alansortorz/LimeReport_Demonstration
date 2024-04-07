cmake_minimum_required(VERSION 3.14)

project(LimeReport VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick)

# Find the required Qt5 components
find_package(Qt5 COMPONENTS Core Gui PrintSupport Qml REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

set(PROJECT_SOURCES
        limereport.cpp limereport.h
        main.cpp
        qml.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(LimeReport
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET LimeReport APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(LimeReport SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(LimeReport
          ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(LimeReport
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick)

# Add Qt5 modules to target link libraries
target_link_libraries(LimeReport PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::PrintSupport
    Qt5::Qml
)

if(WIN32)
    target_link_libraries(LimeReport PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/limereportd.lib)
endif()

set_target_properties(LimeReport PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS LimeReport
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(LimeReport)
    qt_finalize_executable(LimeReport)
endif()
