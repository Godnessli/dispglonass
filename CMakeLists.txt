cmake_minimum_required(VERSION 3.5)

project(dispglonass VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Boost_COMPILER -mgw11)
set(Boost_ARCHITECTURE -x64)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

set(Boost_DEBUG ON)

if(WIN32)
    find_package(Xlnt)
    set(BOOST_ROOT A:/libraries/boost_1_86_0)

    find_package(PythonLibs)
    find_package(Boost 1.86.0 REQUIRED COMPONENTS python312)

    include_directories(${Boost_INCLUDE_DIRS})
    include_directories(${PYTHON_INCLUDE_DIRS})
    include_directories("A:/libraries/utf8/source")
endif(WIN32)

include_directories(modules)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        core/mainmenu.cpp
        core/mainmenu.h
        core/mainmenu.ui
        modules/makeact.h
        modules/makeact.cpp
        modules/makeact.ui
        modules/luninareport.h
        modules/luninareport.cpp
        modules/luninareport.ui
        modules/settings.h
        modules/settings.cpp
        modules/settings.ui
        modules/getacts.h
        modules/getacts.cpp
        python_scripts/excel_reader.py
        python_scripts/google_api_user.py
        python_scripts/main.py
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(dispglonass
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET dispglonass APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(dispglonass SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(dispglonass
            ${PROJECT_SOURCES}
        )
    endif()
endif()

if(WIN32)
    target_compile_definitions(dispglonass PRIVATE QT_NO_KEYWORDS)
    target_link_libraries(dispglonass PRIVATE xlnt::xlnt)
    target_link_libraries(dispglonass PRIVATE
        ${Boost_LIBRARIES}
        ${PYTHON_LIBRARIES})
endif(WIN32)

if(LINUX)
    target_link_libraries(dispglonass PRIVATE ${CMAKE_SOURCE_DIR}/libxlntd.a)
endif(LINUX)

target_link_libraries(dispglonass PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.dispglonass)
endif()
set_target_properties(dispglonass PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS dispglonass
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(dispglonass)
endif()
