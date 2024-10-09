cmake_minimum_required(VERSION 3.5)

project(dispglonass VERSION 0.1 LANGUAGES CXX)
#set(Boost_NO_WARN_NEW_VERSIONS 1)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

if(WIN32)
    set(Boost_COMPILER -mgw11)
    set(Boost_ARCHITECTURE -x64)
    set(BOOST_ROOT A:/libraries/boost_1_86_0)
    list(APPEND CMAKE_PREFIX_PATH "A:/usr/local")
    find_package(Boost 1.86.0 REQUIRED COMPONENTS python312 json)
endif(WIN32)
if(LINUX)
    set(BOOST_ROOT /home/godnessli/Загрузки/boost_1_86_0)
    find_package(Boost 1.86.0 REQUIRED COMPONENTS python312 json)
endif()

include_directories(modules)
add_definitions(-DUNICODE -D_UNICODE)

find_package(PythonLibs REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PYTHON_INCLUDE_DIRS})

find_package(Xlnt)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)

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
        modules/buildreporttable.h
        modules/buildreporttable.cpp
        modules/readtransportworktable.h
        modules/readtransportworktable.cpp
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
    target_link_libraries(dispglonass PRIVATE xlnt::xlnt)
endif(WIN32)

if(LINUX)
    target_link_libraries(dispglonass PRIVATE xlnt::xlnt)
endif(LINUX)

target_compile_definitions(dispglonass PRIVATE QT_NO_KEYWORDS)
target_link_libraries(dispglonass PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(dispglonass PRIVATE
    ${Boost_LIBRARIES}
    ${PYTHON_LIBRARIES})

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
