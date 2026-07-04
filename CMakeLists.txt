cmake_minimum_required(VERSION 3.16)

project(PhotoAlbum VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Multimedia)



set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
	prosetpage.h
	prosetpage.cpp
	prosetpage.ui
	confirmpage.h
	confirmpage.cpp
	confirmpage.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(PhotoAlbum
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
	rc.qrc
	wizard.h wizard.cpp wizard.ui
	prosetpage.h prosetpage.cpp prosetpage.ui
	confirmpage.h confirmpage.cpp confirmpage.ui
	protree.h protree.cpp protree.ui
	protreewidget.h protreewidget.cpp
	protreeitem.h protreeitem.cpp
	const.h
	protreethread.h protreethread.cpp
	removeprodialog.h removeprodialog.cpp removeprodialog.ui
	opentreethread.h opentreethread.cpp
	picshow.h picshow.cpp picshow.ui
	picbutton.h picbutton.cpp
	slideshowdialog.h slideshowdialog.cpp slideshowdialog.ui
	picstatebtn.h picstatebtn.cpp
	picanimationwidget.h picanimationwidget.cpp
	prelistwidget.h prelistwidget.cpp
	prelistitem.h prelistitem.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET PhotoAlbum APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(PhotoAlbum SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(PhotoAlbum
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_include_directories(PhotoAlbum PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(PhotoAlbum PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(PhotoAlbum PRIVATE Qt6::Widgets)
target_link_libraries(PhotoAlbum PRIVATE Qt6::Multimedia)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.PhotoAlbum)
endif()
set_target_properties(PhotoAlbum PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS PhotoAlbum
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(PhotoAlbum)
endif()
