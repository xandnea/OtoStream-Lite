cmake_minimum_required(VERSION 3.16)
project(OtoStream-Lite VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable MOC and UI compilers
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

# Use the recommended Qt wrapper to create the executable
qt_add_executable(OtoStream-Lite
    WIN32 MACOSX_BUNDLE
    main.cpp
    MainWindow.cpp
    MainWindow.h
    MainWindow.ui
    FrameWorker.cpp
    FrameWorker.h
)

# Link your Qt6 libraries cleanly in one place
target_link_libraries(OtoStream-Lite
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)

# Deployment and Installation rules
install(TARGETS OtoStream-Lite
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET OtoStream-Lite
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
